#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "configuration/configuration-state.h"
#include "config/command-type.h"
#include "printer.h"

#define MESSAGE_TIMEOUT 1500

Printer::Printer() : lcd(0x27, 20, 4) {
}

void Printer::init() {
  lcd.init();
  lcd.backlight();
}

void Printer::welcome(int revision) {
    this->lcd.setCursor(0, 0);
    this->lcd.print("MIDI CONTROLLER");
    this->lcd.setCursor(0, 1);
    this->lcd.print("REVISION " + String(revision));
    delay(MESSAGE_TIMEOUT);
    this->lcd.clear();
}

void Printer::enterConfiguration() {
    this->lcd.clear();
    this->lcd.setCursor(0, 0);
    this->lcd.print("MIDI CONTROLLER");
    this->lcd.setCursor(0, 1);
    this->lcd.print("CONFIGURATION");
    delay(MESSAGE_TIMEOUT);
    this->lcd.clear();
}

void Printer::leaveConfiguration() {
    this->lcd.clear();
    this->lcd.setCursor(0, 0);
    this->lcd.print("MIDI CONTROLLER");
    this->lcd.setCursor(0, 1);
    this->lcd.print("SAVED");
    delay(MESSAGE_TIMEOUT);
    this->lcd.clear();
}

void Printer::selectFootswitchPrompt() {
    this->lcd.clear();
    this->lcd.setCursor(0, 0);
    this->lcd.print("SELECT");
    this->lcd.setCursor(0, 1);
    this->lcd.print("FOOTSWITCH");
}

void Printer::configurationPrompt(ConfigurationState state, byte value) {
    String line1;
    String line2;

    if (state == ConfigurationState::SELECT_CHANNEL) {
        line1 = "MIDI CHANNEL";
        line2 = String(value);
    } else if (state == ConfigurationState::SELECT_TYPE) {
        line1 = "COMMAND TYPE";
        line2 = this->valueToCommandTypeLabel(value);
    } else if (state == ConfigurationState::SELECT_VALUE1) {
        line1 = "NOTE/CC/PAGE";
        line2 = String(value);
    } else if (state == ConfigurationState::SELECT_VALUE2) {
        line1 = "VALUE";
        line2 = String(value);
    } else if (state == ConfigurationState::SELECT_VALUE3) {
        line1 = "TOGGLE VALUE";
        line2 = String(value);
    } else {
        line1 = "ERROR - UNKNOWN";
        line2 = "STATE: " + String(state);
    }

    this->lcd.clear();
    this->lcd.setCursor(0, 0);
    this->lcd.print(line1);
    this->lcd.setCursor(0, 1);
    this->lcd.print(line2);
}

String Printer::valueToCommandTypeLabel(byte value) {

    switch (value) {
    case CommandType::UNSET:
        return "UNSET";
    case CommandType::NOTE:
        return "NOTE";
    case CommandType::CC:
        return "CC";
    case CommandType::TOGGLE_CC:
        return "TOGGLE CC";
    case CommandType::NEXT_PAGE:
        return "NEXT PAGE";
    case CommandType::PREV_PAGE:
        return "PREV PAGE";
    case CommandType::PAGE:
        return "GO TO PAGE";
    
    default:
        return "ERROR: UNKNOWN";
    }

}

void Printer::commandInfo(int footswitchNo, ControllerButtonEntity* btn) {
    this->lcd.clear();
    this->lcd.setCursor(0, 0);
    this->lcd.print("FOOTSWITCH " + String(footswitchNo));
    this->lcd.setCursor(0, 1);
    if (btn->type == CommandType::TOGGLE_CC) {
        this->lcd.print(
            this->valueToCommandTypeLabel(CommandType::CC) + " " + 
            String(btn->value1) + " " + 
            String(btn->value2) + " " + 
            String(btn->value3)
        );
    } else {
        this->lcd.print(
            this->valueToCommandTypeLabel(btn->type) + " " + 
            String(btn->value1) + " " + 
            String(btn->value2)
        );
    }
    
}