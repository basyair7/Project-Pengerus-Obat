/*! @file relaycontroller.cpp
 * @version 1.0.0
*/

#include <Arduino.h>
#include <relaycontroller.h>

relaycontroller::relaycontroller(uint8_t pinRelay) {
    _pinRelay = pinRelay;
}

void relaycontroller::init() {
    pinMode(_pinRelay, OUTPUT);
}

void relaycontroller::write(uint8_t state, int _delay) {
    if ((unsigned long) (millis() - LastMillis1) >= (long unsigned int) _delay) {
        LastMillis1 = millis();

        digitalWrite(_pinRelay, state);
    }
}