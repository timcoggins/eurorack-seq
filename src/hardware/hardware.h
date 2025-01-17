#pragma once

#include <Arduino.h>
#include <RotaryEncoder.h>
#include <Adafruit_Protomatter.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MCP4725.h>

#include "../config.h"
#include "../engine/engine.h"

using namespace config;

namespace Hardware 
{
    static uint16_t color(uint8_t red, uint8_t green, uint8_t blue) {
        return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
    }

    #define COLOUR_RED                      color(255, 0, 0)
    #define COLOUR_RED_DIM                  color(7, 0, 0)
    #define COLOUR_GREEN                    color(0, 0, 16)
    #define COLOUR_CYAN                     color(0, 64, 64)
    #define COLOUR_BLUE                     color(0, 64, 0)
    #define COLOUR_PURPLE                   color(255, 255, 0)
    #define COLOUR_PURPLE_DARK              color(32, 32, 32)
    #define COLOUR_WHITE                    color(255, 255, 255)
    #define COLOUR_BLACK                    color(0, 0, 0)
    #define COLOUR_YELLOW                   color(64, 0, 16)
    #define COLOUR_GRAY_DARK                color(16, 16, 16)
    #define COLOUR_GRAY                     color(64, 64, 64)

    #define COLOUR_CURSOR                   COLOUR_BLUE
    #define COLOUR_CURSOR_OVERLAP           COLOUR_CYAN
    #define COLOUR_PLAYHEAD                 COLOUR_PURPLE
    #define COLOUR_NOTE_ACTIVE              COLOUR_RED
    #define COLOUR_NOTE_INACTIVE            COLOUR_RED_DIM
    #define COLOUR_GATE                     COLOUR_WHITE
    #define COLOUR_GATE_ACTIVE              COLOUR_PURPLE
    #define COLOUR_MOD                      COLOUR_YELLOW
    #define COLOUR_LOOP_BOUNDS_ACTIVE       COLOUR_GREEN
    #define COLOUR_LOOP_BOUNDS_INACTIVE     COLOUR_GRAY_DARK
    #define COLOUR_BACKGROUND               COLOUR_BLACK
    #define COLOUR_DIVIDER                  COLOUR_GRAY_DARK
    #define COLOUR_DIVIDER_GRID             COLOUR_GRAY
    #define COLOUR_SEQ                      COLOUR_GREEN
    #define COLOUR_SEQ_ACTIVE               COLOUR_WHITE

    /**
     * MATRIX DISPLAY 
     */
    struct Display
    {
        int selectedTrack = 0;

        void setup();

        void drawPixel(int16_t x, int16_t y, uint16_t color);
        void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
        void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

        void finishDraw();
        void clear();
    };

    struct LCD
    {
        void setup();
        void log(char* text);
    };

    struct Output
    {
        Adafruit_MCP4725 dac;

        void setup();
        void setNote(int channel, int note); 
        void setMod(int channel, int note); 
        void setGate(int channel, bool gate, bool accent);
        void setTuning();
        void setMultiGate(bool b0, bool b1, bool b2, bool b3);
    };

    struct Clock
    {
        int tempoValue = 0;
        int lastReading = 0;

        void setup();
        bool check();
    };

    /**
     * BUTTONS
     */
    struct Button 
    {

        int PIN;
        int lastButtonState = LOW;
        int state = LOW;
        int buttonState = LOW;

        unsigned long lastDebounceTime = 0;
        unsigned long debounceDelay = 50;

        void setup(int bPIN);
        void readDebounced();
        void read();
        int getState();
    };

    /**
     * ENCODER
     */
    struct Encoder 
    {
        // RotaryEncoder *enc = nullptr;
        RotaryEncoder enc = RotaryEncoder(ENC_PIN1, ENC_PIN2, RotaryEncoder::LatchMode::TWO03);
        int state = 0;

        void setup();
        // void checkPosition();
        int read();
    };

}
