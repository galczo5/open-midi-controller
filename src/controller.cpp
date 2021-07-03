#include <Arduino.h>
#include "controller.h"
#include "controller-state.h"

Controller::Controller() {
    this->state = ControllerState::SEND_COMMAND;
}

boolean Controller::inState(ControllerState state) {
    return state == this->state;
}

void Controller::enterState(ControllerState state) {
    this->state = state;
}
