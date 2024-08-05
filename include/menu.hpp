#ifndef MENU_HPP
#define MENU_HPP

#include <Arduino.h>
#include "variable.h"
#include "LCDdisplay"
#include <Keypad.h>

class Menu : protected VariableMillis {
public:
    // メニューの表示
    void menu();

    // メニューへの切り替え
    void switchToMenu();

private:
    uint8_t h;
    uint8_t sizeListMenu;
    Keypad _keyMenu = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
};

#endif