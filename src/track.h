#pragma once

#include <Arduino.h>
#include "config.h"
#include "sequence.h"

using namespace config;

namespace engine 
{
    // ======================================================================
    // TRACK 
    // ======================================================================
    struct Track 
    {
        Sequence seqs[MAX_SEQUENCES];

        int currentSequence = 0;

        int step = 0;
        int tick = 0;
        
        int cursor = DEFAULT_CURSOR_POS;
        int cursorLen = DEFAULT_CURSOR_LENGTH;

        /**
         * Calculate the next tick and move step if needed
         */
        void nextTick() 
        {
            if(tick >= seqs[currentSequence].steps[step].length) 
            {
                tick = 0;
                nextStep();
            } 
            else 
            {
                tick += 1;
            }
        }

        /**
         * Calculate the next step and jump if needed
         */
        void nextStep() 
        {
            if(step == seqs[currentSequence].end && seqs[currentSequence].enableLoop) 
            {
                step = seqs[currentSequence].start;
            } 
            else if(step >= MAX_STEPS - 1)
            {
                step = 0;
            }
            else
            {
                step += 1;
            }
        }

        /**
         * Move this track to the next sequence
         */
        void incSequence() 
        {
            if (currentSequence >= MAX_SEQUENCES - 1)
            {
                return;
            }
            currentSequence += 1;
        }

        /**
         * Move this track to the previous sequence
         */
        void decSequence() 
        {
            if (currentSequence <= 0)
            {
                return;
            }
            currentSequence -= 1;
        }

        /**
         * Jump the playhead to the specified position
         */
        void jump(int pos) 
        {
            if (pos >= MAX_STEPS)
            {
                return;
            }
            step = pos;
            tick = 0;
        }

        /**
         * Set the loop points to the current cursor position
         */
        void setBoundsToCursor() 
        {
            seqs[currentSequence].start = cursor;
            seqs[currentSequence].end = cursor + cursorLen;
        }
    };
}