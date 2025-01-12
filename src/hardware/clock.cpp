#include "hardware.h"

using namespace Hardware;

void Hardware::Clock::setup()
{
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(TEMPO_PIN, INPUT);
}

bool Hardware::Clock::check()
{
    if(millis () - lastReading >= tempoValue) 
    {
        digitalWrite(CLOCK_PIN, HIGH);

        lastReading = millis();
        tempoValue = 4096 - analogRead(TEMPO_PIN);
        tempoValue = sqrt(tempoValue) * 4;

        return true;
    }

    if (millis () - lastReading >= tempoValue / 2)
    {
        digitalWrite(CLOCK_PIN, LOW);
    } 

    return false;
}