#ifndef CONFIGURATION_STATE_H
#define CONFIGURATION_STATE_H

enum ConfigurationState {
    SELECT_FOOTSWITCH = -1,
    SELECT_CHANNEL = 0,
    SELECT_TYPE = 1,
    SELECT_VALUE1 = 2,
    SELECT_VALUE2 = 3,
    EXIT = 4
};

#endif