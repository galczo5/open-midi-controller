#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>
#include "configuration-state.h"
#include "config/controller-button-entity.h"
#include "config/command-type.h"

class ConfigurationStateMachine {
    private:
        byte configBytes[5];
        ConfigurationState state;
        int footswitchNo;
        byte value;
        boolean longClick;
        boolean enterConfiguration;
        boolean printInfo;

    public:
        void reset();
        ConfigurationState next();
        
        ConfigurationState getState();
        
        void incrementValue();
        void decrementValue();
        byte getValue();

        void setFootswitch(int no, boolean longClick);
        int getFootswitch();
        boolean isLongClick();

        ControllerButtonEntity getControllerButton();
        CommandType getCommandType();

        boolean shouldEnterConfiguration();
        void setShouldEnterConfiguration(boolean state);

        boolean shouldPrintInfo();
        void setShouldPrintInfo(boolean state);
};

#endif