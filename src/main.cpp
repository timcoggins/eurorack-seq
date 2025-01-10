#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include "ui.h"
#include "engine.h"

using namespace config;

engine::Engine seqEngine;

ui::Ui interface;

Adafruit_MCP4725 dac;

int tempoValue = 0;

// TODO FIND INTERRUPTS FOR THIS BOARD

int counter = 0;
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

const uint8_t kUpdatePeriod = F_CPU / 32 / 8000 / 100;

void setup() 
{
    Wire.begin(DAC_SDA, DAC_SCL);
    dac.begin(DAC_ADDR);
    pinMode(GATE_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);

    seqEngine.randomiseAll();

    interface.setup();

    pinMode(TEMPO_PIN, INPUT);


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

int lastReading = 0; 
int track = 0; // TODO

void loop() 
{
    interface.scan(seqEngine);

    if (millis () - lastReading >= tempoValue) 
    {
        seqEngine.nextTick();

        dac.setVoltage(seqEngine.getStep(track).note * (DAC_VALUES / MAX_NOTE_VALUE), false);
        digitalWrite(GATE_PIN, seqEngine.getStep(track).active);
        digitalWrite(CLOCK_PIN, HIGH);

        interface.update(seqEngine);

        lastReading = millis();
        tempoValue = analogRead(A1);
    }

    if (millis () - lastReading >= tempoValue / 2)
    {
        digitalWrite(CLOCK_PIN, LOW);
    } 
}

