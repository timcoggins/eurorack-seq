#include "hardware.h"

using namespace Hardware;

void Hardware::Button::setup(int bPIN) 
{
    PIN = bPIN;
    pinMode(PIN, INPUT);
}

int Hardware::Button::readDebounced() 
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

void Hardware::Button::read() 
{
    Button::state = digitalRead(PIN);
}

int Hardware::Button::getState()
{
    return state;
}