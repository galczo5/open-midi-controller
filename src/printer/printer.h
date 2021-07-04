#ifndef PRINTER_H
#define PRINTER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "configuration/configuration-state.h"
#include "config/controller-button.h"

class Printer {
    private:
        String valueToCommandTypeLabel(byte value);

    public:
        void welcome(LiquidCrystal_I2C lcd, int revision);
        void enterConfiguration(LiquidCrystal_I2C lcd);
        void leaveConfiguration(LiquidCrystal_I2C lcd);
        void selectFootswitchPrompt(LiquidCrystal_I2C lcd);
        void configurationPrompt(LiquidCrystal_I2C lcd, ConfigurationState state, byte value);
        void commandInfo(LiquidCrystal_I2C lcd, int footswitchNo, ControllerButton* btn);
};

#endif