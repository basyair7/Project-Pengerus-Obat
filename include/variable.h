#ifndef VARIABLE_H
#define VARIABLE_H

#include <Arduino.h>

class VariableMillis {
protected:
    unsigned long LastMillis_1 = 0;
    unsigned long LastMillis_2 = 0;
};

#endif