#include "hardware.h"

using namespace Hardware;

LiquidCrystal_I2C lcd(0x27,20,4);

void Hardware::LCD::setup() 
{
    lcd.init();                      // initialize the lcd 
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("v0.1");
}

void Hardware::LCD::log(char* text)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(text);
}