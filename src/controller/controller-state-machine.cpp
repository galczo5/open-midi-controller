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
