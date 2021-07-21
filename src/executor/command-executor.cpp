#include <Arduino.h>
#include <MIDI.h>
#include "executor/command-executor.h"
#include "config/command-type.h"
#include "consts.h"

MIDI_CREATE_DEFAULT_INSTANCE();

CommandExecutor::CommandExecutor(MidiControllerConfig* config, Printer *printer) {
    this->config = config;
    this->printer = printer;
    this->toggleIterator = 0;
}

void CommandExecutor::init() {
    MIDI.begin();
}

int CommandExecutor::getLastValue(int no, int page) {
    String key = this->composeKey(no, page);
    int keyIndex = -1;

    for (int i = 0; i < TOGGLE_HISTORY_SIZE; i++) {
        if (this->toggleKeys[i] == key) {
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
    String key = this->composeKey(no, page);
    for (int i = 0; i < TOGGLE_HISTORY_SIZE; i++) {
        if (this->toggleKeys[i] == key) {
            this->toggleValues[i] = value;
            return;        
        }
    }

    this->toggleKeys[this->toggleIterator] = key;
    this->toggleValues[this->toggleIterator] = value;
    this->toggleIterator = (this->toggleIterator + 1) % TOGGLE_HISTORY_SIZE;
}

void CommandExecutor::executeCommand(int no, boolean longClick) {
    ControllerButtonEntity entity = this->config->getButtonData(no, longClick);
    int page = this->config->getPage();

    switch (entity.type) {
        case byte(CommandType::NOTE): {
            MIDI.sendNoteOn(entity.value1, 127, entity.channel);
            this->lastValue = 127;
            break;
        }

        case byte(CommandType::CC): {
            MIDI.sendControlChange(entity.value1, entity.value2, entity.channel);
            this->lastValue = entity.value2;
            break;
        }

        case byte(CommandType::TOGGLE_CC): {
            int lastValue = this->getLastValue(no, page);

            byte valueToSend = entity.value2 != lastValue 
                ? entity.value2 
                : entity.value3;

            MIDI.sendControlChange(entity.value1, valueToSend, entity.channel);
            this->saveToggleHistory(no, page, valueToSend);
            this->lastValue = valueToSend;
            break;
        }

        case byte(CommandType::NEXT_PAGE): {
            this->config->setPage(page + 1);
            break;
        }

        case byte(CommandType::PREV_PAGE): {
            this->config->setPage(page - 1);
            break;
        }

        case byte(CommandType::PAGE): {
            this->config->setPage(entity.value1);
            break;
        }
    }
}

byte CommandExecutor::getExecutedValue() {
    return this->lastValue;
}

void CommandExecutor::sendCommands(Footswitch* footswitches[]) {
    for (int i = 0; i < NUMBER_OF_FOOTSWITCHES; i++) {
        FootswitchState state = footswitches[i]->checkClicked();

        if (state & FootswitchState::ANY_CLICK) {
            int no = footswitches[i]->getNumber();
            boolean longClick = state == FootswitchState::LONG_CLICK;
            
            this->executeCommand(no, longClick);
            this->printer->commandInfo(no, longClick, this->getExecutedValue());

            return;
        }
    }
}