#include <SDCard>
#include <main.h>
#include <variable.h>
#include <LiquidCrystal_animated.h>

void SDCard::writeReport(String date, String time, String finishTime, String speed, String _speed_pwm) {
    LiquidCrystal_animated runningText;
    // レポートファイルを開く
    file = SD.open(fileReport, FILE_WRITE);
    if (file) {
        // レポート内容を書き込む
        file.println("---------------------------");
        file.println("Report Result: ");
        file.println("Date\t: " + date);
        file.println("Time\t: " + time);
        file.println("Finish\t: " + finishTime);
        file.println("Speed\t: " + speed + "(PWM: " + String(_speed_pwm) + ")");
        file.println("---------------------------");
        file.println();
        file.close();
    }
    else {
        // ファイルが開けなかった場合の表示
        TSprintln("Failed to open file for writing");
        lcd->clear();
        lcd->print("Information: ", 0, 0);
        String info_str = "Failed to save report.txt";
        for (int i = 0; i < (int)(info_str.length()) + 16; i++) {
            lcd->print(
                runningText.Scroll_LCD_Left(info_str), 0, 1
            );

            delay(200);
        }
    }
}

void SDCard::SDCardInit() {
    LiquidCrystal_animated runningText;
    TSprintln("Initializing SD Card...");
    lcd->print("Information:", 0, 0);
    // SDカードの初期化メッセージを表示
    for (byte i = 0; i <= 39; i++) {
        lcd->print(runningText.Scroll_LCD_Left("Initializing SD Card..."), 0, 1);
        delay(150);
    }
    runningText.Clear_Scroll_LCD_Left();

    char quit;
    while (true) {
        // SDカードの初期化を試みる
        if (!SD.begin(pinCS)) {
            // 初期化が失敗した場合のメッセージを表示
            TSprintln("Initializing SD Card failed! Please insert SD Card");
            TSprintln("Or check this SD Card Module adapter...\n");

            for (byte i = 0; i <= 67; i++) {
                // 'C' キーが再度押されているかどうかを確認する
                quit = _key.getKey();
                if (quit == 'C') {
                    return;
                }

                lcd->print(
                    runningText.Scroll_LCD_Left(
                        "Initializing SD Card failed! Please insert SD Card"
                    ), 0, 1
                );
                delay(200);
            }
            
        } else {
            // 初期化が成功した場合のメッセージを表示
            TSprintln("Done... Welcome");
            lcd->clear();
            lcd->print("Done", 0, 0);
            lcd->print("Welcome...", 0, 1);
            delay(500);
            runningText.Clear_Scroll_LCD_Left();
            lcd->clear();
            break;
        }
    }
}
