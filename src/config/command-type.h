#ifndef COMMAND_TYPE_H
#define COMMAND_TYPE_H

enum CommandType {
  UNSET = 0,
  NOTE = 1,
  CC = 2,
  TOGGLE_CC = 3,
  NEXT_PAGE = 4,
  PREV_PAGE = 5,
  PAGE = 6,
  TEMP_PAGE = 7
};

#define NUMBER_OF_COMMAND_TYPES 8

#endif
