#include "hardware.h"

using namespace Hardware;

void Hardware::Output::setup() 
{
    dac.begin(DAC_ADDR);
    pinMode(GATE_PIN, OUTPUT);
}

void Hardware::Output::setNote(int channel, int note)
{
    dac.setVoltage(note * (DAC_VALUES / MAX_NOTE_VALUE), false);
} 

void Hardware::Output::setMod(int channel, int note)
{

}

void Hardware::Output::setGate(int channel, bool gate)
{
    digitalWrite(GATE_PIN, gate);
}

void Hardware::Output::setTuning()
{

}
