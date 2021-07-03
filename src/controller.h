#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "controller-state.h"

class Controller {
    private:
        ControllerState state;

    public:
        Controller();
        boolean inState(ControllerState state);
        void enterState(ControllerState state);
};

#endif