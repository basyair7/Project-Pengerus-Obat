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

LCDdisplay* lcd = new LCDdisplay;
DS3231rtc rtc;
SDCard* sdcard = new SDCard;

StandByProgram standby;
HandleMotors handleMotors;
Menu menu;
Info info;
byte num_menu;

class ProgramMain {
public:
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
        TSbegin(115200);
        rtc.begin();
        // rtc.autoAdjust();
        lcd->init();
        // sdcard->SDCardInit();
        menu.menu();
    }

    void _loop() {
        if (num_menu == 1) {
            handleMotors.run();
            menu.menu();
        }
        if (num_menu == 2) {
            info.run();
            menu.menu();
        }
        
        if (num_menu == 3) {
            standby.run();
        }

        menu.switchToMenu();
    }
};

void setup() { ProgramMain::setup(); }
void loop() { ProgramMain::loop(); }