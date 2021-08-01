#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "controller-button-entity.h"
#include "footswitch/footswitch-state.h"
#include "command-type.h"

#define BUTTON_NO 6
#define PAGE_NO 3
#define PAGE_SIZE 48
#define BUTTON_SIZE 5
#define LONG_CLICK_BUFFER_START BUTTON_NO * PAGE_NO * BUTTON_SIZE
#define ACTIONS_NO 3
#define BUFFER_SIZE BUTTON_NO * BUTTON_SIZE * ACTIONS_NO * PAGE_NO

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
};

#endif