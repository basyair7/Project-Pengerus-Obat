/*! @file DS3231rtc.cpp
 * @version 1.0.0
*/

#include <DS3231rtc>

// RTC initialization function
void DS3231rtc::begin() {
    if (!_rtc.begin()) {
        TSprintln("Status: Couldn't find RTC");
        while(1);
    }

    // Display message in case of power loss
    if (_rtc.lostPower()) {
        TSprintln(F("RTC Lost power, setting the time!"));
        _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

DateTime DS3231rtc::DSnow() {
    return _rtc.now();
}

// function to automatically adjust RTC time
void DS3231rtc::autoAdjust() {
    _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

// function to get the current date
void DS3231rtc::date(uint8_t* day, uint8_t* month, uint8_t* year) {
    now = _rtc.now();
    
    if (day != nullptr)   *day    = now.day();
    if (month != nullptr) *month  = now.month();
    if (year != nullptr)  *year   = now.year();
}

// function to get the current time
void DS3231rtc::time(uint8_t* hour, uint8_t* minute, uint8_t* second) {
    now = _rtc.now();

    if (hour != nullptr)   *hour   = now.hour();
    if (minute != nullptr) *minute = now.minute();
    if (second != nullptr) *second = now.second();
}

// function to get the date as a string
String DS3231rtc::datestr() {
    now = _rtc.now();
    return String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
}

// function to get the time as a string
String DS3231rtc::timestr() {
    now = _rtc.now();
    return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

// function to get the current DateTime object
DateTime DS3231rtc::rtcNow() {
    DateTime now = _rtc.now();
    return now;
}

// Function to calculate the estimated end time and return it as a string
String DS3231rtc::estimateFinishTime(int durationInSeconds) {
    DateTime now = _rtc.now();
    DateTime finishTime = now + TimeSpan(durationInSeconds); // 秒数を足す
    return finishTime.toString("hh:mm:ss");
}
