/*! @file standby.hpp
 * @version 1.0.0
*/

#pragma once

#include <Arduino.h>
#include <Keypad.h>
#include "main.h"
#include "variable.h"
#include "TypeSerial"

// スタンバイプログラムクラス
class StandByProgram : protected VariableMillis {
public:
    // メイン実行関数
    void run() {
        int i = 0;
        while (true) {
            // キー入力を取得
            char key = _key.getKey();
            
            // 毎秒処理
            if ((unsigned long) (millis() - LastMillis1) >= (long unsigned int) 900) {
                LastMillis1 = millis();
                
                // 日付と時刻を表示
                TSprintln("Date: " + rtc.datestr());
                TSprintln("Time: " + rtc.timestr());
                lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print(F("Date: "));
                lcd->print(rtc.datestr());
                lcd->setCursor(0, 1);
                lcd->print(F("Time: "));
                lcd->print(rtc.timestr());

                // 10秒経過後、バックライトを消灯
                if (i >= 180) lcd->backlight(false);

                i++;
            }

            // キーが押されたら
            if (key) {
                if (key == 'B') {
                    lcd->backlight(true);
                    break; // ループを抜ける
                }
                
                // キーが押された場合、バックライトを点灯しカウンタをリセット
                lcd->backlight(true);
                i = 0;
            }
            
            delay(100);
        }
    }

private:
    Keypad _key = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
};
