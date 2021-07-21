#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include "configuration-state-machine.h"
#include "printer/printer.h"
#include "footswitch/footswitch.h"
#include "config/midi-controller-config.h"

class Configurator {

    private: 
        MidiControllerConfig *config;
        ConfigurationStateMachine *configurationStateMachine;
        Printer *printer;

    public:
        Configurator(MidiControllerConfig *config, ConfigurationStateMachine *configurationStateMachine, Printer *printer);
        void configure(Footswitch* footswitches[]);
};

#endif