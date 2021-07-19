#ifndef FOOTSWITH_H
#define FOOTSWITH_H

#include <Arduino.h>
#include "footswitch-state.h"

class Footswitch {
	private: 
		int pin;
		int no;
		unsigned long timeClicked;
		unsigned long timeRepeated;
		boolean buttonDown;
		FootswitchState click;
		
	public: 
		Footswitch(int no, int pin);
		void init();
		void scan();
		
		FootswitchState checkClicked();
		int getNumber();		
};

#endif
