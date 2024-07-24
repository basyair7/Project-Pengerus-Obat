//YWROBOT
#ifndef LiquidCrystal_animated_h
#define LiquidCrystal_animated_h

#include <Arduino.h>

class LiquidCrystal_animated {
    private:
        //----------------------------------
        int Li          = 17;
        int Lii         = 1; 
        int Ri          = -1;
        int Rii         = -1;
        //----------------------------------
    public:
        LiquidCrystal_animated();
        String Scroll_LCD_Left(String StrDisplay);
        String Scroll_LCD_Right(String StrDisplay);
        void Clear_Scroll_LCD_Left();
        void Clear_Scroll_LCD_Right();
};

#endif
