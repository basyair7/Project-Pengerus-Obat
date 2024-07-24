#include <Arduino.h>
#include "keypad.cpp"
#include "ds3231rtc.cpp"

KeyTest keytest;
TestLCD_RTC lcd_rtc;

void setup() {
    // keytest.setup();
    lcd_rtc.setup();
}

void loop() {
    // keytest.loop();
    lcd_rtc.loop();
}