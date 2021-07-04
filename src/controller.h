#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "controller-state.h"

class Controller {
    private:
        boolean stateChanged;

    public:
        ControllerState state;
        Controller(ControllerState initState);
        boolean inState(ControllerState state);
        void enterState(ControllerState state);
        boolean checkChanges();
};

#endif