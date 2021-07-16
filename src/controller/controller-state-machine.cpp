#include <Arduino.h>
#include "controller-state-machine.h"
#include "controller-state.h"

ControllerStateMachine::ControllerStateMachine(ControllerState initState) {
    this->state = initState;
    this->stateChanged = false;
}

boolean ControllerStateMachine::inState(ControllerState state) {
    return state == this->state;
}

void ControllerStateMachine::enterState(ControllerState state) {
    this->state = state;
    this->stateChanged = true;
}

boolean ControllerStateMachine::checkChanges() {
    boolean prevState = this->stateChanged;
    this->stateChanged = false;

    return prevState;
}

void ControllerStateMachine::toggleState() {
    if (this->inState(ControllerState::SEND_COMMAND)) {
      this->enterState(ControllerState::CONFIGURE);
    } else if (this->inState(ControllerState::CONFIGURE)) {
      this->enterState(ControllerState::SEND_COMMAND);
    }
}
