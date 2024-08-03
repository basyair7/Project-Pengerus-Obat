#ifndef VARIABLE_H
#define VARIABLE_H

#include <Arduino.h>

#define pinCS 53

#define PIN_MOTOR_A 24
#define PIN_MOTOR_B 25
#define PIN_MOTOR_C 26
#define PIN_MOTOR_D 27

static const byte ROWS = 4;
static const byte COLS = 4;
inline char keys[ROWS][COLS] = {
    {'1', '4', '7', '*'},
    {'2', '5', '8', '0'},
    {'3', '6', '9', '#'},
    {'A', 'B', 'C', 'D'}
};

inline byte rowPins[ROWS] = {9, 8, 7, 6};
inline byte colPins[COLS] = {5, 4, 3, 2};

class VariableMillis {
public:
    unsigned long LastMillis1;
    unsigned long LastMillis2;
};

#endif