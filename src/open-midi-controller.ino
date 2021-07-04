#include <LiquidCrystal_I2C.h>
#include "config/open-midi-controller-config.h"
#include "footswitch/footswitch.h"
#include "config/command-type.h"
#include "controller/controller.h"
#include "printer/printer.h"
#include "configuration/configuration.h"

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

LiquidCrystal_I2C lcd(0x27, 20, 4);
Printer printer;

OpenMidiControllerConfig config({});
Controller ctrl(ControllerState::SEND_COMMAND);
Configuration configuration;

Footswitch fs1(0, FS_1_PIN);
Footswitch fs2(1, FS_2_PIN);
Footswitch fs3(2, FS_3_PIN);
Footswitch fs4(3, FS_4_PIN);
Footswitch fs5(4, FS_5_PIN);
Footswitch fs6(5, FS_6_PIN);

Footswitch* switches[6] = { &fs1, &fs2, &fs3, &fs4, &fs5, &fs6 };

boolean modeShoudlChange = false;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  printer.welcome(lcd, 1);

  for (Footswitch* fs : switches) { 
    fs->init(); 
  }
}

void loop() {

  for (Footswitch* fs : switches) {
    fs->scan();
  }
  
  if (checkForModeChanges()) {
    return;
  }

  if (ctrl.checkChanges()) {
    changeMode();
    return;
  }
  
  if (ctrl.inState(ControllerState::CONFIGURE)) {
    configure();
  } else if (ctrl.inState(ControllerState::SEND_COMMAND)) {
    sendCommands();
  }

}

boolean checkForModeChanges() {
  ClickType configurationSwitch1Click = switches[FS_CONFIG_1]->checkClicked();
  ClickType configurationSwitch2Click = switches[FS_CONFIG_2]->checkClicked();

  if (configurationSwitch1Click == ClickType::PRESSED && configurationSwitch2Click == ClickType::PRESSED) {
    modeShoudlChange = true;
  }

  if (configurationSwitch1Click == ClickType::NONE && configurationSwitch1Click == ClickType::NONE && modeShoudlChange) {
    if (ctrl.inState(ControllerState::SEND_COMMAND)) {
      ctrl.enterState(ControllerState::CONFIGURE);
    } else if (ctrl.inState(ControllerState::CONFIGURE)) {
      ctrl.enterState(ControllerState::SEND_COMMAND);
    }

    modeShoudlChange = false;
    return true;
  }

  return false;
}

void configure() {
  ConfigurationState configurationState = configuration.getState();
  
  if (configurationState == ConfigurationState::SELECT_FOOTSWITCH) {
    for (Footswitch* fs : switches) {
      ClickType click = fs->checkClicked();
      if (click == ClickType::NORMAL || click == ClickType::LONG) {
        configuration.setFootswitch(fs->getNumber(), click == ClickType::LONG);
        configuration.next();
        printer.configurationPrompt(lcd, configuration.getState(), configuration.getValue());
      }
    }
  } else if (switches[FS_CONFIG_1]->checkClicked() == ClickType::NORMAL && configurationState != ConfigurationState::EXIT) {
    configuration.incrementValue();
    printer.configurationPrompt(lcd, configuration.getState(), configuration.getValue());
  } else if (switches[FS_CONFIG_2]->checkClicked() == ClickType::NORMAL && configurationState != ConfigurationState::EXIT) {
    configuration.next();

    ConfigurationState newState = configuration.getState();
    if (newState == ConfigurationState::EXIT) {
      printer.selectFootswitchPrompt(lcd);
      config.setButton(configuration.getFootswitch(), configuration.getControllerButton(), configuration.isLongClick());
      configuration.reset();
    } else {
      printer.configurationPrompt(lcd, configuration.getState(), configuration.getValue());
    }
  }
}

void sendCommands() {
  for (Footswitch* fs : switches) {
    ClickType click = fs->checkClicked();
    int no = fs->getNumber();

    if (click == ClickType::NORMAL || click == ClickType::LONG) {
      ControllerButton btn = config.getButtonData(no, click == ClickType::LONG);
      printer.commandInfo(lcd, no, &btn);
    }
  }
}

void changeMode() {
  if (ctrl.inState(ControllerState::CONFIGURE)) {
    printer.enterConfiguration(lcd);
    printer.selectFootswitchPrompt(lcd);
    configuration.reset();
  } else if (ctrl.inState(ControllerState::SEND_COMMAND)) {
    printer.leaveConfiguration(lcd);
  }  
}