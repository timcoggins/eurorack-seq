#pragma once

#include <Arduino.h>
#include "config.h"
#include "track.h"

using namespace config;

namespace engine 
{
    // ======================================================================
    // MAIN ENGINE 
    // ======================================================================
    struct Engine 
    {
        Track tracks[MAX_TRACKS];

        /**
         * Randomise all tracks / sequences / notes on startup
         */
        void randomiseAll() 
        {
            randomSeed(analogRead(RANDOM_SEED_PIN));
            for(int i = 0; i < MAX_TRACKS; i++) 
            {
                for(int j = 0; j < MAX_SEQUENCES; j++) 
                {
                    tracks[i].seqs[j].randomize(0, MAX_STEPS);
                    tracks[i].seqs[j].randomizeBounds();
                }
            } 
        }

        /**
         * Move all tracks to the next tick
         */
        void nextTick()
        {
            for(int i = 0; i < MAX_TRACKS; i++) 
            {
                tracks[i].nextTick();     
            }
        };

        /**
         * Returns the current step for the provided track number
         */
        Step getStep(int track) 
        {
            int currentStep = tracks[track].step;
            int currentSeq = tracks[track].currentSequence;
            return tracks[track].seqs[currentSeq].steps[currentStep];
        }

        int getNote(int track) 
        {
            return tracks[track].currentNote;
        }
    };
}