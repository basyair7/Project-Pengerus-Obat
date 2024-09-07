/*! @file motor.hpp
 * @version 1.0.0
*/

#pragma once

#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <Arduino.h>
#include <DSXLite.h>

class DriverMotor {
public:
    ~DriverMotor();                                                    // destructor class
    void init(uint8_t pinA, uint8_t pinB, uint8_t pinC, uint8_t pinD); // initializing the driver
    void setSpeedPWM(int speed);                                       // set PWM speed
    void run_forward(unsigned long _delay);                            // advancing the motor
    void run_backward(unsigned long _delay);                           // retracting the motor
    void stops();                                                      // stopping the motor

private:
    // Pointer for motor control
    DSX_pinMotors* driver;
};

#endif
