#include <SDCard>

void SDCard::addConfig(const String& configName, const int id, const int minute, const int speed)
{
    String _readConfig = this->_read(fileConfig), _newConfig;
    StaticJsonDocument<500> doc;
    
    JsonObject _any = doc.createNestedObject(configName);
    _any["id"] = id;
    _any["minute"] = minute;
    _any["speed"] = speed;

    serializeJson(_any, _newConfig);
    this->_write(fileConfig, _newConfig);
}

void SDCard::readConfig(const String& configName, int* id, int* minute, int* speed) {
    String _readConfig = this->_read(fileConfig);
    StaticJsonDocument<500> doc;
    if (_readConfig == "null") {
        this->print(F("Information"), 0, 0);
        this->print(F("No data"), 1, 0);
        *id = 0;
        *minute = 0;
        *speed = 0;
        return;
    }

    DeserializationError error = deserializeJson(doc, _readConfig);
    if (error) {
        this->print(F("Information"), 0, 0);
        this->print(error.c_str(), 1, 0);
        *id = 0; *minute = 0; *speed = 0;
        return;
    }

    JsonObject _any = doc[configName];
    *id = _any["id"];
    *minute = _any["minute"];
    *speed = _any["speed"];
}

void SDCard::removeConfig(const String& configName) {
    String _readConfig = this->_read(fileConfig);
    StaticJsonDocument<500> doc;
    if (_readConfig == "null") {
        this->print(F("Information"), 0, 0);
        this->print(F("No data"), 1, 0);
        return;
    }

    DeserializationError error = deserializeJson(doc, _readConfig);
    if (error) {
        this->print(F("Information"), 0, 0);
        this->print(error.c_str(), 1, 0);
        return;
    }

    if (doc.containsKey(configName)) {
        doc.remove(configName);
        String _newConfig;
        serializeJson(doc, _newConfig);
        this->_write(fileConfig, _newConfig);
    } else {
        this->print(F("Information"), 0, 0);
        this->print(F("Config not found"), 1, 0);
    }
}