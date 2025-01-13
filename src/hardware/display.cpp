#include "hardware.h"

using namespace Hardware;

static uint8_t rgbPins[]  = {4, 12, 13, 14, 15, 9};
static uint8_t addrPins[] = {16, 17, 18, 10};
static uint8_t clockPin   = 8;                         // Must be on same port as rgbPins
static uint8_t latchPin   = 5;
static uint8_t oePin      = 6;

Adafruit_Protomatter matrix(
    64,                                 // Width of matrix (or matrix chain) in pixels
    4,                                  // Bit depth, 1-6
    1, rgbPins,                         // # of matrix chains, array of 6 RGB pins for each
    4, addrPins,                        // # of address pins (height is inferred), array of pins
    clockPin, latchPin, oePin,          // Other matrix control pins
    false                               // No double-buffering here (see "doublebuffer" example)
);   


void Hardware::Display::setup()
{
    Serial.begin(9600);

    // Initialize matrix...
    ProtomatterStatus status = matrix.begin();
    Serial.print("Protomatter begin() status: ");
    Serial.println((int)status);
    if(status != PROTOMATTER_OK) 
    {
        // DO NOT CONTINUE if matrix setup encountered an error.
        for(;;);
    }
}

void Hardware::Display::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    matrix.drawPixel(x, y, color);
}

void Hardware::Display::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    matrix.drawLine(x0, y0, x1, y1, color);
}

void Hardware::Display::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    matrix.fillRect(x, y, w, h, color);
}

void Hardware::Display::finishDraw() 
{
    matrix.show();
}

void Hardware::Display::clear() 
{
    matrix.fillRect(0, 0, 64, 32, COLOUR_BACKGROUND);
}