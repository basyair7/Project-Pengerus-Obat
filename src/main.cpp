#include <main.h>
#include <variable.h>
#include <TypeSerial>
#include <LCDdisplay>
#include <DS3231rtc>
#include <Arduino_FreeRTOS.h>
#include <task.h>

class ProgramMain {
public:
    void program1(void *pvParameter) {
        (void) pvParameter;
        LCDdisplay* lcd = static_cast<LCDdisplay*>(pvParameter);
        DS3231rtc* rtc = static_cast<DS3231rtc*>(pvParameter);
        while (true) {
            lcd->print("Date:" + rtc->datestr(), 0, 0);
            lcd->print("Time:" + rtc->timestr(), 1, 0);
            vTaskDelay(pdMS_TO_TICKS(1000));
            lcd->clear();
        }
    }

    void program2(void *pvParameter) {
        (void) pvParameter;
        pinMode(LED_BUILTIN, OUTPUT);

        while(true) {
            digitalWrite(LED_BUILTIN, HIGH);
            vTaskDelay(pdMS_TO_TICKS(500));
            digitalWrite(LED_BUILTIN, LOW);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
};

class ThisRTOS : public ProgramMain {
public:
    LCDdisplay lcd;
    DS3231rtc rtc;

    static ThisRTOS& instance() {
        static ThisRTOS instance;
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
        rtc.begin(nullptr);
        rtc.autoAdjust(true);
        lcd.init();

        ThisRTOS* p = new ThisRTOS;
        xTaskCreate([](void* param) {
            static_cast<ThisRTOS*>(param)->program1(param);
        }, "program1", 1500, p, 1, NULL);
        xTaskCreate([](void* param) {
            static_cast<ThisRTOS*>(param)->program2(param);
        }, "program2", 1500, p, 1, NULL);

        vTaskStartScheduler();
    }

    void _loop() {
        // TODO("Not yet implemented");
    }
};

void setup() { ThisRTOS::setup(); }
void loop() { ThisRTOS::loop(); }