#ifndef CONTROLLER_BUTTON_H
#define CONTROLLER_BUTTON_H

#include <Arduino.h>

struct ControllerButtonEntity {
  byte channel;
  byte type;
  byte value1;
  byte value2;
  byte value3;
};

#endif