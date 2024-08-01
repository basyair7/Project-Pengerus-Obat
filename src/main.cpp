#include <main.h>
#include <variable.h>
#include <TypeSerial>
#include <LCDdisplay>
#include <DS3231rtc>

LCDdisplay* lcd = new LCDdisplay;
DS3231rtc* rtc = new DS3231rtc;

class ProgramMain {
public:
    static void setup() {
        TSbegin(115200);
        rtc->begin(nullptr);
        rtc->autoAdjust(true);
        lcd->init();
    }

    static void loop() {
        lcd->print("Date:" + rtc->datestr(), 0, 0);
        lcd->print("Time:" + rtc->timestr(), 1, 0);
        delay(1000);
        lcd->clear();
    }
};

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/)()) { return 0; }

// Weak empty variant initialization function.
void initVariant() __attribute__((weak));
void initVariant() { }

void setupUSB() __attribute__((weak));
void setupUSB() { }

int main(void) {
    init();
    initVariant();

#if defined(USBCON)
    USBDevice.attach();
#endif

    ProgramMain __main__;

    __main__.setup();

    while(1) {
        __main__.loop();
        if (serialEventRun) serialEventRun();
    }

    return 0;
}