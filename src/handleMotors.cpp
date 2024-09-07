/*! @file handleMotors.cpp
 * @version 1.0.0
*/

#include <handleMotors>
#include <main.h>

// speed input function
bool HandleMotors::insertSpeedMotors() {
    lcd->clear();
    speedSelect = 0;
    _speed_motors = list_speed_motors[speedSelect];
    lcd->print("Select speed: ", 0, 0);
    lcd->print(key_lable[speedSelect], 0, 1); // current speed display
    delay(100);

    while (true) {
        char _key = keypad.getKey();
        if (_key) {
            lcd->clear();
            lcd->print("Select speed: ", 0, 0); // Redisplay of speed input

            for (uint8_t i = 0; i < sizeListSpeed; i++) {
                if (_key == key_map[i]) {
                    speedSelect = i;
                    _speed_motors = list_speed_motors[speedSelect];
                    lcd->print(key_lable[speedSelect], 0, 1); // show speed options
                }
            }

            if (_key == 'A' && _speed_motors != 0) {
                lcd->clear();
                pwmSpeed = map(_speed_motors, 50, 100, 128, 255); // mapping speed to PWM
                return true;
            }
            if (_key == 'B') {
                return false;
            }
            else {
                lcd->print(key_lable[speedSelect], 0, 1); // show speed options
            }
        }
    }
}

// duration entry function
bool HandleMotors::insertDurations() {
    lcd->clear();
    lcd->print("Insert duration: ", 0, 0); // display of duration input
    String inputDuration = ""; // saving user input

    while (true) {
        char _key = keypad.getKey();
        if (_key) {
            if (_key == 'A') { // confirm button
                if (inputDuration.length() > 0) { // make sure input not emptry
                    durations = inputDuration.toInt(); // convert string to integer
                    return true;
                }
            } else if (_key == 'D') { // delete input button
                inputDuration = ""; // delete accommodated data
                lcd->clear();
                lcd->print("Insert duration: ", 0, 0);
            } else if (isDigit(_key)) {
                inputDuration += _key; // add numbers to input
                lcd->clear();
                // show current input
                lcd->print("Insert duration: ", 0, 0);
                lcd->print(inputDuration + " s", 0, 1); 
            }
            else if (_key == 'B') {
                return false;
            }
        }
    }
}

// intialization function
bool HandleMotors::init() {
    LiquidCrystal_animated runningText;
    bool state = false;
    driver.init(PIN_MOTOR_A, PIN_MOTOR_B, PIN_MOTOR_C, PIN_MOTOR_D); // initialization of motors
    _rtc.begin(); // intialize rtc

    restoreStateFromSDCard();
    if (running && remainingSecs > 0) {
        lcd->clear();
        // lcd->print("Resuming this program?", 0, 0);
        String top_str = "Resuming this program?";
        char key;
        lcd->print("*: yes, #: no", 0, 1);

        while (true) {
            for (int i = 0; i < (int)(top_str.length() + 16); i++) {
                key = keypad.getKey();
                if (key) break;
                lcd->print(runningText.Scroll_LCD_Left(top_str), 0, 0);
                delay(200);
            }

            if (key == '*') {
                return true;
            }
            else if (key == 'B') {
                return false;
            }

            else if (key == '#') {
                clearStateInSDCard();
                break;
            }
        }
    }

    bool speedInput    = this->insertSpeedMotors(); // get speed input
    bool durationInput = speedInput && this->insertDurations(); // get duration input

    if (durationInput) {
        remainingSecs = durations; // set initial remaining time
        state = durationInput;
    }

    return durationInput;
}

