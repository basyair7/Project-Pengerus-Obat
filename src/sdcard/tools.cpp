#include <SDCard>

String SDCard::_read(String cfile) {
    String val;
    if (!SD.exists(cfile)) {
        val = "null";
    }
    
    file = SD.open(cfile, FILE_READ);
    if (file) {
        while (file.available()) {
            val = file.readString();
        }
        file.close();
    }
    else {
        TSprintln(F("Failed to open file to writing"));
        val = "null";
    }

    return val;
}

void SDCard::_write(String cfile, String valJson) {
    file = SD.open(cfile, FILE_WRITE);
    if (file) {
        file.write((const uint8_t *)valJson.c_str(), valJson.length());
        file.close();
    }
    else {
        TSprintln(F("Failed to create a new file"));
        TSprintln(cfile);
        return;
    }
}

bool SDCard::_remove(String cfile) {
    if (!SD.exists(cfile)) {
        return false;
    }
    
    return SD.remove(cfile);
}