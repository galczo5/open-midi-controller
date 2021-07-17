#include "configuration-state-machine.h"
#include "configuration-state.h"
#include "config/command-type.h"
#include "config/controller-button-entity.h"

#define CHANNEL 0
#define TYPE 1
#define VALUE1 2
#define VALUE2 3
#define VALUE3 4

int states[7][5] = {
    { CommandType::UNSET, ConfigurationState::EXIT },
    { CommandType::CC, ConfigurationState::SELECT_VALUE1, ConfigurationState::EXIT },
    { CommandType::TOGGLE_CC, ConfigurationState::SELECT_VALUE1, ConfigurationState::SELECT_VALUE2, ConfigurationState::SELECT_VALUE3, ConfigurationState::EXIT },
    { CommandType::NOTE, ConfigurationState::SELECT_VALUE1, ConfigurationState::SELECT_VALUE2, ConfigurationState::EXIT },
    { CommandType::NEXT_PAGE, ConfigurationState::EXIT },
    { CommandType::PREV_PAGE, ConfigurationState::EXIT },
    { CommandType::PAGE, ConfigurationState::SELECT_VALUE1, ConfigurationState::EXIT }
};

void ConfigurationStateMachine::reset() {
    this->state = ConfigurationState::SELECT_FOOTSWITCH;

    this->value = 0;
    this->footswitchNo = 0;

    this->configBytes[CHANNEL] = 0;
    this->configBytes[TYPE] = 0;
    this->configBytes[VALUE1] = 0;
    this->configBytes[VALUE2] = 0;
    this->configBytes[VALUE3] = 0;
}

void ConfigurationStateMachine::next() {
    this->configBytes[this->state] = this->value;
    this->value = 0;

    if (state < TYPE) {
        return;
    }

    for (int i = 0; i < 7; i++) {
        boolean typeMatches = states[i][0] == this->configBytes[TYPE];
        if (typeMatches) {
            for (int j = 0; j < 5; j++) {
                if (states[i][j] == this->state) {
                    this->state = static_cast<ConfigurationState>(states[i][j + 1]);
                    return;
                }
            }
        }
    }
}

ConfigurationState ConfigurationStateMachine::getState() {
    return this->state;
}

void ConfigurationStateMachine::incrementValue() {

    int numberOfValues = 128;

    if (this->state == ConfigurationState::SELECT_TYPE) {
        numberOfValues = 7;
    } else if (this->state == ConfigurationState::SELECT_VALUE1 && this->configBytes[TYPE] == CommandType::PAGE) {
        numberOfValues = 5;
    }

    this->value = (this->value + 1) % numberOfValues;
}

byte ConfigurationStateMachine::getValue() {
    return this->value;
}

void ConfigurationStateMachine::setFootswitch(int no, boolean longClick) {
    this->footswitchNo = no;
    this->longClick = longClick;
}

int ConfigurationStateMachine::getFootswitch() {
    return this->footswitchNo;
}

ControllerButtonEntity ConfigurationStateMachine::getControllerButton() {
    ControllerButtonEntity button;
    button.channel = this->configBytes[CHANNEL];
    button.type = this->configBytes[TYPE];
    button.value1 = this->configBytes[VALUE1];
    button.value2 = this->configBytes[VALUE2];
    button.value2 = this->configBytes[VALUE3];

    return button;
}

boolean ConfigurationStateMachine::isLongClick() {
    return this->longClick;
}


CommandType ConfigurationStateMachine::getCommandType() {
    return static_cast<CommandType>(this->configBytes[TYPE]);
}