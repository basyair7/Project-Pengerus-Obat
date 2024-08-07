#include <SDCard>
#include <main.h>
#include <variable.h>
#include <LiquidCrystal_animated.h>

void SDCard::SDCardInit() {
    LiquidCrystal_animated runningText;
    TSprintln("Initializing SD Card...");
    lcd->print("Information:", 0, 0);
    // SDカードの初期化メッセージを表示
    for (byte i = 0; i <= 39; i++) {
        lcd->print(runningText.Scroll_LCD_Left("Initializing SD Card..."), 0, 1);
        delay(150);
    }
    runningText.Clear_Scroll_LCD_Left();

    char quit;
    while (true) {
        // SDカードの初期化を試みる
        if (!SD.begin(pinCS)) {
            // 初期化が失敗した場合のメッセージを表示
            TSprintln("Initializing SD Card failed! Please insert SD Card");
            TSprintln("Or check this SD Card Module adapter...\n");

            for (byte i = 0; i <= 67; i++) {
                // 'C' キーが再度押されているかどうかを確認する
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
            // 初期化が成功した場合のメッセージを表示
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

void SDCard::backupState(int _speedSelect, int _speed_motor, int pwmSpeed, int durations, unsigned long remainingSecs, bool running) 
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

    TSprintln("State to be saved: " + newState);

    if (this->_remove(fileState)) {
        this->_write(fileState, newState);
    }
}

void SDCard::restoreState(int *_speedSelect, int *_speed_motor, int *pwmSpeed, int *durations, unsigned long *remainingSecs, bool *running)
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
    // レポートファイルを開く
    file = SD.open(fileReport, FILE_WRITE);
    if (file) {
        // レポート内容を書き込む
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
        // ファイルが開けなかった場合の表示
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

String SDCard::_read(String cfile) {
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

void SDCard::_write(String cfile, String valJson) {
    file = SD.open(cfile, FILE_WRITE);
    if (file) {
        file.write((const uint8_t *) valJson.c_str(), valJson.length());
        file.close();
    }
    else {
        TSprint("Failed to create a ");
        TSprint(cfile);
        TSprintln(" file");
    }
}

bool SDCard::_remove(String cfile) {
    if (!SD.exists(cfile)) {
        return false;
    }
    
    return SD.remove(cfile);
}