#include "configuration.h"
#include "configuration-state.h"
#include "config/command-type.h"
#include "config/controller-button.h"

#define CHANNEL 0
#define TYPE 1
#define VALUE1 2
#define VALUE2 3
#define VALUE3 4

void Configuration::reset() {
    this->state = ConfigurationState::SELECT_FOOTSWITCH;

    this->value = 0;
    this->footswitchNo = 0;

    this->configBytes[CHANNEL] = 0;
    this->configBytes[TYPE] = 0;
    this->configBytes[VALUE1] = 0;
    this->configBytes[VALUE2] = 0;
    this->configBytes[VALUE3] = 0;
}

void Configuration::next() {
    this->configBytes[this->state] = this->value;

    if (state == ConfigurationState::SELECT_VALUE2 && this->configBytes[TYPE] == CommandType::TOGGLE_CC) {
        this->state = ConfigurationState::SELECT_VALUE3;
    } else if (state == ConfigurationState::SELECT_VALUE2) {
        this->state = ConfigurationState::EXIT;
    } else {
        this->state = static_cast<ConfigurationState>(this->state + 1);
    }

    this->value = 0;
}

ConfigurationState Configuration::getState() {
    return this->state;
}

void Configuration::incrementValue() {

    int numberOfValues = 128;

    if (this->state == ConfigurationState::SELECT_TYPE) {
        numberOfValues = 6;
    }

    this->value = (this->value + 1) % numberOfValues;
}

byte Configuration::getValue() {
    return this->value;
}

void Configuration::setFootswitch(int no, boolean longClick) {
    this->footswitchNo = no;
    this->longClick = longClick;
}

int Configuration::getFootswitch() {
    return this->footswitchNo;
}

ControllerButton Configuration::getControllerButton() {
    ControllerButton button;
    button.channel = this->configBytes[CHANNEL];
    button.type = this->configBytes[TYPE];
    button.value1 = this->configBytes[VALUE1];
    button.value2 = this->configBytes[VALUE2];
    button.value2 = this->configBytes[VALUE3];

    return button;
}

boolean Configuration::isLongClick() {
    return this->longClick;
}
