#pragma once

#include <Adafruit_Protomatter.h>
#include <RotaryEncoder.h>
#include "engine.h"
#include "button.h"


using namespace config;

namespace ui 
{
    RotaryEncoder *encoder = nullptr;

    Button encoderBtn(ENC_BUTTON_PIN);
    Button btn1(B1_PIN);
    Button btn2(B2_PIN);
    Button btn3(B3_PIN);
    Button btn4(B4_PIN);
    Button btn5(B5_PIN);
    Button btn6(B6_PIN);
    Button btn7(B7_PIN);
    Button btn8(B8_PIN);

    Adafruit_Protomatter matrix(
        64,                                   // Width of matrix (or matrix chain) in pixels
        4,                                    // Bit depth, 1-6
        1, rgbPins,                           // # of matrix chains, array of 6 RGB pins for each
        4, addrPins,                          // # of address pins (height is inferred), array of pins
        clockPin, latchPin, oePin,            // Other matrix control pins
        false                                 // No double-buffering here (see "doublebuffer" example)
    );                               

    // ==================================================================
    // UI COLOUR DEFS 
    // ==================================================================

    #define COLOUR_RED matrix.color565(255, 0, 0)
    #define COLOUR_RED_DIM matrix.color565(7, 0, 0)
    #define COLOUR_GREEN matrix.color565(0, 0, 20)
    #define COLOUR_BLUE matrix.color565(0, 40, 0)
    #define COLOUR_PURPLE matrix.color565(255, 255, 0)
    #define COLOUR_WHITE matrix.color565(255, 255, 255)
    #define COLOUR_BLACK matrix.color565(0, 0, 0)
    #define COLOUR_YELLOW matrix.color565(0, 0, 0)

    void checkPosition()
    {
        encoder->tick(); // just call tick() to check the state.
    }


    struct Ui 
    {
        int cursor = DEFAULT_CURSOR_POS;
        int cursorLen = DEFAULT_CURSOR_LENGTH;
        int track = 0;
        
