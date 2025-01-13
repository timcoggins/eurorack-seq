#include "hardware.h"

using namespace Hardware;

void Hardware::Button::setup(int bPIN) 
{
    PIN = bPIN;
    pinMode(PIN, INPUT);
}

void Hardware::Button::readDebounced() 
{
    int reading = digitalRead(PIN);    

    if (reading != lastButtonState) 
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) 
    {
        if (reading != buttonState) {
            buttonState = reading;

            if (buttonState == HIGH) {
                state = HIGH;
            }
        }
        else 
        {
            state = LOW;
        }
    }

    lastButtonState = reading;
}

void Hardware::Button::read() 
{
    Button::state = digitalRead(PIN);
}

int Hardware::Button::getState()
{
    return state;
}