#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>
#include "configuration-state.h"
#include "config/controller-button-entity.h"
#include "config/command-type.h"
#include "footswitch/footswitch-state.h"

class ConfigurationStateMachine {
    private:
        byte configBytes[5];
        ConfigurationState state;
        int footswitchNo;
        byte value;
        FootswitchState click;
        boolean enterConfiguration;
        boolean printInfo;

    public:
        void reset();
        ConfigurationState next();
        
        ConfigurationState getState();
        
        void incrementValue();
        void decrementValue();
        byte getValue();

        void setFootswitch(int no, FootswitchState click);
        int getFootswitch();
        FootswitchState getFootswitchState();

        ControllerButtonEntity getControllerButton();
        CommandType getCommandType();

        boolean shouldEnterConfiguration();
        void setShouldEnterConfiguration(boolean state);

        boolean shouldPrintInfo();
        void setShouldPrintInfo(boolean state);
};

#endif