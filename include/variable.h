/*! @file variable.h
 * @version 1.0.0
*/

#pragma once

#ifndef VARIABLE_H
#define VARIABLE_H

#include <Arduino.h>

// 定義と定数
#define pinCS 53

#define PIN_RELAY   22
#define PIN_MOTOR_A 11
#define PIN_MOTOR_B 10
#define PIN_MOTOR_C 13
#define PIN_MOTOR_D 12

// キーパッドの行と列の数
static const byte ROWS = 4;
static const byte COLS = 4;

// キーマップの定義
inline char keys[ROWS][COLS] = {
    {'1', '4', '7', '*'},
    {'2', '5', '8', '0'},
    {'3', '6', '9', '#'},
    {'A', 'B', 'C', 'D'}
};

// キーパッドのピンの定義
inline byte rowPins[ROWS] = {9, 8, 7, 6};
inline byte colPins[COLS] = {5, 4, 3, 2};

// ミリ秒管理クラス
class VariableMillis {
public:
    unsigned long LastMillis1; // 最後のミリ秒1
    unsigned long LastMillis2; // 最後のミリ秒2
};

#endif
