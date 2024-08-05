#include <SDCard>

void SDCard::restoreState(int *_speedSelect, int *_speed_motor, int *pwmSpeed, int *durations, unsigned long *remainingSecs, bool *running)
{
    StaticJsonDocument<500> doc;
    file = SD.open(fileState, FILE_READ);
    String readState;
    if (file && SD.exists(fileState)) {
        while (file.available()) {
            readState = file.readString();
        }
        file.close();
    }
    else {
        readState = "null";
    }

    if (readState == "null") {
        *_speedSelect   = 0;
        *_speed_motor   = 0;
        *pwmSpeed       = 0;
        *durations      = 0;
        *remainingSecs  = 0;
        *running        = false;

        doc["speedSelect"]      = 0;
        doc["speedMotor"]       = 0;
        doc["pwmSpeed"]         = 0;
        doc["durations"]        = 0;
        doc["remainingSecs"]    = 0;
        doc["running"]          = false;

        String newState;
        serializeJson(doc, newState);
        this->_write(fileState, newState);
    }

    else {
        DeserializationError error = deserializeJson(doc, readState);
        if (error) {
            TSprintln("Failed to deserialize restoreState: ");
            TSprintln(error.c_str());
            *_speedSelect   = 0;
            *_speed_motor   = 0;
            *pwmSpeed       = 0;
            *durations      = 0;
            *remainingSecs  = 0;
            *running        = false;
            return;
        }

        *_speedSelect   = doc["speedSelect"];
        *_speed_motor   = doc["speedMotor"];
        *pwmSpeed       = doc["pwmSpeed"];
        *durations      = doc["durations"];
        *remainingSecs  = doc["remainingSecs"];
        *running        = doc["running"];
    }
}