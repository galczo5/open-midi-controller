#ifndef PRINTER_H
#define PRINTER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "configuration/configuration-state.h"
#include "config/controller-button-entity.h"
#include "config/midi-controller-config.h"
#include "config/command-type.h"

class Printer {
    private:
        String valueToCommandTypeLabel(byte value);
        LiquidCrystal_I2C lcd;

    public:
        Printer();
        void init();
        void welcome(String revision);
        void enterConfiguration();
        void leaveConfiguration();
        void selectFootswitchPrompt();
        void configurationPrompt(ConfigurationState state, byte value, CommandType commandType = CommandType::UNSET);
        void commandInfo(int footswitchNo, int page, ControllerButtonEntity btn, byte lastValue);
        void printConfigPage(MidiControllerConfig *config);
        void debug(String value);
};

#endif