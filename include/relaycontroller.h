/*! @file relaycontroller.h
 * @version 1.0.0
*/

#pragma once

#include <variable.h>

#define ON  0x0
#define OFF 0x1

class relaycontroller : protected VariableMillis {
public:
    relaycontroller(uint8_t pinRelay);
    void init();
    void write(uint8_t state, int _delay);

private:
    uint8_t _pinRelay;
};