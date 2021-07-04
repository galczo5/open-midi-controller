#include <Arduino.h>
#include "controller.h"
#include "controller-state.h"

Controller::Controller(ControllerState initState) {
    this->state = initState;
    this->stateChanged = false;
}

boolean Controller::inState(ControllerState state) {
    return state == this->state;
}

void Controller::enterState(ControllerState state) {
    this->state = state;
    this->stateChanged = true;
}

boolean Controller::checkChanges() {
    boolean prevState = this->stateChanged;
    this->stateChanged = false;

    return prevState;
}
