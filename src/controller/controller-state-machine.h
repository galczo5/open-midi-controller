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
        boolean inState(ControllerState state);
        void enterState(ControllerState state);
        boolean checkChanges();
};

#endif