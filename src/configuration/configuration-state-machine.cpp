#include "configuration-state-machine.h"
#include "configuration-state.h"
#include "config/command-type.h"
#include "config/controller-button-entity.h"

#define CHANNEL 0
#define TYPE 1
#define VALUE1 2
#define VALUE2 3
#define VALUE3 4

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

ConfigurationState ConfigurationStateMachine::next() {
    this->configBytes[this->state] = this->value;
    this->value = 0;

    if (this->state == ConfigurationState::SELECT_VALUE2 && this->configBytes[TYPE] == CommandType::CC) {
        this->state = ConfigurationState::EXIT;
    } else if (this->state == ConfigurationState::SELECT_VALUE2 && this->configBytes[TYPE] == CommandType::NOTE) {
        this->state = ConfigurationState::EXIT;
    } else if (this->state == ConfigurationState::SELECT_TYPE && this->configBytes[TYPE] == CommandType::NEXT_PAGE) {
        this->state = ConfigurationState::EXIT;
    } else if (this->state == ConfigurationState::SELECT_TYPE && this->configBytes[TYPE] == CommandType::PREV_PAGE) {
        this->state = ConfigurationState::EXIT;
    } else if (this->state == ConfigurationState::SELECT_TYPE && this->configBytes[TYPE] == CommandType::UNSET) {
        this->state = ConfigurationState::EXIT;
    } else if (this->state == ConfigurationState::SELECT_VALUE1 && this->configBytes[TYPE] == CommandType::PAGE) {
        this->state = ConfigurationState::EXIT;
    } else {
        this->state = static_cast<ConfigurationState>(this->state + 1);
    }

    return this->state;
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

void ConfigurationStateMachine::decrementValue() {

    int numberOfValues = 128;

    if (this->state == ConfigurationState::SELECT_TYPE) {
        numberOfValues = 7;
    } else if (this->state == ConfigurationState::SELECT_VALUE1 && this->configBytes[TYPE] == CommandType::PAGE) {
        numberOfValues = 5;
    }

    int newValue = this->value - 1;
    if (newValue < 0) {
        newValue = numberOfValues - 1;
    }

    this->value = newValue % numberOfValues;
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
    ControllerButtonEntity button = {
        this->configBytes[CHANNEL],
        this->configBytes[TYPE],
        this->configBytes[VALUE1],
        this->configBytes[VALUE2],
        this->configBytes[VALUE3]
    };

    return button;
}

boolean ConfigurationStateMachine::isLongClick() {
    return this->longClick;
}

CommandType ConfigurationStateMachine::getCommandType() {
    return static_cast<CommandType>(this->configBytes[TYPE]);
}

boolean ConfigurationStateMachine::shouldEnterConfiguration() {
    return this->enterConfiguration;
}

void ConfigurationStateMachine::setShouldEnterConfiguration(boolean state) {
    this->enterConfiguration = state;
}

boolean ConfigurationStateMachine::shouldPrintInfo() {
    return this->printInfo;
}

void ConfigurationStateMachine::setShouldPrintInfo(boolean state) {
    this->printInfo = state;
}