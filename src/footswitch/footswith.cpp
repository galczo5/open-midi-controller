#include <Arduino.h>
#include "footswitch.h"
#include "footswitch-state.h"

#define LONG_CLICK_TIME 1000
#define CLICK_TIME 250
#define DEBOUNCE_TIME 5
#define TIME_TO_REPEAT 3000
#define REPEAT_TIMEOUT 100

Footswitch::Footswitch(int no, int pin) {
    this->pin = pin;
    this->no = no;
    this->wasPressed = false;
    this->lastStateChange = 0;
    this->clickTime = 0;
    this->doubleClick = false;
    this->lastState = HIGH;
}

void Footswitch::init() {
    pinMode(this->pin, INPUT_PULLUP);
}

FootswitchState Footswitch::checkClicked() {    
    return this->click;
}

int Footswitch::getNumber() {
    return this->no;
}

void Footswitch::scan() {
    int state = digitalRead(this->pin);
    unsigned long now = millis();

    // DEBOUNCE - DO NOT CHANGE STATE
    if (state != this->lastState && now - this->lastStateChange < DEBOUNCE_TIME) {
        return;
    }

    if (state != this->lastState) {
        this->lastStateChange = now;
    }

    // BUTTON DOWN
    if (state == LOW) {

        // SECOND CLICK
        if (this->wasPressed && this->lastState != LOW) {
            this->doubleClick = true;
        }

        // FIRST CLICK
        if (!wasPressed && this->lastState != LOW) {
            this->clickTime = now;
            this->repeatTime = now;
            this->wasPressed = true;
        }

        // REPEAT ON PRESS
        if (now - this->clickTime > TIME_TO_REPEAT && now - this->repeatTime > REPEAT_TIMEOUT) {
            this->click = FootswitchState::CLICK;
            this->repeatTime = now;
            this->lastState = LOW;
            return;
        }

        this->click = FootswitchState::PRESSED;
        this->lastState = LOW;
        return;
    }

    // BUTTON UP

    unsigned long timeFromClick = now - this->clickTime; 
    if (this->wasPressed && timeFromClick > LONG_CLICK_TIME && !this->doubleClick) {
        this->click = FootswitchState::LONG_CLICK;
        this->lastState = HIGH;
        this->wasPressed = false;
        return;
    }

    if (this->wasPressed && timeFromClick > CLICK_TIME) {
        if (this->doubleClick) {
            this->click = FootswitchState::DOUBLE_CLICK;
        } else {
            this->click = FootswitchState::CLICK;
        }
        
        this->lastState = HIGH;
        this->wasPressed = false;
        this->doubleClick = false;
        return;
    }

    // WAIT FOR DOUBLE CLICK
    if (this->wasPressed && timeFromClick <= CLICK_TIME) {
        this->lastState = state;
        return;
    }
    
    this->click = FootswitchState::NONE;
}