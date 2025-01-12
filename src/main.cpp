#include <Arduino.h>
#include <Wire.h>
#include "interface/interface.h"
#include "engine/engine.h"
#include "hardware/hardware.h"

using namespace config;

Engine::Set engine;
Hardware::Clock internalClock;
Hardware::Output output;
Interface::Ui ui;

void checkPosition()
{
    ui.encoder.enc.tick(); // just call tick() to check the state.
}

// ======================================================================
// SETUP
// ======================================================================
void setup() 
{
    Wire.begin(I2C_SDA, I2C_SCL);

    engine.setup();
    internalClock.setup();
    output.setup(); 
    ui.setup();
    
    attachInterrupt(digitalPinToInterrupt(ENC_PIN1), checkPosition, CHANGE);   
    attachInterrupt(digitalPinToInterrupt(ENC_PIN2), checkPosition, CHANGE);
}

// ======================================================================
// Main LOOP
// ======================================================================
void loop() 
{
    ui.scan();
    ui.handleControls(engine);

    if (internalClock.check()) 
    {
        engine.nextTick();
        
        for(int i = 0; i < MAX_TRACKS; i++)
        {
            output.setNote(i, engine.getNote(i));
            output.setMod(i, engine.getStep(i).mod);
            output.setGate(i, engine.getStep(i).active);
        }

        ui.handleDraw(engine);
    }
}


/**
 * FOR LATER
 */

// const uint8_t kUpdatePeriod = F_CPU / 32 / 8000 / 100;

// TODO FIND INTERRUPTS FOR THIS BOARD

// int counter = 0;
// Timer Interrupt for clocking
// ISR(TIMER2_COMPA_vect, ISR_NOBLOCK) {
//   if(counter < tempoValue) {
//     counter += 1;
//   } else {
//     seqEngine.nextTick();
//     out.update(seqEngine.getNote(0));
    
//     counter = 0;
//   }
// }


// For Setup
// TODO FIND INTERRUPTS FOR THIS BOARD

// sei();
// UCSR0B = 0;

// TCCR2A = _BV(WGM21);
// TCCR2B = 3;
// OCR2A = kUpdatePeriod - 1;
// TIMSK2 |= _BV(1);