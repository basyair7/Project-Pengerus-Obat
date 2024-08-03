#include <handleMotors>
#include <main.h>


bool HandleMotors::insertSpeedMotors() {
    bool x;
    lcd->clear();
    lcd->print("Insert speed: ", 0, 0);
    lcd->print(key_lable[speedSelect], 0, 1);
    while (true) {
        char _key = keypad.getKey();
        if (_key) {
            lcd->clear();
            lcd->print("Insert speed: ", 0, 0);

            if (_key == 'A' && _speed_motors != 0) {
                lcd->clear();
                pwmSpeed = map(_speed_motors, 50, 100, 128, 255);
                driver.setSpeedPWM(pwmSpeed);
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
                    lcd->print(key_lable[speedSelect], 0, 1);
                    validKey = true;
                }
            }

            if (!validKey) {
                speedSelect = 1;
                _speed_motors = list_speed_motors[1];
                lcd->print(key_lable[speedSelect], 0, 1);
            }
        }
    }

    return x;
}

bool HandleMotors::insertDurations() {
    bool x;
    lcd->clear();
    lcd->print("Insert duration: ", 0, 0);
    String inputDuration = ""; // Menyimpan input dari pengguna

    while (true) {
        char _key = keypad.getKey();
        if (_key) {
            if (_key == 'A') { // Tombol konfirmasi
                if (inputDuration.length() > 0) { // Pastikan input tidak kosong
                    durations = inputDuration.toInt(); // Konversi string ke integer
                    x = true;
                    break;
                }
            } else if (_key == 'D') { // Tombol untuk menghapus input
                inputDuration = ""; // Hapus input jika tombol '*' ditekan
                lcd->clear();
                lcd->print("Insert duration: ", 0, 0);
            } else if (isDigit(_key)) {
                inputDuration += _key; // Tambahkan digit ke input
                lcd->clear();
                lcd->print("Insert duration: ", 0, 0);
                lcd->print(inputDuration + " s", 0, 1); // Tampilkan input saat ini
            }
            else if (_key == 'B') {
                x = false;
                break;
            }
        }
    }

    return x;
}

bool HandleMotors::init() {
    bool p1, p2;
    driver.init(PIN_MOTOR_A, PIN_MOTOR_B, PIN_MOTOR_C, PIN_MOTOR_D);
    _rtc.begin();
    p1 = this->insertSpeedMotors();
    p1 ? p2 = this->insertDurations() : p2 = false;

    if (p1 && p2) {
        return true;
    }
    else {
        return false;
    }
}

void HandleMotors::run() {
    ready = init();

    if (ready) {
        DateTime startTime = _rtc.now();
        DateTime finishTime = startTime + TimeSpan(durations); // Add duration in seconds
        String formatFinishTime = finishTime.toString("hh:mm:ss"); // Format finish time
        String finishTimeStr;
        bool stop_program = false;
        int state = 0;

        driver.run_forward(0); // Motor runs continuously

        while (true) {
            unsigned long currentMillis = millis();
            DateTime currentTime = _rtc.now();
            unsigned long remainingSecs = finishTime.unixtime() - currentTime.unixtime(); // Calculate remaining time in seconds

            if ((unsigned long) (currentMillis - LastMillis1) >= 1000) {
                LastMillis1 = currentMillis;

                lcd->clear();
                if (state <= 3) {
                    // Display countdown timer
                    if (remainingSecs > 0) {
                        lcd->print("Remaining time: ", 0, 0);
                        int minutes = remainingSecs / 60;
                        int seconds = remainingSecs % 60;
                        lcd->print(String(minutes) + ":" + (seconds < 10 ? "0" : "") + String(seconds), 0, 1);
                    } else {
                        lcd->print("Finished!", 0, 0);
                    }
                }

                if (state >= 4 && state <= 7) {
                    String lblSpeed = key_lable[speedSelect] + " (" + String(pwmSpeed) + " PWM)";
                    lcd->print("Speed Motor: ", 0, 0);
                    lcd->print(lblSpeed, 0, 1);
                }

                if (state >= 8) {
                    // Display finish time
                    lcd->clear();
                    lcd->print("Finish at: ", 0, 0);
                    lcd->print(formatFinishTime, 0, 1);

                    if (state >= 12) state = 0;
                }

                state++;
            }

            // Check if time has elapsed
            if (remainingSecs <= 0) {
                break; // Exit loop if duration has ended
            }

            // Check keypad input
            char key = keypad.getKey();
            if (key == 'B') {
                lcd->clear();
                lcd->print("Stop program?", 0, 0);
                lcd->print("A: Yes  C: No", 0, 1);

                while (true) {
                    char confirmKey = keypad.getKey();
                    if (confirmKey == 'A') {
                        driver.stops();
                        lcd->clear();
                        lcd->print("Stopped.", 0, 0);
                        delay(2000); // Display message for 2 seconds
                        stop_program = true;
                        finishTimeStr = formatFinishTime + " (" + String(durations) + " seconds)\n";
                        finishTimeStr += "Remaining time\t: " + String(remainingSecs) + " seconds\n";
                        finishTimeStr += "Status\t: Stopped.";
                        break; // Exit run() function if stopped
                    }
                    else if (confirmKey == 'C') {
                        lcd->clear();
                        lcd->print("Resuming...", 0, 0);
                        delay(2000); // Display message for 2 seconds
                        break; // Return to main loop
                    }
                }
            }

            if (stop_program) break;
            delay(100); // Reduce CPU load
        }

        driver.stops();

        lcd->clear();
        lcd->print("Done... ", 0, 0);
        delay(2000);
        lcd->print(formatFinishTime + "(" + String(durations) + "s)", 0, 0);
        lcd->print("Speed: " + key_lable[speedSelect], 0, 1);

        // save reporting
        if (!stop_program) {
            finishTimeStr = formatFinishTime + " (" + String(durations) + " seconds)\n";
            finishTimeStr += "Status\t: Done.";
        }
        // sdcard->writeReport(rtc.datestr(), rtc.timestr(), finishTimeStr, key_lable[speedSelect], pwmSpeed);
        TSprintln(finishTimeStr);
        delay(5000); // Display results for 5 seconds

        _speed_motors = 0;
        speedSelect = 0;
        pwmSpeed = 0;
        durations = 0;
    }
}
