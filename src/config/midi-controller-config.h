#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "controller-button-entity.h"
#include "footswitch/footswitch-state.h"
#include "command-type.h"

#define BUTTON_NO 6
#define PAGE_NO 3
#define BUTTON_SIZE 5
#define ACTIONS_NO 3

#define PAGE_SIZE BUTTON_NO * BUTTON_SIZE * ACTIONS_NO
#define LONG_CLICK_BUFFER_START BUTTON_NO * PAGE_NO * BUTTON_SIZE
#define BUFFER_SIZE PAGE_SIZE * PAGE_NO

#define USB_MODE_EEPROM_ADDR 800

class MidiControllerConfig {

private:
  const int MAX_PAGES = PAGE_NO;

  byte storedData[BUFFER_SIZE];
  int page;

public:
  MidiControllerConfig();

  int getPage();
  void setPage(int page);
  
  ControllerButtonEntity getButtonData(int no, FootswitchState click);
  void setButton(int no, ControllerButtonEntity button, FootswitchState click);
  

  bool isInUsbMidiMode();
  void setUsbMidiMode(boolean enabled);
  void sendConfiguration();
};

#endif