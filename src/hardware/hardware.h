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
    static uint8_t rgbPins[]  = {4, 12, 13, 14, 15, 9};
    static uint8_t addrPins[] = {16, 17, 18, 10};
    static uint8_t clockPin   = 8;                         // Must be on same port as rgbPins
    static uint8_t latchPin   = 5;
    static uint8_t oePin      = 6;

    static Adafruit_Protomatter matrix(
        64,                                 // Width of matrix (or matrix chain) in pixels
        4,                                  // Bit depth, 1-6
        1, rgbPins,                         // # of matrix chains, array of 6 RGB pins for each
        4, addrPins,                        // # of address pins (height is inferred), array of pins
        clockPin, latchPin, oePin,          // Other matrix control pins
        false                               // No double-buffering here (see "doublebuffer" example)
    );   

    #define COLOUR_RED                      matrix.color565(255, 0, 0)
    #define COLOUR_RED_DIM                  matrix.color565(7, 0, 0)
    #define COLOUR_GREEN                    matrix.color565(0, 0, 16)
    #define COLOUR_CYAN                     matrix.color565(0, 64, 64)
    #define COLOUR_BLUE                     matrix.color565(0, 64, 0)
    #define COLOUR_PURPLE                   matrix.color565(255, 255, 0)
    #define COLOUR_PURPLE_DARK              matrix.color565(32, 32, 32)
    #define COLOUR_WHITE                    matrix.color565(255, 255, 255)
    #define COLOUR_BLACK                    matrix.color565(0, 0, 0)
    #define COLOUR_YELLOW                   matrix.color565(64, 0, 16)
    #define COLOUR_GRAY_DARK                matrix.color565(16, 16, 16)
    #define COLOUR_GRAY                     matrix.color565(64, 64, 64)

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
    
    static LiquidCrystal_I2C lcd(0x27,20,4);

    /**
     * MATRIX DISPLAY 
     */
    struct Display
    {

        int selectedTrack = 0;

        void setup();
        void drawDivider(int yOffset);
        void drawTrack(Engine::Track& track, int yOffset);
        void drawModTrack(Engine::Track& track, int yOffset);
        void drawGateTrack(Engine::Track& track, int yOffset);
        void drawSequenceLength(Engine::Track& currentTrack, int y);
        void drawSequenceSelector(Engine::Track& track, int yOffset);
        void drawLoopBounds(Engine::Track& track, int yOffset);
        void drawOutputVoltages(int note);
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
        void setGate(int channel, bool gate);
        void setTuning();
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

        unsigned long lastDebounceTime = 0;
        unsigned long debounceDelay = 50;

        void setup(int bPIN);
        int readDebounced();
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
