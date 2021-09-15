#include <Arduino.h>
#include <EEPROM.h>
#include "midi-controller-config.h"
#include "footswitch/footswitch-state.h"

MidiControllerConfig::MidiControllerConfig() {
  this->page = 0;
  for (int i = 0; i < BUFFER_SIZE; i++) {
    this->storedData[i] = EEPROM.read(i);
  }
}

int MidiControllerConfig::getPage() {
  return this->page;
}

ControllerButtonEntity MidiControllerConfig::getButtonData(int no, FootswitchState click) {
  int mode = 0;

  if (click & FootswitchState::LONG_CLICK) {
    mode = LONG_CLICK_BUFFER_START;
  } else if (click & FootswitchState::DOUBLE_CLICK) {
    mode = 2 * LONG_CLICK_BUFFER_START;
  }

  int index = ((PAGE_SIZE * this->page) + (no * BUTTON_SIZE)) + mode;

  ControllerButtonEntity button = {
    this->storedData[index],
    this->storedData[index + 1],
    this->storedData[index + 2],
    this->storedData[index + 3],
    this->storedData[index + 4],
  };

  return button;
}

void MidiControllerConfig::setButton(int no, ControllerButtonEntity button, FootswitchState click) {
  int mode = 0;

  if (click == FootswitchState::LONG_CLICK) {
    mode = LONG_CLICK_BUFFER_START;
  } else if (click == FootswitchState::DOUBLE_CLICK) {
    mode = 2 * LONG_CLICK_BUFFER_START;
  }
  
  int index = ((PAGE_SIZE * this->page) + (no * BUTTON_SIZE)) + mode;
  
  this->storedData[index] = button.channel;
  this->storedData[index + 1] = button.type;
  this->storedData[index + 2] = button.value1;
  this->storedData[index + 3] = button.value2;
  this->storedData[index + 4] = button.value3;

  EEPROM.write(index, button.channel);
  EEPROM.write(index + 1, button.type);
  EEPROM.write(index + 2, button.value1);
  EEPROM.write(index + 3, button.value2);
  EEPROM.write(index + 4, button.value3);
}

void MidiControllerConfig::setPage(int page) {
  this->page = page % PAGE_NO;

  if (this->page < 0) {
    this->page = PAGE_NO - 1;
  }
}


boolean MidiControllerConfig::isInUsbMidiMode() {
  return EEPROM.read(USB_MODE_EEPROM_ADDR);
}

void MidiControllerConfig::setUsbMidiMode(boolean enabled) {
  EEPROM.write(USB_MODE_EEPROM_ADDR, enabled ? 1 : 0);
}

void MidiControllerConfig::sendConfiguration() {
  Serial.write(this->storedData, BUFFER_SIZE);
}
