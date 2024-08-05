#include <Arduino.h>
#include <Keypad.h>

class KeyTest {
public:
    void setup() {
        Serial.begin(115200);
        Serial.println("Calculator Test: ");
    }
    
    void loop() {
        char key = keypad->getKey();
        if (key) {
            handleKeyPress(key);
        }
    }

private:
    static const byte ROWS = 4;
    static const byte COLS = 4;

    char keys[4][4] = {
        {'1', '4', '7', '*'},
        {'2', '5', '8', '0'},
        {'3', '6', '9', '#'},
        {'A', 'B', 'C', 'D'}
    };

    byte rowPins[4] = {9, 8, 7, 6};
    byte colPins[4] = {5, 4, 3, 2};

    Keypad* keypad = new Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

    float value1 = 0;
    float value2 = 0;
    char operatorKey = '\0';
    bool isEnteringSecondValue = false;
    bool isDecimalMode = false;
    float decimalMultiplier = 0.1; // Used for decimal input

    void handleKeyPress(char key) {
        if (isDigit(key)) {
            int keyAsInt = key - '0';
            Serial.print(keyAsInt);
            if (isEnteringSecondValue) {
                if (isDecimalMode) {
                    value2 += keyAsInt * decimalMultiplier;
                    decimalMultiplier *= 0.1;
                } else {
                    value2 = value2 * 10 + keyAsInt;
                }
            } else {
                if (isDecimalMode) {
                    value1 += keyAsInt * decimalMultiplier;
                    decimalMultiplier *= 0.1;
                } else {
                    value1 = value1 * 10 + keyAsInt;
                }
            }
        } else if (key == 'D') {
            // Switch to decimal mode only if a number has already been entered
            if (!isEnteringSecondValue || (value1 > 0 && !isDecimalMode)) {
                isDecimalMode = true;
                decimalMultiplier = 0.1; // Reset decimal multiplier
                Serial.print(F("."));
            }
        } else if (key == 'A' || key == 'B' || key == '*' || key == 'C') {
            operatorKey = key;
            isEnteringSecondValue = true;
            isDecimalMode = false; // Reset decimal mode for the next value
            decimalMultiplier = 0.1; // Reset decimal multiplier for the next value
            Serial.print(F(" "));
            Serial.print(getOperatorSymbol(operatorKey));
            Serial.print(F(" "));
        } else if (key == '#') {
            float result = 0;
            switch (operatorKey) {
                case 'A': result = value1 + value2; break;
                case 'B': result = value1 - value2; break;
                case '*': result = value1 * value2; break;
                case 'C': result = value1 / value2; break;
            }
            printResult(result);
            resetCalculator();
        }
    }

    char getOperatorSymbol(char op) {
        switch (op) {
            case 'A': return '+';
            case 'B': return '-';
            case '*': return '*';
            case 'C': return '/';
            default: return '?';
        }
    }

    void printResult(float result) {
        // Convert to string with a fixed number of decimal places
        // char resultStr[10];
        // dtostrf(result, 1, 1, resultStr); // Format: width=1, decimals=1

        // // Replace dot with comma
        // for (int i = 0; i < (int)strlen(resultStr); i++) {
        //     if (resultStr[i] == '.') {
        //         resultStr[i] = '.';
        //         break;
        //     }
        // }

        Serial.print(F(" = "));
        // Serial.println(resultStr);
        Serial.println(result);
    }

    void resetCalculator() {
        value1 = 0;
        value2 = 0;
        operatorKey = '\0';
        isEnteringSecondValue = false;
        isDecimalMode = false;
        decimalMultiplier = 0.1;
    }
};
