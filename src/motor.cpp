/*! @file motor.cpp
 * @version 1.0.0
*/

#include <motor.hpp>

// initializing the driverMotor program
void DriverMotor::init(uint8_t pinA, uint8_t pinB, uint8_t pinC, uint8_t pinD) {
    driver = new DSX_pinMotors(pinA, pinB, pinC, pinD); // Creation of DSX_pinMotors object
}

// setSpeedPWM program
void DriverMotor::setSpeedPWM(int speed) {
    driver->speedPWM(speed);
}

// Advancing the motor program
void DriverMotor::run_forward(unsigned long _delay) {
    if (_delay == 0) {
        driver->forward(); // forward
    } else {
        driver->forward(); // forward
        delay(_delay); // wait for specified delay time
        driver->stops(); // stops
        delay(_delay); // wait for specified delay time
    }
}

// Retracting the motor prgram
void DriverMotor::run_backward(unsigned long _delay) {
    if (_delay == 0) {
        driver->backward(); // backward
    } else {
        driver->backward(); // backward
        delay(_delay); // wait for specified delay time
        driver->stops(); // stops
        delay(_delay); // wait for specified delay time
    }
}

// Stops the motor program
void DriverMotor::stops() {
    driver->stops(); // stops the motor
}

// destructor
DriverMotor::~DriverMotor() {
    delete driver; // delete DSX_pinMotors object
}
