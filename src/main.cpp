#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include "interface/interface.h"
#include "engine/engine.h"
#include "hardware/hardware.h"

using namespace config;

Adafruit_MCP4725 dac;

Engine::Set engine;

Interface::Ui ui;


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

// ======================================================================
// SETUP
// ======================================================================

// const uint8_t kUpdatePeriod = F_CPU / 32 / 8000 / 100;

void checkPosition()
{
    ui.encoder.enc.tick(); // just call tick() to check the state.
}

void setup() 
{
    Wire.begin(DAC_SDA, DAC_SCL);
    dac.begin(DAC_ADDR);

    pinMode(GATE_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);

    engine.randomiseAll();

    ui.setup();

    pinMode(TEMPO_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(ENC_PIN1), checkPosition, CHANGE);   
    attachInterrupt(digitalPinToInterrupt(ENC_PIN2), checkPosition, CHANGE);


    // TODO FIND INTERRUPTS FOR THIS BOARD

    // sei();
    // UCSR0B = 0;

    // TCCR2A = _BV(WGM21);
    // TCCR2B = 3;
    // OCR2A = kUpdatePeriod - 1;
    // TIMSK2 |= _BV(1);
}

// ======================================================================
// Main LOOP
// ======================================================================

int tempoValue = 0;
int lastReading = 0;
int track = 0;

void loop() 
{
    ui.scan();
    ui.handleControls(engine);

    if (millis () - lastReading >= tempoValue) 
    {
        engine.nextTick();

        dac.setVoltage(engine.getNote(track) * (DAC_VALUES /MAX_NOTE_VALUE), false);
        digitalWrite(GATE_PIN, engine.getStep(track).active);
        digitalWrite(CLOCK_PIN, HIGH);

        ui.handleDraw(engine);

        lastReading = millis();
        tempoValue = 4096 - analogRead(TEMPO_PIN);
        tempoValue = sqrt(tempoValue) * 4;
    }

    if (millis () - lastReading >= tempoValue / 2)
    {
        digitalWrite(CLOCK_PIN, LOW);
    } 
}


