
#include <Arduino.h> 
#include <LiquidCrystal_I2C.h>
#include "config.h"


#pragma once

namespace ui 
{

    LiquidCrystal_I2C lcd(0x27,20,4);

    struct Lcd
    {

        void setup() 
        {
            lcd.init();                      // initialize the lcd 
            lcd.backlight();
            lcd.setCursor(0,0);
            lcd.print("v0.1");
        }

        void log(char* text)
        {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(text);
        }
    };
}