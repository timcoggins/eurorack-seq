#include "hardware.h"
#include <Adafruit_MCP4728.h>

using namespace Hardware;

Adafruit_MCP4728 mcp;

const int latchPin = 12;
const int clockPin = 11;
const int dataPin = 13;

void Hardware::Output::setup() 
{
    // dac.begin(DAC_ADDR);
    mcp.begin(0x60);

    pinMode(latchPin, OUTPUT);

    pinMode(dataPin, OUTPUT);

    pinMode(clockPin, OUTPUT);
}

void Hardware::Output::setNote(int channel, int note)
{
    // dac.setVoltage(note * (DAC_VALUES / MAX_NOTE_VALUE), false);
    mcp.setChannelValue(MCP4728_CHANNEL_A, note * (DAC_VALUES / MAX_NOTE_VALUE));
} 

void Hardware::Output::setMod(int channel, int note)
{
    mcp.setChannelValue(MCP4728_CHANNEL_D, note * (DAC_VALUES / MAX_NOTE_VALUE));
}

void Hardware::Output::setGate(int channel, bool gate, bool accent)
{
    if(gate == true && accent == true)
    {
        mcp.setChannelValue(MCP4728_CHANNEL_B, 2048);
    } 
    else if (gate == true)
    {
        mcp.setChannelValue(MCP4728_CHANNEL_B, 1024);
    }
    else
    {
        mcp.setChannelValue(MCP4728_CHANNEL_B, 0);
    }
}

void Hardware::Output::setTuning()
{

}

void Hardware::Output::setMultiGate(bool b0, bool b1, bool b2, bool b3) {
  digitalWrite(latchPin, LOW);
  
  byte newData = (false << 7) | (false << 6) | (false << 5) | (false << 4) |
              (b0 << 3) | (b1 << 2) | (b2 << 1) | b3;
              
  shiftOut(dataPin, clockPin, MSBFIRST, newData);
  digitalWrite(latchPin, HIGH);
}