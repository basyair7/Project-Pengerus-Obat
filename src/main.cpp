#include <main.h>
#include <variable.h>
#include <TypeSerial>
#include <LCDdisplay>
#include <DS3231rtc>
#include <SDCard>

#include <info>
#include <menu.hpp>
#include <standby.hpp>
#include <handleMotors>

// グローバルオブジェクトの初期化
LCDdisplay* lcd = new LCDdisplay;
DS3231rtc rtc;
SDCard sdcard;

StandByProgram standby;
HandleMotors handleMotors;
Menu menu;
Info info;
byte num_menu;

// メインプログラムクラス
class ProgramMain {
public:
    // インスタンスの取得
    static ProgramMain& instance() {
        static ProgramMain instance;
        return instance;
    }

    // セットアップ関数
    static void setup(void) {
        instance()._setup();
    }

    // ループ関数
    static void loop(void) {
        instance()._loop();
    }

private:
    // プライベートセットアップ関数
    void _setup() {
        TSbegin(115200);  // シリアル通信の初期化
        rtc.begin();      // RTCの初期化
        // rtc.autoAdjust(); // RTCの自動調整
        lcd->init();      // LCDの初期化
        sdcard.SDCardInit(); // SDカードの初期化
        menu.menu();      // メニューの初期化
    }

    // プライベートループ関数
    void _loop() {
        if (num_menu == 1) {
            handleMotors.run();  // モーターのハンドル実行
        }
        if (num_menu == 2) {
            info.run();  // 情報表示の実行
        }
        
        if (num_menu == 3) {
            standby.run(); // スタンバイモードの実行
        }

        
        menu.menu(); // メニューの再表示
        menu.switchToMenu(); // メニューへの切り替え
    }
};

void setup() { /*Arduinoのループ関数*/ ProgramMain::setup(); }
void loop() { /*Arduinoのセットアップ関数*/ ProgramMain::loop(); }
