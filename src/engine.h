#include <Arduino.h>
#include "config.h"

#pragma once

using namespace config;

namespace engine 
{

    // ======================================================================
    // step 
    // ======================================================================
    struct Step 
    {
        int note = 1;
        int length = 1;
        bool active = true;
    };

    // ======================================================================
    // SEQUENCE 
    // ======================================================================
    struct Sequence 
    {
        std::array<Step, MAX_STEPS> steps;
        
        int start = DEFAULT_START_POS;
        int end = DEFAULT_END_POS - 1;

        void setStart(int pos) 
        {
            if (pos >= MAX_STEPS) return;
            if (pos >= end) return;
            if (pos < 0) return;
            start = pos;
        }

        void increaseStart() 
        {
            setStart(start + 1);
        }

        void decreaseStart() 
        {
            setStart(start - 1);
        }

        void setEnd(int pos) 
        {
            if (pos > MAX_STEPS) return;
            if (pos <= start) return;
            end = pos;
        }

        void increaseEnd() 
        {
            setEnd(end + 1);
        }

        void decreaseEnd() 
        {
            setEnd(end - 1);
        }

        void randomize(int start_selection, int end_selection) 
        {
            int s = max(start_selection, 0);
            int e = min(end_selection, MAX_STEPS);
            for(int i = s; i < e; i++) 
            {
                steps[i].note = random(RANDOM_MIN_NOTE, RANDOM_MAX_NOTE + 1);
                steps[i].length = random(RANDOM_MIN_NOTE_LENGTH, RANDOM_MAX_NOTE_LENGTH + 1);
                steps[i].active = random(0, 2);
            }   
        }

        void invert(int start_selection, int end_selection) 
        {
            int s = max(start_selection, 0);
            int e = min(end_selection, MAX_STEPS);

            for(int i = s; i < e; i++) 
            {
                steps[i].note = steps[i].note * -1 + MAX_NOTE_VALUE;
            }
        }

        void transposeUp(int start_selection, int end_selection) 
        {
            int s = max(start_selection, 0);
            int e = min(end_selection, MAX_STEPS);

            for(int i = s; i < e; i++) 
            {
                if(steps[i].note < MAX_NOTE_VALUE) 
                {
                    steps[i].note += 1; 
                }
            }
        }

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

        void sustain(int start_selection, int end_selection) 
        {
            int s = max(start_selection, 0);
            int e = min(end_selection, MAX_STEPS);

            for(int i = s + 1; i < e; i++) 
            {
                steps[i].note = steps[s].note; 
            } 
        }

        void paste(int start_selection, int values[4]) 
        {
            int s = max(start_selection, 0);

            for(int i = s + 1; i < s + 4; i++) 
            {
                steps[i].note = values[i]; 
            } 
        }
    };

    // ======================================================================
    // TRACK 
    // ======================================================================
    struct Track 
    {
        Sequence seqs[MAX_SEQUENCES];

        int currentSequence = 0;
        int step = 0;
        int tick = 0;

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

        void nextStep() 
        {
            if(step >= seqs[currentSequence].end || step >= MAX_STEPS - 1) 
            {
                step = seqs[currentSequence].start;
            } 
            else 
            {
                step += 1;
            }
        }

        void incSequence() 
        {
            if (currentSequence >= MAX_SEQUENCES - 1)
            {
                return;
            }
            currentSequence += 1;
        }

        void decSequence() 
        {
            if (currentSequence <= 0)
            {
                return;
            }
            currentSequence -= 1;
        }
    };

    // ======================================================================
    // MAIN ENGINE 
    // ======================================================================
    struct Engine 
    {
        Track tracks[MAX_TRACKS];

        void randomiseAll() 
        {
            randomSeed(analogRead(RANDOM_SEED_PIN));
            for(int i = 0; i < MAX_TRACKS; i++) 
            {
                for(int j = 0; j < MAX_SEQUENCES; j++) 
                {
                    tracks[i].seqs[j].randomize(0, MAX_STEPS);
                }
            } 
        }

        void nextTick() 
        {
            for(int i = 0; i < MAX_TRACKS; i++) 
            {
                tracks[i].nextTick();     
            }
        }

        Step getStep(int track) 
        {
            int currentStep = tracks[track].step;
            int currentSeq = tracks[track].currentSequence;
            return tracks[track].seqs[currentSeq].steps[currentStep];
        }
    };
}