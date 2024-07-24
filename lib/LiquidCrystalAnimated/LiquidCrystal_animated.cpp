// Based on the work by DFRobot

#include "Arduino.h"
#include "LiquidCrystal_animated.h"

LiquidCrystal_animated::LiquidCrystal_animated(){
    // Nothing
}

String LiquidCrystal_animated::Scroll_LCD_Left(String StrDisplay) {
    String result;
    String StrProcess = "                " + StrDisplay + "                ";
    int StrProcessLength = StrProcess.length();
    result = StrProcess.substring(Li,Lii);
    Li++;
    Lii++;
    if (Li>StrProcessLength){
        Li=16;
        Lii=0;
    }
    return result;    
}

String LiquidCrystal_animated::Scroll_LCD_Right(String StrDisplay) {
    String result;
    String StrProcess = "                " + StrDisplay + "                ";
    if (Rii<1){
        Ri  = StrProcess.length();
        Rii = Ri-16;
    }
    result = StrProcess.substring(Rii,Ri);
    Ri--;
    Rii--;
    return result;
}

void LiquidCrystal_animated::Clear_Scroll_LCD_Left() {
    Li = 16;
    Lii = 0;
}

void LiquidCrystal_animated::Clear_Scroll_LCD_Right() {
    Ri = -1;
    Rii = -1;
}