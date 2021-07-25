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
        MidiControllerConfig *config;

    public:
        Printer(MidiControllerConfig *config);
        void init();
        void welcome(String revision);
        void enterConfiguration();
        void leaveConfiguration();
        void selectFootswitchPrompt();
        void configurationPrompt(ConfigurationState state, byte value, CommandType commandType);
        void commandInfo(int footswitchNo, boolean longClick, byte lastValue);
        void printConfigPage(MidiControllerConfig *config);
        void changeModeMessage(boolean inConfigurationMode);
        void usbMode(boolean enabled);
};

#endif