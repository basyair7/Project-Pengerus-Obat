#include <SDCard>
#include <main.h>
#include <variable.h>
#include <LiquidCrystal_animated.h>

void SDCard::addConfig(const String& configName, int id, int minute, int speed)
{
    // 現在の設定を読み取る
    String _readConfig = this->_read(fileConfig), _newConfig;
    StaticJsonDocument<500> doc;
    
    // 新しい設定を作成
    JsonObject _any = doc.createNestedObject(configName);
    _any["id"] = id;
    _any["minute"] = minute;
    _any["speed"] = speed;

    // 新しい設定をシリアライズして書き込む
    serializeJson(_any, _newConfig);
    this->_write(fileConfig, _newConfig);
}

void SDCard::readConfig(const String& configName, int* id, int* minute, int* speed) {
    // 設定を読み取る
    String _readConfig = this->_read(fileConfig);
    StaticJsonDocument<500> doc;
    if (_readConfig == "null") {
        // データがない場合の表示
        lcd->print("Information", 0, 0);
        lcd->print("No data", 1, 0);
        *id = 0;
        *minute = 0;
        *speed = 0;
        return;
    }

    // JSONデータのデシリアライズ
    DeserializationError error = deserializeJson(doc, _readConfig);
    if (error) {
        // エラーがあった場合の表示
        lcd->print("Information", 0, 0);
        lcd->print(error.c_str(), 1, 0);
        *id = 0; *minute = 0; *speed = 0;
        return;
    }

    // 設定データを取得
    JsonObject _any = doc[configName];
    *id = _any["id"];
    *minute = _any["minute"];
    *speed = _any["speed"];
}

void SDCard::removeConfig(const String& configName) {
    // 設定を読み取る
    String _readConfig = this->_read(fileConfig);
    StaticJsonDocument<500> doc;
    if (_readConfig == "null") {
        // データがない場合の表示
        lcd->print("Information", 0, 0);
        lcd->print("No data", 1, 0);
        return;
    }

    // JSONデータのデシリアライズ
    DeserializationError error = deserializeJson(doc, _readConfig);
    if (error) {
        // エラーがあった場合の表示
        lcd->print("Information", 0, 0);
        lcd->print(error.c_str(), 1, 0);
        return;
    }

    // 指定された設定が存在する場合、削除する
    if (doc.containsKey(configName)) {
        doc.remove(configName);
        String _newConfig;
        serializeJson(doc, _newConfig);
        this->_write(fileConfig, _newConfig);
    } else {
        // 設定が見つからなかった場合の表示
        lcd->print("Information", 0, 0);
        lcd->print("Config not found", 1, 0);
    }
}

void SDCard::writeReport(String date, String time, String finishTime, String speed, int _speed_pwm) {
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
        TSprintln("Failed to open file for writing");
        lcd->print("Information: ", 0, 0);
        lcd->print("Failed...", 0, 1);
    }
}

void SDCard::SDCardInit() {
    LiquidCrystal_animated runningText;
    TSprintln("Initializing SD Card...");
    // SDカード初期化メッセージの表示
    for (byte i = 0; i <= 39; i++) {
        lcd->print(runningText.Scroll_LCD_Left("Initializing SD Card..."), 0, 0);
        delay(100);
    }
    runningText.Clear_Scroll_LCD_Left();

    while (true) {
        // SDカードの初期化
        if (!SD.begin(pinCS)) {
            // 初期化失敗時の表示
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
            // 初期化成功時の表示
            TSprintln("Done... Welcome");
            lcd->print("Done... Welcome", 0, 1);
            delay(500);
            runningText.Clear_Scroll_LCD_Left();
            lcd->clear();
            break;
        }
    }
}
