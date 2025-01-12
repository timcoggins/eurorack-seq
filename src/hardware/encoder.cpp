#include "hardware.h"

using namespace Hardware;

void Hardware::Encoder::setup() 
{
    // enc = new RotaryEncoder(ENC_PIN1, ENC_PIN2, RotaryEncoder::LatchMode::TWO03);

    // register interrupt routine
    // attachInterrupt(digitalPinToInterrupt(ENC_PIN1), enc->tick(), CHANGE);   
    // attachInterrupt(digitalPinToInterrupt(ENC_PIN2), Encoder::checkPosition, CHANGE);
}

int Hardware::Encoder::read() 
{
    static int pos = 0;
    int current = 0;

    enc.tick(); // just call tick() to check the state.
    int newPos = enc.getPosition();


    if (pos == newPos) 
    {
        state = 0;
        return 0;
    }

    if (pos > newPos) 
    {
        current = 1;
    } 
    else 
    {
        current -= 1;
    }

    pos = newPos;

    state = current;

    return current;
}