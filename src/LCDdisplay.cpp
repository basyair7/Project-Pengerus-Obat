#include <LCDdisplay>

// LCDの初期化
void LCDdisplay::init() {
    lcd->init();
    lcd->backlight();
    lcd->clear();
}

// LCDのクリア
void LCDdisplay::clear() {
    lcd->clear();
}

// バックライトのオン/オフ
void LCDdisplay::backlight(bool x) {
    if (x) lcd->backlight();
    else lcd->noBacklight();
}

// 文字列の表示
void LCDdisplay::print(const String& x, const uint8_t col, const uint8_t row) {
    lcd->setCursor(col, row);
    lcd->print(x);
}
