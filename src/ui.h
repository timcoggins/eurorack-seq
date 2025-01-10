#pragma once

#include <Arduino.h>
#include <RotaryEncoder.h>
#include "engine.h"
#include "button.h"
#include "display.h"


using namespace config;

namespace ui 
{

    Display display;

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
    Button btn9(B9_PIN);
    Button btn10(B10_PIN);

    


    // ==================================================================
    // UI COLOUR DEFS 
    // ==================================================================



    void checkPosition()
    {
        encoder->tick(); // just call tick() to check the state.
    }

    struct Ui 
    {
        // MODE currentMode = MODE::TRACK;

        int selectedTrack = 0;
        engine::Step copyBuffer[MAX_STEPS];
        int copyLength = 0;
        int page = 0;

        // ==================================================================
        // UI SETUP 
        // ==================================================================
        void setup() 
        {
            display.setup();

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
            btn9.setup();
            btn10.setup();

            // register interrupt routine
            attachInterrupt(digitalPinToInterrupt(ENC_PIN1), checkPosition, CHANGE);   
            attachInterrupt(digitalPinToInterrupt(ENC_PIN2), checkPosition, CHANGE);
        }

        void copyToBuffer(engine::Track& currentTrack)
        {
            engine::Sequence& currentSequence = currentTrack.seqs[currentTrack.currentSequence];
            int j = 0;
            for(int i = currentTrack.cursor; i < currentTrack.cursor + currentTrack.cursorLen; i++)
            {
                copyBuffer[j] = currentSequence.steps[i];
                j += 1;
            }
            copyLength = j;
        }

        void nextPage() {
            if(page == 1)
            {
                page = 0;
            }
            else
            {
                page = 1;
            }
        }


        // ==================================================================
        // RUNS ON EVERY LOOP 
        // ==================================================================
        void scan(engine::Engine& seq) 
        {
            engine::Track& currentTrack = seq.tracks[selectedTrack];
            engine::Sequence& currentSequence = currentTrack.seqs[currentTrack.currentSequence];

            int encoderButton = encoderBtn.read();

            int b1Pin = btn1.read();
            int b2Pin = btn2.read();
            int b3Pin = btn3.read();
            int b4Pin = btn4.read();
            int b5Pin = btn5.readDebounced();
            int b6Pin = btn6.readDebounced();
            int b7Pin = btn7.readDebounced();
            int b8Pin = btn8.readDebounced();
            int b9Pin = btn9.readDebounced();
            int b10Pin = btn10.readDebounced();

            if(b5Pin == HIGH)
            {
                currentSequence.toggleLoop();
            }
            if(b6Pin == HIGH)
            {
                nextPage();
            }
            if(b7Pin == HIGH)
            {
                copyToBuffer(currentTrack);
            }
            if(b8Pin == HIGH) 
            {
                int len = min(copyLength, currentTrack.cursorLen);
                currentSequence.paste(currentTrack.cursor, copyBuffer, len);
            }
            if(b9Pin == HIGH)
            {
                currentTrack.setBoundsToCursor();
            }
            if(b10Pin == HIGH)
            {
                currentTrack.jump(currentTrack.cursor);
            }


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

            if(encoderButton == LOW)    // REVERSE POLARITY FOR SOME REASON... 
            {
                if(state == 1) currentTrack.cursorLen += 1;
                else if(state == -1) currentTrack.cursorLen -= 1;
            }
            else if(b1Pin == HIGH) 
            {
                if(state == 1) currentSequence.transposeUp(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
                else if(state == -1) currentSequence.transposeDown(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
            }
            else if(b2Pin == HIGH) 
            {
                if(state == 1) currentSequence.mute(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
                else if(state == -1) currentSequence.unmute(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
            }
            else if(b3Pin == HIGH && b4Pin == HIGH) 
            {
                if(state == 1) 
                {
                    currentSequence.increaseStart();
                    currentSequence.increaseEnd();
                }
                if(state == -1) 
                {
                    currentSequence.decreaseStart();
                    currentSequence.decreaseEnd();
                }
            }
            else if(b3Pin == HIGH) 
            {
                if(state == 1) currentSequence.increaseStart();
                else if(state == -1) currentSequence.decreaseStart();
            }
            else if(b4Pin == HIGH) 
            {
                if(state == 1) currentSequence.increaseEnd();
                else if(state == -1) currentSequence.decreaseEnd();
            }
            // else if(b5Pin == HIGH) 
            // {
            //     currentSequence.invert(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
            // }
            // else if(b6Pin == HIGH) 
            // {
            //     currentSequence.reverse(cursor, cursor + cursorLen);
            // }
            // else if(b7Pin == HIGH) 
            // {
            //     currentSequence.randomize(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
            // }
            // else if(b8Pin == HIGH) 
            // {
            //     if(state == 1) currentTrack.incSequence();
            //     else if(state == -1) currentTrack.decSequence();
            //     // currentSequence.sustain(cursor, cursor + cursorLen);
            // }
            else if(state == 1)
            {
                if(currentTrack.cursor >= MAX_STEPS) return;
                currentTrack.cursor += 1;
            }
            else if(state == -1)
            {
                if(currentTrack.cursor <= 0) return;
                currentTrack.cursor -= 1;
            }

            update(seq);
        }

        void update(engine::Engine& seq) 
        {
            engine::Track& track = seq.tracks[selectedTrack];

            if(page == 0)
            {
                display.drawModTrack(track, 0);
                display.drawDivider(8);
                display.drawTrack(track, 9);
                display.drawDivider(25);
                display.drawGateTrack(track, 26);
                display.drawDivider(28);

                display.drawSequenceSelector(track);
                // display.drawSequenceLength(track);
            }
            else if(page == 1)
            {
                display.drawSequenceLength(track, 24);
            }

            display.finishDraw();
        }

        void selectNextTrack() {
            if(selectedTrack >= MAX_TRACKS - 1)
            {
                selectedTrack = 0;
            } else {
                selectedTrack += 1;
            }
        }
    };
}