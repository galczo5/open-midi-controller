#include <Arduino.h>
#include "footswitch.h"
#include "click-type.h"

Footswitch::Footswitch(int no, int pin) {
    this->pin = pin;
    this->no = no;
}

void Footswitch::init() {
    this->timeClicked = 0;
    this->lastState = -1;

    pinMode(this->pin, INPUT_PULLUP);
}

ClickType Footswitch::checkClicked() {
    ClickType result = ClickType::NONE;
    
    int state = digitalRead(this->pin);

    // wait for first state change 
    if (this->lastState == -1) {
        this->timeClicked = millis();
    }

    boolean stateNotChanged = state == this->lastState;

    if (stateNotChanged && state == HIGH) {
        return ClickType::NONE;
    } else if (stateNotChanged && state == LOW) {
        return ClickType::PRESSED;
    }

    if (state == LOW) {
        timeClicked = millis();
        result = ClickType::PRESSED;
    } else {
        int timeFromClickStart = millis() - timeClicked;

        if (timeFromClickStart > ClickType::LONG) {
            result = ClickType::LONG;
        } else if (timeFromClickStart > ClickType::NORMAL) {
            result = ClickType::NORMAL;
        }
    }

    this->lastState = state;

    return result;
}

int Footswitch::getNumber() {
    return this->no;
}
