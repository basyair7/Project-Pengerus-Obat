#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_animated.h>

#define DELAYMS 350

LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_animated ChrAnimated;

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i <= 12; i++) {
    lcd.setCursor(0, 0);
    lcd.print(ChrAnimated.Scroll_LCD_Right("Basyair7"));
    delay(DELAYMS);
  }
  
  for (int i = 0; i <= 46; i++) {
    lcd.setCursor(0, 1);
    lcd.print(ChrAnimated.Scroll_LCD_Left("Cara Membuat Text Berjalan...."));
    delay(DELAYMS);
  }
  ChrAnimated.Clear_Scroll_LCD_Left();

  for (int i = 16; i <= 27; i++) {
    lcd.setCursor(0, 0);
    lcd.print(ChrAnimated.Scroll_LCD_Right("Basyair7"));
    delay(DELAYMS);
  }
  ChrAnimated.Clear_Scroll_LCD_Right();
  
  for (int i = 0; i <= 12; i++) {
    lcd.setCursor(0, 0);
    lcd.print(ChrAnimated.Scroll_LCD_Right("Basyair7"));
    delay(DELAYMS);
  }

  for (int i = 0; i <= 66; i++) {
    lcd.setCursor(0, 1);
    lcd.print(ChrAnimated.Scroll_LCD_Left("Hello World!!..."));
    delay(DELAYMS);
  }
  ChrAnimated.Clear_Scroll_LCD_Left();

  for (int i = 13; i <= 24; i++) {
    lcd.setCursor(0, 0);
    lcd.print(ChrAnimated.Scroll_LCD_Right("Basyair7"));
    delay(DELAYMS);
  }
  ChrAnimated.Clear_Scroll_LCD_Right();

}
