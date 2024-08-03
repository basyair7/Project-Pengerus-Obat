#include <DS3231rtc>

// RTCの初期化関数
void DS3231rtc::begin() {
    if (!_rtc.begin()) {
        TSprintln("Status: Couldn't find RTC");
        while(1);
    }

    // 電力が失われた場合の処理
    if (_rtc.lostPower()) {
        TSprintln(F("RTC Lost power, setting the time!"));
        _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

// RTCの時間を自動で調整する関数
void DS3231rtc::autoAdjust() {
    _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

// 現在の日付を取得する関数
void DS3231rtc::date(uint8_t* day, uint8_t* month, uint8_t* year) {
    now = _rtc.now();
    
    if (day != nullptr)   *day    = now.day();
    if (month != nullptr) *month  = now.month();
    if (year != nullptr)  *year   = now.year();
}

// 現在の時刻を取得する関数
void DS3231rtc::time(uint8_t* hour, uint8_t* minute, uint8_t* second) {
    now = _rtc.now();

    if (hour != nullptr)   *hour   = now.hour();
    if (minute != nullptr) *minute = now.minute();
    if (second != nullptr) *second = now.second();
}

// 日付を文字列として取得する関数
String DS3231rtc::datestr() {
    now = _rtc.now();
    return String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
}

// 時刻を文字列として取得する関数
String DS3231rtc::timestr() {
    now = _rtc.now();
    return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

// 現在のDateTimeオブジェクトを取得する関数
DateTime DS3231rtc::rtcNow() {
    DateTime now = _rtc.now();
    return now;
}

// 終了予定時刻を計算し、文字列として返す関数
String DS3231rtc::estimateFinishTime(int durationInSeconds) {
    DateTime now = _rtc.now();
    DateTime finishTime = now + TimeSpan(durationInSeconds); // 秒数を足す
    return finishTime.toString("hh:mm:ss");
}