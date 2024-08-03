// DSX-07 Lite Motor L293D
// By Ahul07
// 2020

#ifndef DSXLite_h
#define DSXLite_h

#if defined (ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #if defined (__AVR__)
    #include <avr/io.h>
  #endif
#endif

class DSX_pinMotors {
  private:
    int _speed_pwm;
    uint8_t dinamo1;
    uint8_t dinamo2;
    uint8_t dinamo3;
    uint8_t dinamo4;

  public:
    // Constructor
    DSX_pinMotors(uint8_t motorA, uint8_t motorB, uint8_t motorC, uint8_t motorD);

    // Set the PWM speed (0-255)
    void speedPWM(int speed);

    // Set the pin modes
    void pinMotors();

    // Motor control functions
    void forward();
    void backward();
    void turnRight();
    void turnLeft();
    void stops();
};

#endif
// DSX-07 Lite Motor L293D
// By Ahul07
// 2020

#ifndef DSXLite_h
#define DSXLite_h

#if defined (ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #if defined (__AVR__)
    #include <avr/io.h>
  #endif
#endif

class DSX_pinMotors {
  private:
    int _speed_pwm; // PWM速度
    uint8_t dinamo1; // モーター1のピン
    uint8_t dinamo2; // モーター2のピン
    uint8_t dinamo3; // モーター3のピン
    uint8_t dinamo4; // モーター4のピン

  public:
    // コンストラクタ
    DSX_pinMotors(uint8_t motorA, uint8_t motorB, uint8_t motorC, uint8_t motorD);

    // PWM速度を設定する (0-255)
    void speedPWM(int speed);

    // ピンモードを設定する
    void pinMotors();

    // モーターの制御関数
    void forward(); // 前進
    void backward(); // 後退
    void turnRight(); // 右回転
    void turnLeft(); // 左回転
    void stops(); // 停止
};

#endif
