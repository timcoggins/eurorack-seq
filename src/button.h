#include <Arduino.h> 
#include "config.h"

#pragma once

struct Button 
{

    int PIN;
    int lastButtonState = LOW;
    int state = LOW;

    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

    Button(int bPIN) 
    {
        PIN = bPIN;
    }

    void setup() 
    {
        pinMode(PIN, INPUT);
    }

    int readDebounced() 
    {
        int reading = digitalRead(PIN);    

        if (reading != lastButtonState) 
        {
            lastDebounceTime = millis();
        }

        if ((millis() - lastDebounceTime) > debounceDelay) 
        {
            if (reading != state) {
                state = reading;
                if (state == HIGH) {
                    lastButtonState = reading;
                    return HIGH;
                }
            }
        }

        lastButtonState = reading;
        return LOW;
    }

    int read() 
    {
        state = digitalRead(PIN);
        return state;
    }
};