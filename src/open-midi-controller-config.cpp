#include <Arduino.h>
#include "open-midi-controller-config.h"


// 6 buttons x 2 actions x 5 pages * 4 bytes = 240

OpenMidiControllerConfig::OpenMidiControllerConfig(byte data[]) {
  for (int i = 0; i < BUFFER_SIZE; i++) {
    this->storedData[i] = data[i];
  }
}

int OpenMidiControllerConfig::getPage() {
  return this->page;
}

ControllerButton OpenMidiControllerConfig::getButtonData(int no) {
  int index = (PAGE_SIZE * this->page) + (no * BUTTON_SIZE);

  ControllerButton button;
  button.channel = this->storedData[index];
  button.type = this->storedData[index + 1];
  button.value1 = this->storedData[index + 2];
  button.value2 = this->storedData[index + 3];

  return button;
}

void OpenMidiControllerConfig::setButton(int no, ControllerButton button) {
  int index = (PAGE_SIZE * this->page) + (no * BUTTON_SIZE);
  
  this->storedData[index] = button.channel;
  this->storedData[index + 1] = button.type;
  this->storedData[index + 2] = button.value1;
  this->storedData[index + 3] = button.value2;
}

ControllerButton OpenMidiControllerConfig::createButton(byte channel, CommandType type, byte value1, byte value2) {
  ControllerButton button;

  button.channel = channel;
  button.type = type;
  button.value1 = value1;
  button.value2 = value2;

  return button;
}