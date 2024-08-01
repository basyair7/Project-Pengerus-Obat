#include <LCDdisplay>
#include <Print.h>

void LCDdisplay::init() {
    lcd->init();
    lcd->backlight();
    lcd->clear();
}

void LCDdisplay::clear() {
    lcd->clear();
}

void LCDdisplay::print(const String& x, const uint8_t col, const uint8_t row) {
    lcd->setCursor(col, row);
    lcd->print(x);
}
