#include <Arduino.h>
#include "keypad.cpp"
#include "motortest.cpp"
#include "ds3231rtc.cpp"

KeyTest* keytest = new KeyTest();
TestLCD_RTC* lcd_rtc = new TestLCD_RTC();
MotorTest* motortest = new MotorTest();

void setup() {
    keytest->setup();
    // lcd_rtc->setup();
    // motortest->setup();
}

void loop() {
    keytest->loop();
    // lcd_rtc->loop();
    // motortest->loop();
}