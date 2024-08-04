#include <handleMotors>
#include <main.h>

// スピード入力関数
bool HandleMotors::insertSpeedMotors() {
    bool x;
    lcd->clear();
    lcd->print("Select speed: ", 0, 0); // スピード入力の表示
    lcd->print(key_lable[speedSelect], 0, 1); // 現在のスピード表示
    while (true) {
        char _key = keypad.getKey();
        if (_key) {
            lcd->clear();
            lcd->print("Select speed: ", 0, 0); // スピード入力の再表示

            if (_key == 'A' && _speed_motors != 0) {
                lcd->clear();
                pwmSpeed = map(_speed_motors, 50, 100, 128, 255); // スピードをPWMにマッピング
                driver.setSpeedPWM(pwmSpeed); // モーターのスピード設定
                x = true;
                break;
            }

            if (_key == 'B') {
                x = false;
                break;
            }

            bool validKey = false;
            for (uint8_t i = 0; i < sizeListSpeed; i++) {
                if (_key == key_map[i]) {
                    speedSelect = i;
                    _speed_motors = list_speed_motors[i];
                    lcd->print(key_lable[speedSelect], 0, 1); // 新しいスピード表示
                    validKey = true;
                }
            }

            if (!validKey) {
                speedSelect = 1;
                _speed_motors = list_speed_motors[1];
                lcd->print(key_lable[speedSelect], 0, 1); // デフォルトのスピード表示
            }
        }
    }

    return x;
}

// 継続時間入力関数
bool HandleMotors::insertDurations() {
    bool x;
    lcd->clear();
    lcd->print("Insert duration: ", 0, 0); // 継続時間入力の表示
    String inputDuration = ""; // ユーザー入力を保存

    while (true) {
        char _key = keypad.getKey();
        if (_key) {
            if (_key == 'A') { // 確認ボタン
                if (inputDuration.length() > 0) { // 入力が空でないことを確認
                    durations = inputDuration.toInt(); // 文字列を整数に変換
                    x = true;
                    break;
                }
            } else if (_key == 'D') { // 入力を削除するボタン
                inputDuration = ""; // 入力を削除
                lcd->clear();
                lcd->print("Insert duration: ", 0, 0); // 再表示
            } else if (isDigit(_key)) {
                inputDuration += _key; // 数字を入力に追加
                lcd->clear();
                lcd->print("Insert duration: ", 0, 0); // 再表示
                lcd->print(inputDuration + " s", 0, 1); // 現在の入力を表示
            }
            else if (_key == 'B') {
                x = false;
                break;
            }
        }
    }

    return x;
}

// 初期化関数
bool HandleMotors::init() {
    bool p1, p2;
    driver.init(PIN_MOTOR_A, PIN_MOTOR_B, PIN_MOTOR_C, PIN_MOTOR_D); // モーターの初期化
    _rtc.begin(); // RTCの初期化
    p1 = this->insertSpeedMotors(); // スピード入力
    p1 ? p2 = this->insertDurations() : p2 = false; // 継続時間入力

    if (p1 && p2) {
        return true;
    }
    else {
        return false;
    }
}

// メインの実行関数
void HandleMotors::run() {
    ready = init(); // 初期化

    if (ready) {
        DateTime startTime = _rtc.now();
        DateTime finishTime = startTime + TimeSpan(durations); // 継続時間を追加
        String formatFinishTime = finishTime.toString("hh:mm:ss"); // 終了時刻をフォーマット
        String finishTimeStr, remainingTime;
        bool stop_program = false;
        bool paused = false;
        int state = 0;
        unsigned long pausedAt = 0;
        unsigned long remainingSecs = durations;

        driver.run_forward(0); // モーターを連続的に動かす

        while (true) {
            if (!paused) {
                unsigned long currentMillis = millis();
                DateTime currentTime = _rtc.now();
                remainingSecs = finishTime.unixtime() - currentTime.unixtime(); // 残り時間を秒で計算

                if ((unsigned long)(currentMillis - LastMillis1) >= 1000) {
                    LastMillis1 = currentMillis;

                    lcd->clear();
                    if (state >= 0 && state <= 5) {
                        // カウントダウンタイマーの表示
                        if (remainingSecs > 0) {
                            lcd->print("Remaining time: ", 0, 0);
                            int minutes = remainingSecs / 60;
                            int seconds = remainingSecs % 60;
                            remainingTime = String(minutes) + ":" + (seconds < 10 ? "0" : "") + String(seconds);
                            lcd->print(remainingTime, 0, 1);
                        } else {
                            lcd->print("Finished!", 0, 0);
                        }
                    }

                    if (state >= 5 && state <= 10) {
                        String lblSpeed = key_lable[speedSelect] + " (" + String(pwmSpeed) + " PWM)";
                        lcd->print("Speed Motor: ", 0, 0);
                        lcd->print(lblSpeed, 0, 1); // スピード表示
                    }

                    if (state >= 10) {
                        // 終了時刻の表示
                        lcd->clear();
                        lcd->print("Finish at: ", 0, 0);
                        lcd->print(formatFinishTime, 0, 1);

                        if (state >= 15) state = 0;
                    }

                    state++;
                }

                // 時間が経過したかどうかを確認
                if (remainingSecs <= 0) {
                    break; // 継続時間が終了した場合、ループを終了
                }
            }

            // キーパッド入力の確認
            char key = keypad.getKey();
            if (key == 'B' && !paused) {
                lcd->clear();
                lcd->print("Pause program?", 0, 0);
                lcd->print("A: Stop  C: Resume", 0, 1);
                pausedAt = millis();
                paused = true;

                while (paused) {
                    char confirmKey = keypad.getKey();
                    if (confirmKey == 'A') {
                        driver.stops();
                        lcd->clear();
                        lcd->print("Stopped.", 0, 0);
                        delay(2000); // メッセージを2秒間表示
                        stop_program = true;
                        finishTimeStr = formatFinishTime + " (" + String(durations) + " seconds)\n";
                        finishTimeStr += "Remaining time\t: " + remainingTime + " seconds\n";
                        finishTimeStr += "Status\t: Stopped.";
                        paused = false;
                        break; // プログラムを停止した場合、run()関数を終了
                    } else if (confirmKey == 'C') {
                        lcd->clear();
                        lcd->print("Resuming...", 0, 0);
                        delay(2000); // メッセージを2秒間表示
                        finishTime = _rtc.now() + TimeSpan(remainingSecs);
                        formatFinishTime = finishTime.toString("hh:mm:ss"); // 終了時刻を再フォーマット
                        paused = false;
                    }
                }
            }

            if (stop_program) break;
            delay(100); // CPU負荷を減らすために待機
        }

        driver.stops(); // モーター停止

        lcd->clear();
        lcd->print("Done... ", 0, 0);
        delay(2000);
        lcd->print(formatFinishTime + "(" + String(durations) + "s)", 0, 0);
        lcd->print("Speed: " + key_lable[speedSelect], 0, 1); // 結果表示

        // レポートを保存
        if (!stop_program) {
            finishTimeStr = formatFinishTime + " (" + String(durations) + " seconds)\n";
            finishTimeStr += "Status\t: Done.";
        }
        delay(5000); // 結果を5秒間表示
        
        sdcard->writeReport(rtc.datestr(), rtc.timestr(), finishTimeStr, key_lable[speedSelect], pwmSpeed);
        TSprintln(finishTimeStr);
        delay(100);

        _speed_motors = 0;
        speedSelect = 0;
        pwmSpeed = 0;
        durations = 0;
    }
}
