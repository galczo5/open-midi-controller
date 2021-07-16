#include <Arduino.h>
#include <MIDI.h>
#include "executor/command-executor.h"
#include "config/command-type.h"

MIDI_CREATE_DEFAULT_INSTANCE();

CommandExecutor::CommandExecutor(MidiControllerConfig* config) {
    this->config = config;
    this->toggleIterator = 0;
}

void CommandExecutor::init() {
    MIDI.begin();
}

int CommandExecutor::getLastValue(int no, int page) {
    String key = this->composeKey(no, page);
    int keyIndex = -1;

    for (int i = 0; i < TOGGLE_HISTORY_SIZE; i++) {
        if (this->toggleKeys[i].equals(key)) {
            keyIndex = i;
        }
    }

    if (keyIndex >= 0) {
        return this->toggleValues[keyIndex];
    } else {
        return -1;
    }
}

String CommandExecutor::composeKey(int no, int page) {
    return String(page) + 'x' + String(no);
}

void CommandExecutor::saveToggleHistory(int no, int page, byte value) {
    this->toggleKeys[this->toggleIterator] = this->composeKey(no, page);
    this->toggleValues[this->toggleIterator] = value;
    this->toggleIterator++;
}

void CommandExecutor::executeCommand(int no, boolean longClick) {

    ControllerButtonEntity entity = this->config->getButtonData(no, longClick);

    int page = config->getPage();

    switch (entity.type) {
        case CommandType::NOTE:
            MIDI.sendNoteOn(entity.value1, 127, entity.channel);
            break;

        case CommandType::CC:
            MIDI.sendControlChange(entity.value1, 127, entity.channel);
            break;

        case CommandType::TOGGLE_CC:
            int lastValue = this->getLastValue(no, page);

            byte valueToSend = entity.value2 != lastValue 
                ? entity.value2 
                : entity.value3;

            MIDI.sendControlChange(entity.value1, valueToSend, entity.channel);
            this->saveToggleHistory(no, page, valueToSend);
            break;

        case CommandType::NEXT_PAGE:
            config->setPage(page + 1);
            break;

        case CommandType::PREV_PAGE:
            config->setPage(page - 1);
            break;

        case CommandType::PAGE:
            config->setPage(entity.value1);
            break;
    
    }

}