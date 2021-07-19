#define REVISION "20210718"

#include <MIDI.h>

#include "config/midi-controller-config.h"
#include "footswitch/footswitch.h"
#include "config/command-type.h"
#include "controller/controller-state-machine.h"
#include "printer/printer.h"
#include "configuration/configuration-state-machine.h"
#include "executor/command-executor.h"

/**
 * Open Midi Controller
 * 
 * Footswitch pin map:
 *  __________________________
 * |                          |
 * | fs1 D2   fs3 D4   fs5 D6 |
 * |                          |
 * |                          |
 * | fs2 D3   fs4 D5   fs6 D7 |
 * |__________________________|
 */

#define FS_1_PIN 2 
#define FS_2_PIN 3
#define FS_3_PIN 4
#define FS_4_PIN 5
#define FS_5_PIN 6
#define FS_6_PIN 7

#define FS_CONFIG_1 1
#define FS_CONFIG_2 3
#define FS_CONFIG_DECREMENT 0

#define FS_INFO_1 0
#define FS_INFO_2 2

// INIT FOOTSWITCHES
Footswitch fs1(0, FS_1_PIN);
Footswitch fs2(1, FS_2_PIN);
Footswitch fs3(2, FS_3_PIN);
Footswitch fs4(3, FS_4_PIN);
Footswitch fs5(4, FS_5_PIN);
Footswitch fs6(5, FS_6_PIN);

Footswitch* footswitches[6] = { &fs1, &fs2, &fs3, &fs4, &fs5, &fs6 };

boolean modeShouldChange = false;
boolean infoShouldBePrinted = false;

Printer printer;

MidiControllerConfig *config = new MidiControllerConfig();
ControllerStateMachine controllerStateMachine(ControllerState::SEND_COMMAND);
ConfigurationStateMachine configurationStateMachine;
CommandExecutor commandExecutor(config);


void setup() {
  commandExecutor.init();

  printer.init();
  printer.welcome(REVISION);

  for (Footswitch* fs : footswitches) { 
    fs->init(); 
  }
}

void loop() {

  for (Footswitch* fs : footswitches) {
    fs->scan();
  }
  
  if (infoSwitchesPressed()) {
    return;
  }

  if (configSwitchesPressed()) {
    return;
  }

  if (controllerStateMachine.checkChanges()) {
    changeMode();
    return;
  }
  
  if (controllerStateMachine.inState(ControllerState::CONFIGURE)) {
    configure();
  } else if (controllerStateMachine.inState(ControllerState::SEND_COMMAND)) {
    sendCommands();
  }

}

boolean infoSwitchesPressed() {
  FootswitchState fs1State = footswitches[FS_INFO_1]->checkClicked();
  FootswitchState fs2State = footswitches[FS_INFO_2]->checkClicked();

  if (fs1State == FootswitchState::PRESSED && fs2State == FootswitchState::PRESSED) {
    infoShouldBePrinted = true;
  }

  if (fs1State == FootswitchState::NONE && fs1State == FootswitchState::NONE && infoShouldBePrinted) {
    infoShouldBePrinted = false;
    printer.printConfigPage(config);
    return true;
  }

  return false;

}

boolean configSwitchesPressed() {
  FootswitchState fs1State = footswitches[FS_CONFIG_1]->checkClicked();
  FootswitchState fs2State = footswitches[FS_CONFIG_2]->checkClicked();

  if (fs1State == FootswitchState::PRESSED && fs2State == FootswitchState::PRESSED) {
    modeShouldChange = true;
  }

  if (fs1State == FootswitchState::NONE && modeShouldChange) {
    controllerStateMachine.toggleState();
    modeShouldChange = false;
    return true;
  }

  return false;
}

void configure() {
  ConfigurationState configurationState = configurationStateMachine.getState();
  
  // Select footswitch
  if (configurationState == ConfigurationState::SELECT_FOOTSWITCH) {
    for (Footswitch* fs : footswitches) {
      FootswitchState click = fs->checkClicked();
      if (click == FootswitchState::CLICK || click == FootswitchState::LONG_CLICK) {
        configurationStateMachine.setFootswitch(fs->getNumber(), click == FootswitchState::LONG_CLICK);
        configurationStateMachine.next();

        printer.configurationPrompt(configurationStateMachine.getState(), configurationStateMachine.getValue());
      }
    }

    return;
  } 
  
  // Fill values
  if (footswitches[FS_CONFIG_1]->checkClicked() == FootswitchState::CLICK && configurationState != ConfigurationState::EXIT) {
    configurationStateMachine.incrementValue();
    printer.configurationPrompt(
      configurationStateMachine.getState(), 
      configurationStateMachine.getValue(), 
      configurationStateMachine.getCommandType()
    );

  } else if (footswitches[FS_CONFIG_DECREMENT]->checkClicked() == FootswitchState::CLICK && configurationState != ConfigurationState::EXIT) {
    configurationStateMachine.decrementValue();
    printer.configurationPrompt(
      configurationStateMachine.getState(), 
      configurationStateMachine.getValue(), 
      configurationStateMachine.getCommandType()
    );

  } else if (footswitches[FS_CONFIG_2]->checkClicked() == FootswitchState::CLICK && configurationState != ConfigurationState::EXIT) {
    configurationStateMachine.next();

    ConfigurationState newState = configurationStateMachine.getState();
    
    if (newState == ConfigurationState::EXIT) {
      config->setButton(
        configurationStateMachine.getFootswitch(), 
        configurationStateMachine.getControllerButton(),
        configurationStateMachine.isLongClick()
      );

      configurationStateMachine.reset();
      printer.selectFootswitchPrompt();
    } else {
      printer.configurationPrompt(
        configurationStateMachine.getState(), 
        configurationStateMachine.getValue(),
        configurationStateMachine.getCommandType()
      );
    }
  }
}

void sendCommands() {
  for (Footswitch* fs : footswitches) {
    FootswitchState state = fs->checkClicked();

    if (state & FootswitchState::ANY) {
      int no = fs->getNumber();
      boolean longClick = state == FootswitchState::LONG_CLICK;
      ControllerButtonEntity btn = config->getButtonData(no, longClick);
      commandExecutor.executeCommand(no, longClick);

      int page = config->getPage();
      printer.commandInfo(no, page, btn, commandExecutor.getExecutedValue());
      return;
    }
  }
}

void changeMode() {
  if (controllerStateMachine.inState(ControllerState::CONFIGURE)) {
    printer.enterConfiguration();
    printer.selectFootswitchPrompt();
    configurationStateMachine.reset();
  } else if (controllerStateMachine.inState(ControllerState::SEND_COMMAND)) {
    printer.leaveConfiguration();
  }  
}