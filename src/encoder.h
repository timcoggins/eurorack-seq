#include <Arduino.h> 
#include <RotaryEncoder.h>
#include "config.h"

#pragma once

namespace ui 
{

    RotaryEncoder *enc = nullptr;

    void checkPosition()
    {
        enc->tick(); // just call tick() to check the state.
    }

    struct Encoder 
    {
        int state = 0;

        void setup() 
        {
            enc = new RotaryEncoder(ENC_PIN1, ENC_PIN2, RotaryEncoder::LatchMode::TWO03);

            // register interrupt routine
            attachInterrupt(digitalPinToInterrupt(ENC_PIN1), checkPosition, CHANGE);   
            attachInterrupt(digitalPinToInterrupt(ENC_PIN2), checkPosition, CHANGE);
        }

        int read() 
        {
            static int pos = 0;
            int current = 0;

            enc->tick(); // just call tick() to check the state.
            int newPos = enc->getPosition();


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
    };
}