#include <Arduino.h>
#include "footswitch.h"
#include "click-type.h"

#define LONG_CLICK_TIME 1000
#define DEBOUNCE_TIME 5

Footswitch::Footswitch(int no, int pin) {
    this->pin = pin;
    this->no = no;
    this->buttonDown = false;
}

void Footswitch::init() {
    pinMode(this->pin, INPUT_PULLUP);
}

ClickType Footswitch::checkClicked() {    
    return this->click;
}

int Footswitch::getNumber() {
    return this->no;
}

void Footswitch::scan() {
    int state = digitalRead(this->pin);
    ClickType result = ClickType::NONE;

    if (state == LOW) {
        if (!this->buttonDown) {
            this->timeClicked = millis();
            this->buttonDown = true;
        }

        result = ClickType::PRESSED;

    } else if (state == HIGH && this->buttonDown) {
        this->buttonDown = false;
        unsigned long timeFromClickStart = millis() - timeClicked;

        if (timeFromClickStart >= LONG_CLICK_TIME) {
            result = ClickType::LONG;
        } else if (timeFromClickStart >= DEBOUNCE_TIME) {
            result = ClickType::NORMAL;
        }
    }

    this->click = result;
}