#include <LiquidCrystal_I2C.h>
#include "open-midi-controller-config.h"
#include "footswitch.h"
#include "command-type.h"

/**
 * Open Midi Controller
 * 
 * Footswitch pin map:
 *  __________________________
 * |                          |
 * | fs1 D2   fs3 D4   fs5 D6 |
 * |                          |
 * |                          |
 * | fs2 D3   fs4 D5   fs6 D7 |
 * |__________________________|
 */

#define FS_1_PIN 2 
#define FS_2_PIN 3
#define FS_3_PIN 4
#define FS_4_PIN 5
#define FS_5_PIN 6
#define FS_6_PIN 7

#define ENTER_CONFIG_FS_1 3
#define ENTER_CONFIG_FS_2 5


// init global objects
LiquidCrystal_I2C lcd(0x27, 20, 4);
OpenMidiControllerConfig config({});

Footswitch fs1(1, FS_1_PIN);
Footswitch fs2(2, FS_2_PIN);
Footswitch fs3(3, FS_3_PIN);
Footswitch fs4(4, FS_4_PIN);
Footswitch fs5(5, FS_5_PIN);
Footswitch fs6(6, FS_6_PIN);

Footswitch switches[6] = { fs1, fs2, fs3, fs4, fs5, fs6 };

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  for (Footswitch fs : switches) {
    fs.init();
  }

  printWelcomeMessage();
}

void loop() {

}

void printWelcomeMessage() {

  lcd.setCursor(0, 0);
  lcd.print("WELCOME");

  lcd.setCursor(0, 1);
  lcd.print("MIDI CONTROLLER");

  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("INITIALIZING");
  
  lcd.setCursor(0, 1);
  lcd.print("VERSION 0.0.1");

  delay(2000);

  lcd.clear();
}