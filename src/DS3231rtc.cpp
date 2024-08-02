#include <DS3231rtc>

void DS3231rtc::begin(String* status) {
    String _status;
    if (!_rtc.begin()) {
        _status = "Status: Couldn't find RTC";
        while(1);
    }

    if (_rtc.lostPower()) {
        _status = "RTC Lost power, setting the time!";
        _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    if (status != nullptr) 
        *status = _status;
}

void DS3231rtc::autoAdjust(bool enable) {
    if (enable) {
        _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

void DS3231rtc::date(uint8_t* day, uint8_t* month, uint8_t* year) {
    now = _rtc.now();
    
    if (day != nullptr)   *day    = now.day();
    if (month != nullptr) *month  = now.month();
    if (year != nullptr)  *year   = now.year();
}

void DS3231rtc::time(uint8_t* hour, uint8_t* minute, uint8_t* second) {
    now = _rtc.now();

    if (hour != nullptr)   *hour   = now.hour();
    if (minute != nullptr) *minute = now.minute();
    if (second != nullptr) *second = now.second();
}

String DS3231rtc::datestr() {
    now = _rtc.now();
    return String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
}

String DS3231rtc::timestr() {
    now = _rtc.now();
    return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}