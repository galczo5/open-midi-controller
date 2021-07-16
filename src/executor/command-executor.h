#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include <Arduino.h>
#include <MIDI.h>
#include "config/controller-button-entity.h"
#include "config/midi-controller-config.h"

#define TOGGLE_HISTORY_SIZE 10

class CommandExecutor {
    private:
        MidiControllerConfig *config;

        int toggleIterator;
        String toggleKeys[TOGGLE_HISTORY_SIZE];
        byte toggleValues[TOGGLE_HISTORY_SIZE];

        int getLastValue(int no, int page);
        String composeKey(int no, int page);
        void saveToggleHistory(int no, int page, byte value);

    public:
        CommandExecutor(MidiControllerConfig* config);
        void init();
        void executeCommand(int no, boolean longClick);

};

#endif