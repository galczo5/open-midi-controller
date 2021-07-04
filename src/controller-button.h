#ifndef CONTROLLER_BUTTON_H
#define CONTROLLER_BUTTON_H

#include <Arduino.h>

struct ControllerButton {
  byte channel;
  byte type;
  byte value1;
  byte value2;
};

#endif