        // ==================================================================
        // UI SETUP 
        // ==================================================================
        void setup() 
        {
            encoder = new RotaryEncoder(ENC_PIN1, ENC_PIN2, RotaryEncoder::LatchMode::TWO03);

            encoderBtn.setup();
            btn1.setup();
            btn2.setup();
            btn3.setup();
            btn4.setup();
            btn5.setup();
            btn6.setup();
            btn7.setup();
            btn8.setup();

            // register interrupt routine
            attachInterrupt(digitalPinToInterrupt(ENC_PIN1), checkPosition, CHANGE);   
            attachInterrupt(digitalPinToInterrupt(ENC_PIN2), checkPosition, CHANGE);

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

        // ==================================================================
        // RUNS ON EVERY LOOP 
        // ==================================================================
        void scan(engine::Engine& seq) 
        {
            engine::Track& currentTrack = seq.tracks[track];
            engine::Sequence& currentSequence = currentTrack.seqs[currentTrack.currentSequence];

            int encoderButton = encoderBtn.read();
            int b1Pin = btn1.read();
            int b2Pin = btn2.read();
            int b3Pin = btn3.read();
            int b4Pin = btn4.read();
            int b5Pin = btn5.read();
            int b6Pin = btn6.read();
            int b7Pin = btn7.read();
            int b8Pin = btn8.read();

            static int pos = 0;
            int state = 0;

            encoder->tick(); // just call tick() to check the state.
            int newPos = encoder->getPosition();

            if (pos == newPos) return;

            if (pos > newPos) 
            {
                state = 1;
            } 
            else 
            {
                state -= 1;
            }
            pos = newPos;


            if(encoderButton == LOW) 
            {
                if(state == 1) cursorLen += 1;
                else if(state == -1) cursorLen -= 1;
            }
            else if(b1Pin == HIGH) 
            {
                if(state == 1) currentTrack.incSequence();
                else if(state == -1) currentTrack.decSequence();
                drawSequenceLength(seq);
            }
            else if(b2Pin == HIGH) 
            {
                if(state == 1) currentSequence.transposeUp(cursor, cursor + cursorLen);
                else if(state == -1) currentSequence.transposeDown(cursor, cursor + cursorLen);
            }
            else if(b3Pin == HIGH && b4Pin == HIGH) 
            {
                if(state == 1) 
                {
                    currentSequence.increaseStart();
                    currentSequence.increaseEnd();
                    drawSequenceLength(seq);
                }
                if(state == -1) 
                {
                    currentSequence.decreaseStart();
                    currentSequence.decreaseEnd();
                    drawSequenceLength(seq);
                }
            }
            else if(b3Pin == HIGH) 
            {
                if(state == 1) currentSequence.increaseStart();
                else if(state == -1) currentSequence.decreaseStart();
                drawSequenceLength(seq);
            }
            else if(b4Pin == HIGH) 
            {
                if(state == 1) currentSequence.increaseEnd();
                else if(state == -1) currentSequence.decreaseEnd();
                drawSequenceLength(seq);
            }
            else if(b5Pin == HIGH) 
            {
                currentSequence.invert(cursor, cursor + cursorLen);
            }
            else if(b6Pin == HIGH) 
            {
                currentSequence.reverse(cursor, cursor + cursorLen);
            }
            else if(b7Pin == HIGH) 
            {
                currentSequence.randomize(cursor, cursor + cursorLen);
            }
            else if(b8Pin == HIGH) 
            {
                currentSequence.sustain(cursor, cursor + cursorLen);
            }
            else if(state == 1)
            {
                if(cursor >= MAX_STEPS) return;
                cursor += 1;
            }
            else if(state == -1)
            {
                if(cursor <= 0) return;
                cursor -= 1;
            }

            update(seq);
        }

        void update(engine::Engine& seq) 
        {
            drawTrack(seq);
            drawGateTrack(seq);
            drawSequenceSelector(seq);
            finishDraw();
        }


        // ==================================================================
        // DRAW THE CURRENT TRACK 
        // ==================================================================
        void drawTrack(engine::Engine& seq) 
        {
            engine::Track& currentTrack = seq.tracks[track];
            engine::Sequence& currentSequence = currentTrack.seqs[currentTrack.currentSequence];

            // Draw Track
            for(int i = 0; i < MAX_STEPS; i++) 
            {
                for(int j = 0; j < MAX_NOTE_VALUE; j++) 
                {

                    // Note
                    if((MAX_NOTE_VALUE - 1 - currentSequence.steps[i].note) == j) 
                    {
                        if(currentSequence.steps[i].active == true) 
                        {
                            matrix.drawPixel(i, j, COLOUR_RED);
                        } 
                        else 
                        {
                            matrix.drawPixel(i, j, COLOUR_RED_DIM);
                        }
                    } 
                    else 
                    {
                        // Playhead
                        if (i == currentTrack.step) 
                        {
                            matrix.drawPixel(i, j, COLOUR_WHITE);
                        } 
                        // Cursor
                        else if(i == cursor) {
                            matrix.drawPixel(i, j, COLOUR_BLUE);
                        } 
                        // Selection from Cursor
                        else if (i > cursor && i < cursor + cursorLen) 
                        {
                            matrix.drawPixel(i, j, COLOUR_BLUE);
                        } 
                        // Start Mask
                        else if (i < currentSequence.start) 
                        {
                            matrix.drawPixel(i, j, COLOUR_BLACK);
                        } 
                        // End Mask
                        else if (i > currentSequence.end) 
                        {
                            matrix.drawPixel(i, j, COLOUR_BLACK);
                        } 
                        // Void 
                        else 
                        {
                            matrix.drawPixel(i, j, COLOUR_GREEN);
                        }
                    }
                }
            }
        } 

        // ==================================================================
        // DRAW GATE TRACK 
        // ==================================================================
        void drawGateTrack(engine::Engine& seq) 
        {
            engine::Track& currentTrack = seq.tracks[track];
            engine::Sequence& currentSequence = currentTrack.seqs[currentTrack.currentSequence];

            for(int i = 0; i < MAX_STEPS; i++) 
            {
                if(currentSequence.steps[i].active == true) 
                {
                    if(currentTrack.step == i) 
                    {
                        matrix.drawLine(i, 20, i, 21, COLOUR_PURPLE);
                    } 
                    else 
                    {
                        matrix.drawLine(i, 20, i, 21, COLOUR_WHITE);
                    }
                } 
                else 
                {
                    matrix.drawLine(i, 20, i, 21, COLOUR_BLACK);
                }
            }  
        }

        // ==================================================================
        // DRAW SEQUENCE LENGTH 
        // ==================================================================
        void drawSequenceLength(engine::Engine& seq)
        {

            engine::Track& currentTrack = seq.tracks[track];
            engine::Sequence& currentSequence = currentTrack.seqs[currentTrack.currentSequence];

            matrix.fillRect(26, 24, 11, 8, COLOUR_BLACK);

            matrix.setCursor(26, 24);
            matrix.print(currentSequence.end - currentSequence.start);
        }
        
        // ==================================================================
        // DRAW SEQUENCE SELECTOR 
        // ==================================================================
        void drawSequenceSelector(engine::Engine& seq) 
        {
            for(int i = 0; i < MAX_SEQUENCES; i++) 
            {
                if(seq.tracks[track].currentSequence < i) 
                {
                    matrix.drawLine(i, 28, i, 31, COLOUR_GREEN);
                }
                else 
                {
                    matrix.drawLine(i, 28, i, 31, COLOUR_WHITE);
                }
            }
        }

        // ==================================================================
        // COMMIT DRAWING 
        // ==================================================================
        void finishDraw() 
        {
            matrix.show();
        }

    };
}