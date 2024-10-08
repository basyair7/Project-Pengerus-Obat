/*! @file main.h
 * @version 1.0.0
*/

#pragma once

#include <Arduino.h>
#include "LCDdisplay"
#include "DS3231rtc"
#include "SDCard"

extern LCDdisplay* lcd;
extern DS3231rtc rtc;
extern SDCard sdcard;
extern byte num_menu;
