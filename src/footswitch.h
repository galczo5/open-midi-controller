#ifndef FOOTSWITH_H
#define FOOTSWITH_H

#include <Arduino.h>
#include "click-type.h"

class Footswitch {
	private: 
		int pin;
		int timeClicked;
		int lastState;
		int no;
		
	public: 
		Footswitch(int no, int pin);
		void init();
		ClickType checkClicked();
		int getNumber();
		
};

#endif
