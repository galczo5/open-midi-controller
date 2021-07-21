#include "configurator.h"
#include "configuration-state.h"
#include "footswitch/footswitch.h"
#include "config/command-type.h"
#include "consts.h"

Configurator::Configurator(MidiControllerConfig *config, ConfigurationStateMachine *configurationStateMachine, Printer *printer) {
    this->configurationStateMachine = configurationStateMachine;
    this->printer = printer;
    this->config = config;
}

void Configurator::configure(Footswitch* footswitches[]) {
    ConfigurationState configurationState = this->configurationStateMachine->getState();
    
    // Select footswitch
    if (configurationState == ConfigurationState::SELECT_FOOTSWITCH) {
        for (int i = 0; i < NUMBER_OF_FOOTSWITCHES; i++) {
            FootswitchState click = footswitches[i]->checkClicked();
            if (click == FootswitchState::CLICK || click == FootswitchState::LONG_CLICK) {
                this->configurationStateMachine->setFootswitch(footswitches[i]->getNumber(), click == FootswitchState::LONG_CLICK);
                this->configurationStateMachine->next();

                this->printer->configurationPrompt(
                    this->configurationStateMachine->getState(), 
                    this->configurationStateMachine->getValue(),
                    CommandType::UNSET
                );
                return;
            }
        }

        return;
    } 
    
    if (configurationState == ConfigurationState::EXIT) {
        return;
    }

    ConfigurationState state = this->configurationStateMachine->getState();
    CommandType commandType = this->configurationStateMachine->getCommandType();

    FootswitchState fsIncrementState = footswitches[FS_CONFIG_INCREMENT]->checkClicked();
    FootswitchState fsDecrementState = footswitches[FS_CONFIG_DECREMENT]->checkClicked();
    FootswitchState fsNextState = footswitches[FS_CONFIG_NEXT]->checkClicked();
    
    if (fsIncrementState == FootswitchState::CLICK) {
        this->configurationStateMachine->incrementValue();
        byte value = this->configurationStateMachine->getValue();
        this->printer->configurationPrompt(state, value, commandType);

    } else if (fsDecrementState == FootswitchState::CLICK) {
        this->configurationStateMachine->decrementValue();
        byte value = this->configurationStateMachine->getValue();
        this->printer->configurationPrompt(state, value, commandType);

    } 
    
    if (fsNextState == FootswitchState::CLICK) {
        ConfigurationState newState = this->configurationStateMachine->next();

        if (newState == ConfigurationState::EXIT) {
            this->config->setButton(
                this->configurationStateMachine->getFootswitch(), 
                this->configurationStateMachine->getControllerButton(),
                this->configurationStateMachine->isLongClick()
            );

            this->configurationStateMachine->reset();
            this->printer->selectFootswitchPrompt();
        } else {
            this->printer->configurationPrompt(
                this->configurationStateMachine->getState(), 
                this->configurationStateMachine->getValue(),
                this->configurationStateMachine->getCommandType()
            );
        }
    }
}