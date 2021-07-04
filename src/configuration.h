#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>
#include "configuration-state.h"
#include "controller-button.h"

class Configuration {
    private:
        ConfigurationState state;
        int footswitchNo;
        byte value;
        byte configBytes[4];

    public:
        void reset();
        void next();
        ConfigurationState getState();
        void incrementValue();
        byte getValue();
        void setFootswitch(int no);
        int getFootswitch();
        ControllerButton getControllerButton();
};

#endif