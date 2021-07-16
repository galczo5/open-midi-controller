#ifndef PRINTER_H
#define PRINTER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "configuration/configuration-state.h"
#include "config/controller-button-entity.h"

class Printer {
    private:
        String valueToCommandTypeLabel(byte value);
        LiquidCrystal_I2C lcd;

    public:
        Printer();
        void init();
        void welcome(int revision);
        void enterConfiguration();
        void leaveConfiguration();
        void selectFootswitchPrompt();
        void configurationPrompt(ConfigurationState state, byte value);
        void commandInfo(int footswitchNo, ControllerButtonEntity* btn);
};

#endif