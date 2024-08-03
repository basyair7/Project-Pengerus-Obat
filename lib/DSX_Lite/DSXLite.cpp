// DSX-07 Lite Motor L293D
// By Ahul07
// 2020

#include "DSXLite.h"
#include "Arduino.h"

// ピンボードのライブラリの宣言
DSX_pinMotors::DSX_pinMotors(uint8_t motorA, uint8_t motorB, uint8_t motorC, uint8_t motorD) {
  dinamo1 = motorA; // モーター1のピン
  dinamo2 = motorB; // モーター2のピン
  dinamo3 = motorC; // モーター3のピン
  dinamo4 = motorD; // モーター4のピン
  _speed_pwm = 255; // デフォルトの速度 (最大速度)
}

void DSX_pinMotors::speedPWM(int speed) {
  _speed_pwm = constrain(speed, 0, 255); // 速度が0から255の範囲に制限されていることを確認
}

// `setup`のためのピンモード設定
void DSX_pinMotors::pinMotors() {
  pinMode(dinamo1, OUTPUT); // モーター1のピンを出力として設定
  pinMode(dinamo2, OUTPUT); // モーター2のピンを出力として設定
  pinMode(dinamo3, OUTPUT); // モーター3のピンを出力として設定
  pinMode(dinamo4, OUTPUT); // モーター4のピンを出力として設定
}

// モーターを前進させる
void DSX_pinMotors::forward() {
  analogWrite(dinamo1, _speed_pwm); // モーター1にPWM信号を送信
  analogWrite(dinamo2, 0); // モーター2をオフ
  analogWrite(dinamo3, _speed_pwm); // モーター3にPWM信号を送信
  analogWrite(dinamo4, 0); // モーター4をオフ
}

// モーターを後退させる
void DSX_pinMotors::backward() {
  analogWrite(dinamo1, 0); // モーター1をオフ
  analogWrite(dinamo2, _speed_pwm); // モーター2にPWM信号を送信
  analogWrite(dinamo3, 0); // モーター3をオフ
  analogWrite(dinamo4, _speed_pwm); // モーター4にPWM信号を送信
}

// モーターを右に回転させる
void DSX_pinMotors::turnRight() {
  analogWrite(dinamo1, 0); // モーター1をオフ
  analogWrite(dinamo2, _speed_pwm); // モーター2にPWM信号を送信
  analogWrite(dinamo3, _speed_pwm); // モーター3にPWM信号を送信
  analogWrite(dinamo4, 0); // モーター4をオフ
}

// モーターを左に回転させる
void DSX_pinMotors::turnLeft() {
  analogWrite(dinamo1, _speed_pwm); // モーター1にPWM信号を送信
  analogWrite(dinamo2, 0); // モーター2をオフ
  analogWrite(dinamo3, 0); // モーター3をオフ
  analogWrite(dinamo4, _speed_pwm); // モーター4にPWM信号を送信
}

// モーターを停止させる
void DSX_pinMotors::stops() {
  analogWrite(dinamo1, 0); // モーター1をオフ
  analogWrite(dinamo2, 0); // モーター2をオフ
  analogWrite(dinamo3, 0); // モーター3をオフ
  analogWrite(dinamo4, 0); // モーター4をオフ
}
