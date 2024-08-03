#include <SDCard>
#include <main.h>
#include <variable.h>
#include <LiquidCrystal_animated.h>

void SDCard::addConfig(const String& configName, int id, int minute, int speed)
{
    String _readConfig = this->_read(fileConfig), _newConfig;
    StaticJsonDocument<500> doc;
    
    JsonObject _any = doc.createNestedObject(configName);
    _any["id"] = id;
    _any["minute"] = minute;
    _any["speed"] = speed;

    serializeJson(_any, _newConfig);
    this->_write(fileConfig, _newConfig);
}

void SDCard::readConfig(const String& configName, int* id, int* minute, int* speed) {
    String _readConfig = this->_read(fileConfig);
    StaticJsonDocument<500> doc;
    if (_readConfig == "null") {
        lcd->print("Information", 0, 0);
        lcd->print("No data", 1, 0);
        *id = 0;
        *minute = 0;
        *speed = 0;
        return;
    }

    DeserializationError error = deserializeJson(doc, _readConfig);
    if (error) {
        lcd->print("Information", 0, 0);
        lcd->print(error.c_str(), 1, 0);
        *id = 0; *minute = 0; *speed = 0;
        return;
    }

    JsonObject _any = doc[configName];
    *id = _any["id"];
    *minute = _any["minute"];
    *speed = _any["speed"];
}

void SDCard::removeConfig(const String& configName) {
    String _readConfig = this->_read(fileConfig);
    StaticJsonDocument<500> doc;
    if (_readConfig == "null") {
        lcd->print("Information", 0, 0);
        lcd->print("No data", 1, 0);
        return;
    }

    DeserializationError error = deserializeJson(doc, _readConfig);
    if (error) {
        lcd->print("Information", 0, 0);
        lcd->print(error.c_str(), 1, 0);
        return;
    }

    if (doc.containsKey(configName)) {
        doc.remove(configName);
        String _newConfig;
        serializeJson(doc, _newConfig);
        this->_write(fileConfig, _newConfig);
    } else {
        lcd->print("Information", 0, 0);
        lcd->print("Config not found", 1, 0);
    }
}

void SDCard::writeReport(String date, String time, String finishTime, String speed, int _speed_pwm) {
    file = SD.open(fileReport, FILE_WRITE);
    if (file) {
        file.println("---------------------------");
        file.println("Report Result: ");
        file.println("Date\t: " + date);
        file.println("Time\t: " + time);
        file.println("Finish\t: " + finishTime);
        file.println("Speed\t: " + speed + "(PWM: " + String(_speed_pwm) + ")");
        file.println("---------------------------");
        file.println();
        file.close();
    }
    else {
        TSprintln("Failed to open file for writing");
        lcd->print("Information: ", 0, 0);
        lcd->print("Failed...", 0, 1);
    }
}

void SDCard::SDCardInit() {
    LiquidCrystal_animated runningText;
    TSprintln("Initializing SD Card...");
    for (byte i = 0; i <= 39; i++) {
        lcd->print(runningText.Scroll_LCD_Left("Initializing SD Card..."), 0, 0);
        delay(100);
    }
    runningText.Clear_Scroll_LCD_Left();

    while (true) {
        if (!SD.begin(pinCS)) {
            TSprintln("Initializing SD Card failed! Please insert SD Card");
            TSprintln("Or check this SD Card Module adapter...\n");
            for (byte i = 0; i <= 67; i++) {
                lcd->print(
                    runningText.Scroll_LCD_Left(
                        "Initializing SD Card failed! Please insert SD Card"
                    ), 0, 0
                );

                delay(200);
            }
        }

        else {
            TSprintln("Done... Welcome");
            lcd->print("Done... Welcome", 0, 1);
            delay(500);
            runningText.Clear_Scroll_LCD_Left();
            lcd->clear();
            break;
        }
    }
}