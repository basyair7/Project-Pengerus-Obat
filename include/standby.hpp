#ifndef STANDBY_HPP
#include <Arduino.h>
#include <Keypad.h>
#include "main.h"
#include "menu.hpp"
#include "variable.h"
#include "TypeSerial"

class StandByProgram : protected VariableMillis {
public:
    void run() {
        int i = 0;
        while (true) {
            char key = _key.getKey();
            if ((unsigned long) (millis() - LastMillis1) >= (long unsigned int)1000) {
                LastMillis1 = millis();
                TSprintln("Date: " + rtc.datestr());
                TSprintln("Time: " + rtc.timestr());
                lcd->clear();
                lcd->print("Date: " + rtc.datestr(), 0, 0);
                lcd->print("Time: " + rtc.timestr(), 0 ,1);

                if (i >= 10) lcd->backlight(false);

                i++;
            }

            if (key) {
                if (key == 'B') {
                    lcd->backlight(true);
                    break;
                }
                
                lcd->backlight(true);
                i = 0;
            }
        }

        _menu.menu();
    }

private:
    Menu _menu;
    Keypad _key = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
};

#endif