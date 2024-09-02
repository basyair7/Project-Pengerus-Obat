/*! @file SDCard.cpp
 * @version 1.0.0
*/

#include <SDCard>
#include <main.h>
#include <variable.h>
#include <LiquidCrystal_animated.h>

void SDCard::SDCardInit() {
    LiquidCrystal_animated runningText;
    TSprintln("Initializing SD Card...");
    lcd->print("Information:", 0, 0);
    // display SDCard initialization message
    for (byte i = 0; i <= 39; i++) {
        lcd->print(runningText.Scroll_LCD_Left("Initializing SD Card..."), 0, 1);
        delay(150);
    }
    runningText.Clear_Scroll_LCD_Left();

    char quit;
    while (true) {
        // Attempt to initialize the SDCard
        if (!SD.begin(pinCS)) {
            // display a message if initialization fails
            TSprintln("Initializing SD Card failed! Please insert SD Card");
            TSprintln("Or check this SD Card Module adapter...\n");

            for (byte i = 0; i <= 67; i++) {
                // check if the 'C' key is pressed again
                quit = _key.getKey();
                if (quit == 'C') {
                    return;
                }

                lcd->print(
                    runningText.Scroll_LCD_Left(
                        "Initializing SD Card failed! Please insert SD Card"
                    ), 0, 1
                );
                delay(200);
            }
            
        } else {
            // display a message if initialization successful
            TSprintln("Done... Welcome");
            lcd->clear();
            lcd->print("Done", 0, 0);
            lcd->print("Welcome...", 0, 1);
            delay(500);
            runningText.Clear_Scroll_LCD_Left();
            lcd->clear();
            break;
        }
    }
}

void SDCard::backupState(int _speedSelect, int _speed_motor, int pwmSpeed, int durations, uint32_t remainingSecs, bool running) 
{
    TSprintln("Backing up state...");
    StaticJsonDocument<500> doc;
    doc["speedSelect"]      = _speedSelect;
    doc["speedMotor"]       = _speed_motor;
    doc["pwmSpeed"]         = pwmSpeed;
    doc["durations"]        = durations;
    doc["remainingSecs"]    = remainingSecs;
    doc["running"]          = running;

    String newState;
    serializeJson(doc, newState);

    this->_write(fileState, newState);
    TSprintln("State to be saved: " + newState);
}

void SDCard::restoreState(int *_speedSelect, int *_speed_motor, int *pwmSpeed, int *durations, uint32_t *remainingSecs, bool *running)
{
    StaticJsonDocument<500> doc;
    String readState = this->_read(fileState);

    if (readState == "null") {
        *_speedSelect   = 0;
        *_speed_motor   = 0;
        *pwmSpeed       = 0;
        *durations      = 0;
        *remainingSecs  = 0;
        *running        = false;

        doc["speedSelect"]      = 0;
        doc["speedMotor"]       = 0;
        doc["pwmSpeed"]         = 0;
        doc["durations"]        = 0;
        doc["remainingSecs"]    = 0;
        doc["running"]          = false;

        String newState;
        serializeJson(doc, newState);
        this->_write(fileState, newState);
    }

    else {
        DeserializationError error = deserializeJson(doc, readState);
        if (error) {
            TSprintln("Failed to deserialize restoreState: ");
            TSprintln(error.c_str());
            *_speedSelect   = 0;
            *_speed_motor   = 0;
            *pwmSpeed       = 0;
            *durations      = 0;
            *remainingSecs  = 0;
            *running        = false;
            return;
        }

        *_speedSelect   = doc["speedSelect"];
        *_speed_motor   = doc["speedMotor"];
        *pwmSpeed       = doc["pwmSpeed"];
        *durations      = doc["durations"];
        *remainingSecs  = doc["remainingSecs"];
        *running        = doc["running"];
    }
}

void SDCard::writeReport(String date, String time, String finishTime, String speed, String _speed_pwm) {
    LiquidCrystal_animated runningText;
    // open report file
    file = SD.open(fileReport, FILE_WRITE);
    if (file) {
        // write the contents of the report
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
        // display if file could not be opened
        TSprint("Failed to open file ");
        TSprint(fileReport);
        TSprintln(" for writing");
        lcd->clear();
        lcd->print("Information: ", 0, 0);
        String info_str = "Failed to save " + fileReport;
        for (int i = 0; i < (int)(info_str.length()) + 16; i++) {
            lcd->print(
                runningText.Scroll_LCD_Left(info_str), 0, 1
            );

            delay(200);
        }
    }
}

String SDCard::_read(const String& cfile) {
    String val;
    if (!SD.exists(cfile)) {
        val = "null";
    }
    
    file = SD.open(cfile, FILE_READ);
    if (file) {
        while (file.available()) {
            char c = file.read();
            val += c;
        }
        file.close();
    }
    else {
        val = "null";
    }

    return val;
}

void SDCard::_write(const String& cfile, String valJson) {
    if (SD.exists(cfile)) {
        SD.remove(cfile);
    }

    file = SD.open(cfile, FILE_WRITE);
    if (file) {
        file.seek(0);
        file.write((const uint8_t *) valJson.c_str(), valJson.length());
        file.close();
    }
    else {
        TSprint("Failed to create a ");
        TSprint(cfile);
        TSprintln(" file");
    }
}

bool SDCard::_remove(const String& cfile) {
    if (!SD.exists(cfile)) {
        return false;
    }
    
    return SD.remove(cfile);
}
