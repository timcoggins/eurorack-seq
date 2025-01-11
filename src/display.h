#pragma once
#include <Adafruit_Protomatter.h>
#include "engine.h"
#include "config.h"

namespace ui 
{

    Adafruit_Protomatter matrix(
        64,                                     // Width of matrix (or matrix chain) in pixels
        4,                                      // Bit depth, 1-6
        1, rgbPins,                             // # of matrix chains, array of 6 RGB pins for each
        4, addrPins,                            // # of address pins (height is inferred), array of pins
        clockPin, latchPin, oePin,              // Other matrix control pins
        false                                   // No double-buffering here (see "doublebuffer" example)
    );   

    class Display
    {

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


        int selectedTrack = 0;

        public:

            // ==================================================================
            // DISPLAY SETUP 
            // ==================================================================
            void setup()
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

            // ==================================================================
            // DRAW DIVIDER 
            // ==================================================================
            void drawDivider(int yOffset)
            {
                matrix.drawLine(0, yOffset, MAX_STEPS - 1, yOffset, COLOUR_DIVIDER);
                for(int i = 0; i < MAX_STEPS; i += SHOW_GRID_EVERY)
                {
                    matrix.drawPixel(i, yOffset, COLOUR_DIVIDER_GRID);
                }
            }



            // ==================================================================
            // DRAW PITCH TRACK 
            // ==================================================================
            void drawTrack(engine::Track& track, int yOffset) 
            {
                engine::Sequence& currentSequence = track.seqs[track.currentSequence];

                int t = 0;

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
                                matrix.drawPixel(i, j + yOffset, COLOUR_NOTE_ACTIVE);
                            } 
                            else 
                            {
                                matrix.drawPixel(i, j + yOffset, COLOUR_NOTE_INACTIVE);
                            }
                        } 
                        else 
                        {
                            // Playhead
                            if (i == track.step) 
                            {
                                matrix.drawPixel(i, j + yOffset, COLOUR_PLAYHEAD);
                            } 
                            // Cursor in Bounds
                            else if(i == track.cursor && selectedTrack == t && i >= currentSequence.start && i <= currentSequence.end) {
                                matrix.drawPixel(i, j + yOffset, COLOUR_CURSOR_OVERLAP);
                            } 
                            // Cursor
                            else if(i == track.cursor && selectedTrack == t) {
                                matrix.drawPixel(i, j + yOffset, COLOUR_CURSOR);
                            } 
                            // Selection from Cursor in Bounds
                            else if (i > track.cursor && i < track.cursor + track.cursorLen && selectedTrack == t && i >= currentSequence.start && i <= currentSequence.end) 
                            {
                                matrix.drawPixel(i, j + yOffset, COLOUR_CURSOR_OVERLAP);
                            } 
                            // Selection from Cursor
                            else if (i > track.cursor && i < track.cursor + track.cursorLen && selectedTrack == t) 
                            {
                                matrix.drawPixel(i, j + yOffset, COLOUR_CURSOR);
                            } 
                            // // Start Mask
                            // else if (i >= currentSequence.start && i <= currentSequence.end) 
                            // {
                            //     if(currentSequence.enableLoop)
                            //     {
                            //         matrix.drawPixel(i, j + yOffset, COLOUR_LOOP_BOUNDS_ACTIVE);
                            //     } 
                            //     else
                            //     {
                            //         matrix.drawPixel(i, j + yOffset, COLOUR_LOOP_BOUNDS_INACTIVE);
                            //     }
                            // } 
                            // Void 
                            else 
                            {
                                matrix.drawPixel(i, j + yOffset, COLOUR_BACKGROUND);
                            }
                        }
                    }
                }
            } 

        
            // ==================================================================
            // DRAW MODULATION TRACK 
            // ==================================================================
            void drawModTrack(engine::Track& track, int yOffset)
            {
                    engine::Sequence& currentSequence = track.seqs[track.currentSequence];

                    for(int i = 0; i < MAX_STEPS; i++) 
                    {
                        for(int j = 0; j < MAX_MOD_VALUE; j++) 
                        {
                            if((MAX_MOD_VALUE - 1 - currentSequence.steps[i].mod) <= j) 
                            {
                                matrix.drawPixel(i, j + yOffset, COLOUR_MOD);
                            }
                            else
                            {
                                matrix.drawPixel(i, j + yOffset, COLOUR_BACKGROUND);
                            }
                        }
                    }
            }

            // ==================================================================
            // DRAW GATE TRACK 
            // ==================================================================
            void drawGateTrack(engine::Track& track, int yOffset) 
            {
                engine::Sequence& currentSequence = track.seqs[track.currentSequence];

                for(int i = 0; i < MAX_STEPS; i++) 
                {
                    if(currentSequence.steps[i].active == true) 
                    {
                        if(track.step == i) 
                        {
                            matrix.drawLine(i, yOffset, i, yOffset + 1, COLOUR_GATE_ACTIVE);
                        } 
                        else 
                        {
                            matrix.drawLine(i, yOffset, i, yOffset + 1, COLOUR_GATE);
                        }
                    } 
                    else 
                    {
                        matrix.drawLine(i, yOffset, i, yOffset + 1, COLOUR_BACKGROUND);
                    }
                }  
            }

            // ==================================================================
            // DRAW SEQUENCE LENGTH 
            // ==================================================================
            void drawSequenceLength(engine::Track& currentTrack, int y)
            {
                engine::Sequence& currentSequence = currentTrack.seqs[currentTrack.currentSequence];

                matrix.fillRect(26, y, 11, 8, COLOUR_BACKGROUND);

                matrix.setCursor(26, y);
                matrix.setTextSize(0);
                matrix.print(currentSequence.end - currentSequence.start);
            }
        
            // ==================================================================
            // DRAW SEQUENCE SELECTOR 
            // ==================================================================
            void drawSequenceSelector(engine::Track& track, int yOffset) 
            {
                for(int i = 0; i < MAX_SEQUENCES; i++) 
                {
                    if(track.currentSequence < i) 
                    {
                        matrix.drawLine(i, yOffset, i, yOffset + 1, COLOUR_SEQ);
                    }
                    else 
                    {
                        matrix.drawLine(i, yOffset, i, yOffset + 1, COLOUR_SEQ_ACTIVE);
                    }
                }
            }

            /**
             * DRAW LOOP BOUNDS
             */
            void drawLoopBounds(engine::Track& track, int yOffset)
            {
                engine::Sequence& currentSequence = track.seqs[track.currentSequence];

                for(int i = 0; i < MAX_STEPS; i++) 
                {
                    if(i >= currentSequence.start && i <= currentSequence.end) 
                    {
                        matrix.drawLine(i, yOffset, i, yOffset + 1, currentSequence.enableLoop ? COLOUR_LOOP_BOUNDS_ACTIVE : COLOUR_LOOP_BOUNDS_INACTIVE);
                    }
                    else 
                    {
                        matrix.drawLine(i, yOffset, i, yOffset + 1, COLOUR_BACKGROUND);
                    }
                }
            }

            
            // ==================================================================
            // DRAW OUTPUT VOLTAGES 
            // ==================================================================
            void drawOutputVoltages(int note) {
                for(int i = 0; i < 16; i += 1) 
                {
                    matrix.fillRect(i * 4 , 0, i * 4 + 2, i * 2, note == i ? COLOUR_GREEN : COLOUR_BLUE);
                }
            }

            // ==================================================================
            // COMMIT DRAWING 
            // ==================================================================
            void finishDraw() 
            {
                matrix.show();
            }

            // ==================================================================
            // COMMIT DRAWING 
            // ==================================================================
            void clear() 
            {
                matrix.fillRect(0, 0, 64, 32, COLOUR_BACKGROUND);
            }
    };
}