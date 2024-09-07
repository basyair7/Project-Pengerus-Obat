/*! @file main.cpp
 * @version 1.0.0
*/

#include <main.h>
#include <variable.h>
#include <TypeSerial>
#include <LCDdisplay>
#include <DS3231rtc>
#include <SDCard>

#include <info>
#include <menu.hpp>
#include <standby.hpp>
#include <handleMotors>
#include <relaycontroller.h>

// Initialize the global object
LCDdisplay* lcd = new LCDdisplay;
DS3231rtc rtc;
SDCard sdcard;

StandByProgram standby;
HandleMotors handleMotors;
relaycontroller relay(PIN_RELAY);
Menu menu;
Info info;
byte num_menu;

// Define class ProgramMain
class ProgramMain {
public:
    // Get an instance of class ProgramMain
    static ProgramMain& instance() {
        static ProgramMain instance;
        return instance;
    }

    static void setup(void) {
        instance()._setup();
    }

    static void loop(void) {
        instance()._loop();
    }

private:
    void _setup() {
        // put your setup code here, to run once:
        TSbegin(115200);  // Initialize Serial Baudrate
        rtc.begin();      // Initialize rtc
        relay.init();     // Initialize relay
        // rtc.autoAdjust(); // RTC time adjustment
        lcd->init();      // Initialize lcd
        sdcard.SDCardInit(); // Initialize sdcard
        menu.menu();      // run the menu program
    }

    void _loop() {
        // put your main code here, to run repeatedly:
        if (num_menu == 1) {
            handleMotors.run();  // run the handleMotors program
        }
        if (num_menu == 2) {
            info.run();  // run the info program
        }
        
        if (num_menu == 3) {
            standby.run(); // run the standby program
        }

        
        menu.menu(); // run the menu program
        menu.switchToMenu(); // return to the menu program (if user presses the back button)
    }
};

void setup() { /*Arduinoのループ関数*/ ProgramMain::setup(); }
void loop() { /*Arduinoのセットアップ関数*/ ProgramMain::loop(); }
