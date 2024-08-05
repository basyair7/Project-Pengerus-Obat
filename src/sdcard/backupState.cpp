#include <SDCard>

void SDCard::backupState(int _speedSelect, int _speed_motor, int pwmSpeed, int durations, unsigned long remainingSecs, bool running) 
{
    StaticJsonDocument<500> doc;
    doc["speedSelect"]      = _speedSelect;
    doc["speedMotor"]       = _speed_motor;
    doc["pwmSpeed"]         = pwmSpeed;
    doc["durations"]        = durations;
    doc["remainingSecs"]    = remainingSecs;
    doc["running"]          = running;

    String newState;
    serializeJson(doc, newState);

    file = SD.open(fileState, FILE_WRITE);
    file.print("hello");
    file.close();
}