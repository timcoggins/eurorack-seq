#include <Arduino.h>
#include "config.h"

#pragma once

using namespace config;

namespace engine 
{

    // ======================================================================
    // STEP 
    // ======================================================================
    struct Step 
    {
        int note = 1;
        int length = 1;
        bool active = true;
        int mod = 3;
    };

    // ======================================================================
    // SEQUENCE 
    // ======================================================================
    struct Sequence 
    {
        std::array<Step, MAX_STEPS> steps;
        
        int start = DEFAULT_START_POS;
        int end = DEFAULT_END_POS - 1;

        bool enableLoop = true;

        /**
         * Toggles the internal sequence loop
         */
        void toggleLoop()
        {
            enableLoop = !enableLoop;
        }

        /**
         * Sets the start point of the loop
         */
        void setStart(int pos) 
        {
            if (pos >= MAX_STEPS) return;
            if (pos >= end) return;
            if (pos < 0) return;
            start = pos;
        }

        /**
         * Increase the loop start point by one step
         */
        void increaseStart() 
        {
            setStart(start + 1);
        }

        /**
         * Decrease the loop start point by one step
         */
        void decreaseStart() 
        {
            setStart(start - 1);
        }

        /**
         * Sets the end point of the loop
         */
        void setEnd(int pos) 
        {
            if (pos > MAX_STEPS) return;
            if (pos <= start) return;
            end = pos;
        }

        /**
         * Increase the loop end point by one step
         */
        void increaseEnd() 
        {
            setEnd(end + 1);
        }

        /**
         * Decrease the loop end point by one step
         */
        void decreaseEnd() 
        {
            setEnd(end - 1);
        }

        /**
         * Randomise all parameters of the selected steps
         */
        void randomize(int start_selection, int end_selection) 
        {
            int s = max(start_selection, 0);
            int e = min(end_selection, MAX_STEPS);
            for(int i = s; i < e; i++) 
            {
                steps[i].note = random(RANDOM_MIN_NOTE, RANDOM_MAX_NOTE);
                steps[i].length = random(RANDOM_MIN_NOTE_LENGTH, RANDOM_MAX_NOTE_LENGTH + 1);
                steps[i].mod = random(0, MAX_MOD_VALUE + 1);

                int temp = random(0, 100);

                if(temp < RANDOM_GATE_PROBABILITY)
                {
                    steps[i].active = true;
                }
                else
                {
                    steps[i].active = false;
                }
            }   
        }

        /**
         * Randomise the start and end points of the loop
         */
        void randomizeBounds()
        {
            start = random(0, MAX_STEPS / 2);
            end = random(MAX_STEPS / 2 + 1, MAX_STEPS);
        }

        /**
         * Invert the selection around its pitch center
         */
        void invert(int start_selection, int end_selection) 
        {
            int s = max(start_selection, 0);
            int e = min(end_selection, MAX_STEPS);

            for(int i = s; i < e; i++) 
            {
                steps[i].note = steps[i].note * -1 + MAX_NOTE_VALUE;
            }
        }

        /**
         * Transpose selection by one note up
         */ 
        void transposeUp(int start_selection, int end_selection) 
        {
            int s = max(start_selection, 0);
            int e = min(end_selection, MAX_STEPS);

            for(int i = s; i < e; i++) 
            {
                if(steps[i].note < MAX_NOTE_VALUE - 1) 
                {
                    steps[i].note += 1; 
                }
            }
        }

        /**
         * Transpose selection by one note down 
         */ 
        void transposeDown(int start_selection, int end_selection) 
        {
            int s = max(start_selection, 0);
            int e = min(end_selection, MAX_STEPS);

            for(int i = s; i < e; i++) 
            {
                if(steps[i].note > 0) 
                {
                    steps[i].note -= 1; 
                }
            }
        }

        /**
         * Reverse the selected steps 
         */ 
        void reverse(int start_selection, int end_selection) 
        {
            int s = max(start_selection, 0);
            int e = min(end_selection, MAX_STEPS);

            int temp[end_selection - start_selection];
            for(int i = s; i < e; i++) 
            {
                temp[i] = steps[i].note; 
            }

            for(int i = s; i < e; i++) 
            {
                steps[i].note = temp[end_selection - start_selection - 1 - i]; 
            }
        }

        /**
         * Hold the first step for the duration of the selection
         */ 
        void sustain(int start_selection, int end_selection) 
        {
            int s = max(start_selection, 0);
            int e = min(end_selection, MAX_STEPS);

            for(int i = s + 1; i < e; i++) 
            {
                steps[i].note = steps[s].note; 
            } 
        }

        /**
         * Paste in a new set of steps to a location
         */
        void paste(int start_selection, Step values[MAX_STEPS], int len) 
        {
            int s = max(start_selection, 0);

            int j = 0;
            for(int i = s; i < s + len; i++) 
            {
                steps[i] = values[j]; 
                j += 1;
            } 
        }

        /**
         * Turn the gate off for a selection
         */
        void mute(int start_selection, int end_selection) 
        {
            int s = max(start_selection, 0);
            int e = min(end_selection, MAX_STEPS);

            for(int i = s; i < e; i++) 
            {
                steps[i].active = true; 
            }
        }

        /**
         * Turn the gate on for a selection
         */
        void unmute(int start_selection, int end_selection) 
        {
            int s = max(start_selection, 0);
            int e = min(end_selection, MAX_STEPS);

            for(int i = s; i < e; i++) 
            {
                steps[i].active = false; 
            }
        }

        // remove()
        // insert()
    };
}