#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "controller-button.h"
#include "command-type.h"

#define BUFFER_SIZE 240
#define PAGE_SIZE 48
#define BUTTON_SIZE 4

class OpenMidiControllerConfig {

private:
  const int MAX_PAGES = 5;

  byte storedData[BUFFER_SIZE];
  int page = 0;

public:
  OpenMidiControllerConfig(byte data[]);
  int getPage();
  ControllerButton getButtonData(int no);
  void setButton(int no, ControllerButton button);

  static ControllerButton createButton(byte channel, CommandType type, byte value1, byte value2);
};

#endif