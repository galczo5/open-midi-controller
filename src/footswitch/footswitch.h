#ifndef FOOTSWITH_H
#define FOOTSWITH_H

#include <Arduino.h>
#include "click-type.h"

class Footswitch {
	private: 
		int pin;
		int no;
		unsigned long timeClicked;
		boolean buttonDown;
		ClickType click;
		
	public: 
		Footswitch(int no, int pin);
		void init();
		void scan();
		
		ClickType checkClicked();
		int getNumber();		
};

#endif
