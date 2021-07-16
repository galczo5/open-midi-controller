#include <LiquidCrystal_I2C.h>
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

// INIT FOOTSWITCHES
Footswitch fs1(0, FS_1_PIN);
Footswitch fs2(1, FS_2_PIN);
Footswitch fs3(2, FS_3_PIN);
Footswitch fs4(3, FS_4_PIN);
Footswitch fs5(4, FS_5_PIN);
Footswitch fs6(5, FS_6_PIN);

Footswitch* footswitches[6] = { &fs1, &fs2, &fs3, &fs4, &fs5, &fs6 };

boolean modeShouldChange = false;

LiquidCrystal_I2C lcd(0x27, 20, 4);
Printer printer;

MidiControllerConfig config;
ControllerStateMachine controllerStateMachine(ControllerState::SEND_COMMAND);
ConfigurationStateMachine configurationStateMachine;
CommandExecutor commandExecutor(&config);


void setup() {
  Serial.begin(9600);

  commandExecutor.init();

  lcd.init();
  lcd.backlight();

  printer.welcome(lcd, 1);

  for (Footswitch* fs : footswitches) { 
    fs->init(); 
  }
}

void loop() {

  for (Footswitch* fs : footswitches) {
    fs->scan();
  }
  
  if (checkForModeChanges()) {
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

boolean checkForModeChanges() {
  FootswitchState fs1State = footswitches[FS_CONFIG_1]->checkClicked();
  FootswitchState fs2State = footswitches[FS_CONFIG_2]->checkClicked();

  if (fs1State == FootswitchState::PRESSED && fs2State == FootswitchState::PRESSED) {
    modeShouldChange = true;
  }

  if (fs1State == FootswitchState::NONE && fs1State == FootswitchState::NONE && modeShouldChange) {
    if (controllerStateMachine.inState(ControllerState::SEND_COMMAND)) {
      controllerStateMachine.enterState(ControllerState::CONFIGURE);
    } else if (controllerStateMachine.inState(ControllerState::CONFIGURE)) {
      controllerStateMachine.enterState(ControllerState::SEND_COMMAND);
    }

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

        printer.configurationPrompt(lcd, configurationStateMachine.getState(), configurationStateMachine.getValue());
      }
    }

    return;
  } 
  
  // Fill values
  if (footswitches[FS_CONFIG_1]->checkClicked() == FootswitchState::CLICK && configurationState != ConfigurationState::EXIT) {
    configurationStateMachine.incrementValue();
    printer.configurationPrompt(lcd, configurationStateMachine.getState(), configurationStateMachine.getValue());
  } else if (footswitches[FS_CONFIG_2]->checkClicked() == FootswitchState::CLICK && configurationState != ConfigurationState::EXIT) {
    configurationStateMachine.next();

    ConfigurationState newState = configurationStateMachine.getState();
    
    if (newState == ConfigurationState::EXIT) {
      printer.selectFootswitchPrompt(lcd);
      config.setButton(configurationStateMachine.getFootswitch(), configurationStateMachine.getControllerButton(), configurationStateMachine.isLongClick());
      configurationStateMachine.reset();
    } else {
      printer.configurationPrompt(lcd, configurationStateMachine.getState(), configurationStateMachine.getValue());
    }
  }
}

void sendCommands() {
  for (Footswitch* fs : footswitches) {
    FootswitchState state = fs->checkClicked();
    int no = fs->getNumber();

    if (state & FootswitchState::ANY) {
      ControllerButtonEntity btn = config.getButtonData(no, state == FootswitchState::LONG_CLICK);
      commandExecutor.executeCommand(no, state == FootswitchState::LONG_CLICK);
      printer.commandInfo(lcd, no, &btn);
    }
  }
}

void changeMode() {
  if (controllerStateMachine.inState(ControllerState::CONFIGURE)) {
    printer.enterConfiguration(lcd);
    printer.selectFootswitchPrompt(lcd);
    configurationStateMachine.reset();
  } else if (controllerStateMachine.inState(ControllerState::SEND_COMMAND)) {
    printer.leaveConfiguration(lcd);
  }  
}