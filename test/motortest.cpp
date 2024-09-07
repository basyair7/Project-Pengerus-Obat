#include <Arduino.h>
#include <DSXLite.h>
#include "variable.h"

class MotorTest {
public:
    void setup() {
        Serial.begin(115200);
    }

    void loop() {
        for (int i = 0; i < 255; i++) {
            Serial.println(i);
            driver.speedPWM(i);
            driver.forward();
            delay(100);
        }

        for (int i = 255; i > 0; i--) {
            Serial.println(i);
            driver.speedPWM(i);
            driver.forward();
            delay(100);
        }
    }

private:
    DSX_pinMotors driver = DSX_pinMotors(11, 10, 13, 12);
};