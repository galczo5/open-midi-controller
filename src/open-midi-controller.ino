#define REVISION "20210801"

#include <MIDI.h>

#include "consts.h"
#include "config/midi-controller-config.h"
#include "footswitch/footswitch.h"
#include "config/command-type.h"
#include "controller/controller-state-machine.h"
#include "printer/printer.h"
#include "configuration/configuration-state-machine.h"
#include "executor/command-executor.h"
#include "configuration/configurator.h"

/**
 * Open Midi Controller
 * Footswitch pin map:
 *  __________________________
 * |                          |
 * | fs1 D2   fs3 D4   fs5 D6 |
 * |                          |
 * |                          |
 * | fs2 D3   fs4 D5   fs6 D7 |
 * |__________________________|
 */

// INIT FOOTSWITCHES
Footswitch fs1(0, FS_1_PIN);
Footswitch fs2(1, FS_2_PIN);
Footswitch fs3(2, FS_3_PIN);
Footswitch fs4(3, FS_4_PIN);
Footswitch fs5(4, FS_5_PIN);
Footswitch fs6(5, FS_6_PIN);

Footswitch* footswitches[6] = { &fs1, &fs2, &fs3, &fs4, &fs5, &fs6 };

MidiControllerConfig config;
ControllerStateMachine controllerStateMachine(ControllerState::SEND_COMMAND);
ConfigurationStateMachine configurationStateMachine;
Printer printer(&config);
CommandExecutor commandExecutor(&config, &printer);
Configurator configurator(&config, &configurationStateMachine, &printer);
boolean usbModeButtonsPressed = false;
void(* resetFunc) (void) = 0;

void setup() {
    commandExecutor.init();

    printer.init();
    printer.welcome(REVISION);

    boolean usbMode = config.isInUsbMidiMode();
    if (usbMode) {
        Serial.begin(115200);
    }

    printer.usbMode(usbMode);

    for (Footswitch* fs : footswitches) { 
        fs->init(); 
    }
}

void loop() {

    if (tryReadSerialPort()) {
        return;
    }

    for (Footswitch* fs : footswitches) {
        fs->scan();
    }
    
    if (infoSwitchesPressed() || configSwitchesPressed() || usbModeSwitchesPressed()) {
        return;
    }

    ControllerState controllerState = controllerStateMachine.getState();

    if (controllerStateMachine.checkChanges()) {
        configurationStateMachine.reset();
        printer.changeModeMessage(controllerState == ControllerState::CONFIGURE);    
        return;
    }

    switch (controllerState) {
        case ControllerState::CONFIGURE:
            configurator.configure(footswitches);
            break;

        case ControllerState::SEND_COMMAND:
            commandExecutor.sendCommands(footswitches);
            break;
    }

}

boolean tryReadSerialPort() {
    boolean usbMode = config.isInUsbMidiMode();
    if (usbMode && Serial.available() > 0) {
        byte bytes[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        Serial.readBytes(bytes, 8);
        
        if (bytes[0] == 0) {
            int page = bytes[1];
            config.setPage(page);
            printer.usbPageChanged(page);
        } else if (bytes[0] == 1) {
            ControllerButtonEntity button = { bytes[3], bytes[4], bytes[5], bytes[6], bytes[7] };

            FootswitchState state;
            int fsStateByte = bytes[2];

            if (fsStateByte == 0) {
                state = FootswitchState::NONE;
            } else if (fsStateByte == 1) {
                state = FootswitchState::CLICK;
            } else if (fsStateByte == 2) {
                state = FootswitchState::LONG_CLICK;
            } else if (fsStateByte == 3) {
                state = FootswitchState::DOUBLE_CLICK;
            }

            int fsNo = bytes[1];
            config.setButton(fsNo, button, state);
            printer.commandInfo(fsNo, state, 0);
        }

        return true;
    }

    return false;
}

boolean usbModeSwitchesPressed() {
    FootswitchState fs1State = footswitches[FS_USB_MIDI_1]->checkClicked();
    FootswitchState fs2State = footswitches[FS_USB_MIDI_2]->checkClicked();

    if (fs1State == FootswitchState::PRESSED && fs2State == FootswitchState::PRESSED) {
        usbModeButtonsPressed = true;
    }

    if (fs1State == FootswitchState::NONE && usbModeButtonsPressed) {
        config.setUsbMidiMode(!config.isInUsbMidiMode());
        resetFunc();
        return true;
    }

    return false;
}

boolean infoSwitchesPressed() {
    FootswitchState fs1State = footswitches[FS_INFO_1]->checkClicked();
    FootswitchState fs2State = footswitches[FS_INFO_2]->checkClicked();

    if (fs1State == FootswitchState::PRESSED && fs2State == FootswitchState::PRESSED) {
        configurationStateMachine.setShouldPrintInfo(true);
    }

    if (fs1State == FootswitchState::NONE && configurationStateMachine.shouldPrintInfo()) {
        configurationStateMachine.setShouldPrintInfo(false);
        printer.printConfigPage(&config);
        return true;
    }

    return false;
}

boolean configSwitchesPressed() {
    FootswitchState fs1State = footswitches[FS_CONFIG_1]->checkClicked();
    FootswitchState fs2State = footswitches[FS_CONFIG_2]->checkClicked();

    if (fs1State == FootswitchState::PRESSED && fs2State == FootswitchState::PRESSED) {
        configurationStateMachine.setShouldEnterConfiguration(true);
    }
    
    if (fs1State == FootswitchState::NONE && configurationStateMachine.shouldEnterConfiguration()) {
        controllerStateMachine.toggleState();
        configurationStateMachine.setShouldEnterConfiguration(false);
        return true;
    }

    return configurationStateMachine.shouldEnterConfiguration();;
}
