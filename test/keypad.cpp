// Example code for 4x4 keypad

#include <Arduino.h>
#include <Keypad.h>

class KeyTest {
public:
    void setup() {
        Serial.begin(115200);
    }
    
    void loop() {
        char key = keypad->getKey();
        if (key) {
            Serial.print(F("Key Pressed: "));
            Serial.println(key);
        }
    }

private:
    static const byte ROWS = 4;
    static const byte COLS = 4;

    char keys[ROWS][COLS] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    byte rowPins[ROWS] = {9, 8, 7, 6};
    byte colPins[COLS] = {5, 4, 3, 2};

    Keypad* keypad = new Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

};