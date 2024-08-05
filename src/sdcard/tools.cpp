#include <SDCard>
#include <main.h>

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
        val = "null";
    }

    return val;
}

void SDCard::_write(String cfile, String valJson) {
    file = SD.open(cfile, FILE_WRITE);
    if (file) {
        file.print(valJson);
        file.close();
    }
    else {
        TSprintln("Failed to create a new file");
        return;
    }
}

bool SDCard::_remove(String cfile) {
    if (!SD.exists(cfile)) {
        return false;
    }
    
    return SD.remove(cfile);
}