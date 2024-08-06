#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

class TestLCD_RTC {
public:
    void setup() {
        Serial.begin(115200);
		this->setupRTC();
        this->resetEEPROM(true);
    }

    void loop() {
        this->loopRTC();
    }

private:
	void setupRTC() {
		if (!_resetEEPROM) {
			Wire.begin();

			lcd->init();
			lcd->backlight();
			lcd->setCursor(0, 0);
			lcd->print(F("LCD I2C Ready!"));

			if (!rtc.begin()) {
				Serial.println(F("Couldn't find RTC"));
				while(1);
			}

			if (rtc.lostPower()) {
				Serial.println(F("RTC Lost power, setting the time!"));
				rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
			}

			rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		}
	}
	
	void loopRTC() {
		if (!_resetEEPROM) {
			unsigned long currentTime = millis();
			if ((unsigned long) (currentTime - lastTime) >= 1000) {
				lastTime = currentTime;
				
				DateTime now = rtc.now();

				// Time
				lcd->setCursor(0, 0);
				lcd->print(F("Time: "));
				lcd->print(now.hour());
				lcd->print(F(":"));
				lcd->print(now.minute());
				lcd->print(F(":"));
				lcd->print(now.second());

				// Date
				lcd->setCursor(0, 1);
				lcd->print(F("Date: "));
				lcd->print(now.day());
				lcd->print(F("/"));
				lcd->print(now.month());
				lcd->print("/");
				lcd->print(now.year());
			}
		}
	}
	
    void resetEEPROM(bool enable) {
		_resetEEPROM = enable;
		if (enable) 
			for (int i = 0; i < (int)(EEPROM.length()); i++) {
				EEPROM.write(i, 0xFF);
			}
    }
	
private:
    unsigned long lastTime = 0;
	bool _resetEEPROM = false;
    LiquidCrystal_I2C* lcd = new LiquidCrystal_I2C(0x27, 16, 2);
    RTC_DS3231 rtc;
};