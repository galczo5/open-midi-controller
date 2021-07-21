#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "controller-state.h"

class ControllerStateMachine {
    private:
        boolean stateChanged;

    public:
        ControllerState state;
        ControllerStateMachine(ControllerState initState);
        ControllerState getState();
        void enterState(ControllerState state);
        void toggleState();
        boolean checkChanges();
};

#endif