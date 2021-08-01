#ifndef FOOTSWITH_H
#define FOOTSWITH_H

#include <Arduino.h>
#include "footswitch-state.h"

class Footswitch {
	private: 
		int pin;
		int no;

		int lastState;
		boolean doubleClick;

		unsigned long clickTime;
		unsigned long repeatTime;

		unsigned long lastStateChange;

		boolean wasPressed;
		FootswitchState click;
		
	public: 
		Footswitch(int no, int pin);
		void init();
		void scan();
		
		FootswitchState checkClicked();
		int getNumber();		
};

#endif
