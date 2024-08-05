#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <Arduino.h>
#include <DSXLite.h>

class DriverMotor {
public:
    ~DriverMotor();                                                    // デストラクタ
    void init(uint8_t pinA, uint8_t pinB, uint8_t pinC, uint8_t pinD); // ドライバの初期化
    void setSpeedPWM(int speed);                                       // PWM速度を設定
    void run_forward(unsigned long _delay);                            // モーターを前進させる
    void run_backward(unsigned long _delay);                           // モーターを後退させる
    void stops();                                                      // モーターを停止させる

private:
    // モーター制御のためのポインタ
    DSX_pinMotors* driver;
};

#endif
