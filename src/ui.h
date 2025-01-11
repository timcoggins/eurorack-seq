#pragma once

#include <Arduino.h>

#include "engine.h"
#include "button.h"
#include "encoder.h"
#include "display.h"
#include "lcd.h"


using namespace config;

namespace ui 
{

    Display display;
    Lcd lcdD;

    Encoder encoder;
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

    struct Ui 
    {
        engine::Step copyBuffer[MAX_STEPS];

        int selectedTrack = 0;
        int copyLength = 0;
        int page = 0;

        /**
         * Setup hardware
         */
        void setup() 
        {
            display.setup();
            lcdD.setup();

            encoder.setup();
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
        }

        /**
         * Check the hardware and update each items state
         */
        void scan() 
        {
            encoder.read();
            encoderBtn.read();

            btn1.read();
            btn2.read();
            btn3.read();
            btn4.read();
            btn5.readDebounced();
            btn6.readDebounced();
            btn7.read();
            btn8.readDebounced();
            btn9.readDebounced();
            btn10.readDebounced();
        }

        /**
         * Copies the selected part of a sequence to the temporary buffer
         */
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

        /**
         * Selects the next track
         */
        void selectNextTrack() {
            if(selectedTrack >= MAX_TRACKS - 1)
            {
                selectedTrack = 0;
            } else {
                selectedTrack += 1;
            }
        }

        /**
         * Moves to the next menu page
         */
        void nextPage() {
            if(page == MAX_UI_PAGES - 1)
            {
                page = 0;
            }
            else
            {
                page += 1;
            }
            display.clear();
        }


        /**
         * Route controls based on which page we are on
         */
        void handleControls(engine::Engine& seq)
        {

            // Can switch between pages on every page
            if(btn8.state == HIGH)
            {
                nextPage();
            }

            handleMainControls(seq);

            // switch(page)
            // {
            //     case 0:
            //         handleControls(seq);
            //         break;
            //     default:
            //         break; 
            // }
        }


        /**
         * Handle Main Track Controls 
         */
        void handleMainControls(engine::Engine& seq)
        {
            engine::Track& currentTrack = seq.tracks[selectedTrack];
            engine::Sequence& currentSequence = currentTrack.seqs[currentTrack.currentSequence];

            // BUTTON COMBOS
            if(btn1.state == HIGH && btn2.state == HIGH) 
            {
                if(encoder.state == 1) currentSequence.randomizeNotes(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
                else if (encoder.state == -1) currentSequence.randomizeGates(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
            }
            else if(btn1.state == HIGH && btn3.state == HIGH) 
            {
                if(encoder.state == 1) currentSequence.increaseStart();
                else if(encoder.state == -1) currentSequence.decreaseStart();
            }
            else if(btn1.state == HIGH && btn4.state == HIGH) 
            {
                if(encoder.state == 1) currentSequence.increaseEnd();
                else if(encoder.state == -1) currentSequence.decreaseEnd();
            }
            else if(btn3.state == HIGH && btn4.state == HIGH) 
            {
                if(encoder.state == 1) 
                {
                    currentSequence.increaseStart();
                    currentSequence.increaseEnd();
                }
                if(encoder.state == -1) 
                {
                    currentSequence.decreaseStart();
                    currentSequence.decreaseEnd();
                }
            }

            // WITH ENCODER
            else if(btn1.state == HIGH) 
            {
                if(encoder.state == 1) 
                {
                    currentSequence.transposeUp(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
                }
                else if(encoder.state == -1) 
                {
                    currentSequence.transposeDown(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
                }
            }
            else if(btn2.state == HIGH) 
            {
                if(encoder.state == 1) currentSequence.mute(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
                else if(encoder.state == -1) currentSequence.unmute(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
            }
            else if(btn6.state == HIGH) 
            {
                if(encoder.state == 1) currentTrack.incSequence();
                else if(encoder.state == -1) currentTrack.decSequence();
            }
            else if(btn7.state == HIGH) 
            {
                if(encoder.state == 1) currentTrack.incSequence();
                else if(encoder.state == -1) currentTrack.decSequence();
            }
            else if(encoderBtn.state == LOW)    // REVERSE POLARITY FOR SOME REASON... 
            {
                if(encoder.state == 1) currentTrack.cursorLen += 1;
                else if(encoder.state == -1) currentTrack.cursorLen -= 1;
            }
            else if(encoder.state == 1)
            {
                if(currentTrack.cursor >= MAX_STEPS) return;
                currentTrack.cursor += 1;
            }
            else if(encoder.state == -1)
            {
                if(currentTrack.cursor <= 0) return;
                currentTrack.cursor -= 1;
            }
            else if(btn5.state == HIGH)
            {
                currentSequence.toggleLoop();
            }
            else if(btn6.state == HIGH) 
            {
                currentSequence.sustain(currentTrack.cursor, currentTrack.cursorLen);
            }
            else if(btn7.state == HIGH)
            {
                currentTrack.jump(currentTrack.cursor);
            }
            else if(btn9.state == HIGH)
            {
                copyToBuffer(currentTrack);
            }
            else if(btn10.state == HIGH)
            {   
                int len = min(copyLength, currentTrack.cursorLen);
                currentSequence.paste(currentTrack.cursor, copyBuffer, len);
            }

            update(seq);
        }

        /**
         * Update the displays
         */
        void update(engine::Engine& seq) 
        {
            engine::Track& track = seq.tracks[selectedTrack];

            if(page == 0)
            {
                display.drawLoopBounds(track, 0);
                display.drawDivider(2);
                display.drawTrack(track, 3);
                display.drawDivider(19);
                display.drawDivider(25);
                display.drawGateTrack(track, 26);
                display.drawDivider(28);

                display.drawSequenceSelector(track, 30);
            }
            else if(page == 1)
            {
                display.drawOutputVoltages(seq.getNote(0));

            }
            else if(page == 2)
            {
                display.drawModTrack(track, 0);
                display.drawDivider(8);
                display.drawTrack(track, 9);
                display.drawDivider(25);
                display.drawGateTrack(track, 26);
                display.drawSequenceLength(track, 26);
            }

            display.finishDraw();
        }
    };
}