// main execution function
void HandleMotors::run() {
    ready = init(); // get initializing boolean

    if (ready) {
        DateTime finishTime = _rtc.now() + TimeSpan(remainingSecs); // add remaining time
        String formatFinishTime = finishTime.toString("hh:mm:ss"); // format the end time
        String finishTimeStr, remainingTime;
        
        running = true; // program is running
        bool stop_program = false;
        bool paused = false;
        int state = 0;
        unsigned long pausedAt = 0;
        relayState = ON;

        driver.setSpeedPWM(pwmSpeed); // set speed motors
        relay.write(relayState, 1000);
        float pwmPercentage = (pwmSpeed / 255.0) * 100;
        
        if (remainingSecs >= 60) {
            saveStateToSDCard();
        }

        while (true) {
            if (!paused) {
                driver.run_forward(0); // run the motors
                unsigned long currentMillis = millis();
                DateTime currentTime = _rtc.now();
                remainingSecs = finishTime.unixtime() - currentTime.unixtime(); // remaining time in seconds

                if ((unsigned long)(currentMillis - LastMillis1) >= 1000) {
                    LastMillis1 = currentMillis;
                    lcd->clear();
                    
                    if (state >= 0 && state <= 5) {
                        // countdown time display
                        if (remainingSecs > 0) {
                            lcd->print("Remaining time: ", 0, 0);
                            int hours   = remainingSecs / 3600;
                            int minutes = (remainingSecs % 3600) / 60;
                            int seconds = remainingSecs % 60;
                            remainingTime = String(hours) + ":" + 
                                            (minutes < 10 ? "0" : "") + String(minutes) + ":" + 
                                            (seconds < 10 ? "0" : "") + String(seconds);
                            lcd->print(remainingTime, 0, 1);
                        }
                    }

                    if (state >= 5 && state <= 10) {
                        String lblSpeed = key_lable[speedSelect] + " (" + String(pwmPercentage) + " %)  ";
                        lcd->print(lblSpeed, 0, 0);
                        lcd->print("Finish at: " + formatFinishTime, 0, 1);
                    }
                    state++;
                    if (state > 10) state = 0;
                }

                if (remainingSecs <= 0) {
                    driver.stops(); // motor stops
                    running = false; // end the program
                    stop_program = true;
                    break; // loop ends when program terminates
                }
            }

            // confirmation of keypad input
            char key = keypad.getKey();
            if (key == 'B' && !paused) {
                relayState = OFF;
                driver.stops();
                relay.write(relayState, 1000);
                lcd->clear();
                lcd->print("Pause program...", 0, 0);
                lcd->print("A:Stop C:Resume", 0, 1);
                pausedAt = millis();
                paused = true;

                while (paused) {
                    char confirmKey = keypad.getKey();
                    if (confirmKey == 'A') {
                        lcd->clear();
                        lcd->print("Stopped.", 0, 0);
                        delay(2000);
                        stop_program = true;
                        finishTimeStr = formatFinishTime + " (" + String(durations) + " seconds)\n";
                        finishTimeStr += "Remaining time\t: " + remainingTime + " seconds\n";
                        finishTimeStr += "Status\t: Stopped.";
                        paused = false;
                        running = false; // program stop
                        break; // exit run() functions when program is stopped
                    } 
                    else if (confirmKey == 'C') {
                        lcd->clear();
                        lcd->print("Resuming...", 0, 0);
                        delay(2000);
                        finishTime = _rtc.now() + TimeSpan(remainingSecs);
                        formatFinishTime = finishTime.toString("hh:mm:ss"); // reformat the end time
                        paused = false;
                        running = true; // program resume
                    }
                }
            }

            if (stop_program) break;
            saveDurationToSDCard();
            delay(100); // standby to reduce CPU load
        }

        driver.stops(); // motor stops

        lcd->clear();
        lcd->print("Finished! ", 0, 0);
        delay(2000);
        lcd->print(formatFinishTime + "(" + String(durations) + "s)", 0, 0);
        lcd->print("Speed: " + key_lable[speedSelect], 0, 1); // result display

        // save report
        if (!stop_program) {
            finishTimeStr = formatFinishTime + " (" + String(durations) + " seconds)\n";
            finishTimeStr += "Status\t: Done.";
        }
        delay(3000); // display results for 3 seconds
        
        sdcard.writeReport(rtc.datestr(), rtc.timestr(), finishTimeStr, key_lable[speedSelect], String(pwmPercentage) + "%");
        TSprintln(finishTimeStr);
        clearStateInSDCard();
        delay(1000);
    }
}

void HandleMotors::saveStateToSDCard() {
    sdcard.backupState(
        relayState,
        speedSelect, 
        _speed_motors, 
        pwmSpeed, 
        durations, 
        remainingSecs, 
        running
    );
}

void HandleMotors::saveDurationToSDCard() {
    if ((unsigned long) (millis() - LastMillis2) >= 60000) {
        LastMillis2 = millis();
        this->saveStateToSDCard();
    }
}

void HandleMotors::restoreStateFromSDCard() {
    sdcard.restoreState(
        &relayState,
        &speedSelect,
        &_speed_motors, 
        &pwmSpeed, 
        &durations, 
        &remainingSecs, 
        &running
    );
}

void HandleMotors::clearStateInSDCard() {
    relayState    = OFF;
    speedSelect   = 0;
    _speed_motors = 0;
    pwmSpeed      = 0;
    durations     = 0;
    remainingSecs = 0;
    running       = false;

    this->saveStateToSDCard();
}
