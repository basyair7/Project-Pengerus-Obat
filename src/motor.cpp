#include <motor.hpp>

// ドライバの初期化
void DriverMotor::init(uint8_t pinA, uint8_t pinB, uint8_t pinC, uint8_t pinD) {
    driver = new DSX_pinMotors(pinA, pinB, pinC, pinD); // DSX_pinMotorsオブジェクトの作成
}

// PWM速度を設定
void DriverMotor::setSpeedPWM(int speed) {
    driver->speedPWM(speed);
}

// モーターを前進させる
void DriverMotor::run_forward(unsigned long _delay) {
    if (_delay == 0) {
        driver->forward(); // モーターを前進させる
    } else {
        driver->forward(); // モーターを前進させる
        delay(_delay); // 指定した遅延時間を待つ
        driver->stops(); // モーターを停止させる
        delay(_delay); // 指定した遅延時間を待つ
    }
}

// モーターを後退させる
void DriverMotor::run_backward(unsigned long _delay) {
    if (_delay == 0) {
        driver->backward(); // モーターを後退させる
    } else {
        driver->backward(); // モーターを後退させる
        delay(_delay); // 指定した遅延時間を待つ
        driver->stops(); // モーターを停止させる
        delay(_delay); // 指定した遅延時間を待つ
    }
}

// モーターを停止させる
void DriverMotor::stops() {
    driver->stops(); // モーターを停止させる
}

// デストラクタ
DriverMotor::~DriverMotor() {
    delete driver; // DSX_pinMotorsオブジェクトを削除
}