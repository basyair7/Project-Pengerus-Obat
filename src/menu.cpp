/*! @file menu.cpp
 * @version 1.0.0
*/

#include <menu.hpp>
#include <main.h>

// Menu program
void Menu::menu() {
    lcd->clear();
    num_menu = 0;
    sizeListMenu = 3;
    const char key_map[] = {'1', '2', '3'};
    const char *option_menu[sizeListMenu] = {
        "1. Run Program",
        "2. Info Board",
        "3. Mode StandBy"
    };
    
    // display the first menu before the loop starts
    lcd->print(F("===== Menu ====="), 0, 0);
    lcd->print(option_menu[0], 0, 1);
    int x = 1;

    while (true) {
        char _key_btn = _keyMenu.getKey();
        if (_key_btn) {
            lcd->clear();
            lcd->print("===== Menu =====", 0, 0);

            if (_key_btn == 'A' && x != 0) {
                lcd->clear();
                num_menu = x;
                break;
            }

            bool validKey = false;
            for (uint8_t i = 0; i < sizeListMenu; i++) {
                if (_key_btn == key_map[i]) {
                    lcd->print(option_menu[i], 0, 1);
                    x = i + 1;
                    validKey = true;
                }
            }

            if (!validKey) {
                lcd->print(option_menu[0], 0, 1);
                x = 1;
            }
        }

        delay(50);
    }
}

// switch to menu
void Menu::switchToMenu() {
    char _key_btn = _keyMenu.getKey();
    if (_key_btn == 'B') {
        num_menu = 0;
        this->menu();
    }
}
