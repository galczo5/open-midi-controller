#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "controller-button.h"
#include "command-type.h"
#include "footswitch/click-type.h"

#define BUFFER_SIZE 300
#define PAGE_SIZE 48
#define BUTTON_SIZE 5

// 6 buttons x 2 actions x 5 pages * 5 bytes = 300

class OpenMidiControllerConfig {

private:
  const int MAX_PAGES = 5;

  byte storedData[BUFFER_SIZE];
  int page = 0;

public:
  OpenMidiControllerConfig(byte data[]);
  int getPage();
  
  ControllerButton getButtonData(int no, boolean longClick);
  void setButton(int no, ControllerButton button, boolean longClick);

  static ControllerButton createButton(byte channel, CommandType type, byte value1, byte value2, byte value3);
};

#endif