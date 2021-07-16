#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "controller-button-entity.h"
#include "command-type.h"

#define PAGE_NO 5
#define BUFFER_SIZE 300
#define PAGE_SIZE 48
#define BUTTON_SIZE 5

// 6 buttons x 2 actions x 5 pages * 5 bytes = 300

class MidiControllerConfig {

private:
  const int MAX_PAGES = 5;

  byte storedData[BUFFER_SIZE];
  int page = 0;

public:
  MidiControllerConfig();

  int getPage();
  void setPage(int page);
  
  ControllerButtonEntity getButtonData(int no, boolean longClick);
  void setButton(int no, ControllerButtonEntity button, boolean longClick);

  static ControllerButtonEntity createButton(byte channel, CommandType type, byte value1, byte value2, byte value3);
};

#endif