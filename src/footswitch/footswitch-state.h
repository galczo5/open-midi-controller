#ifndef CLICK_TYPE_H
#define CLICK_TYPE_H

enum FootswitchState {
	NONE = 0,
	PRESSED = 1,
	CLICK = 2,
	LONG_CLICK = 4,
	ANY = CLICK | LONG_CLICK
};

#endif
