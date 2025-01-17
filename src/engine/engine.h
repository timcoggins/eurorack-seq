#pragma once

#include "../config.h"

namespace Engine 
{
    struct Step 
    {
        int note = 1;
        int length = 1;
        bool active = true;
        int mod = 3;
        bool accent = false;
    };

    struct Sequence 
    {
        std::array<Step, MAX_STEPS> steps;
        
        int start = DEFAULT_START_POS;
        int end = DEFAULT_END_POS - 1;

        bool enableLoop = DEFAULT_LOOP_ENABLE;

        void toggleLoop();
        void setStart(int pos);
        void increaseStart();
        void decreaseStart();
        void setEnd(int pos);
        void increaseEnd();
        void decreaseEnd();
        void randomize(int start_selection, int end_selection);
        void randomizeNotes(int start_selection, int end_selection);
        void randomizeGates(int start_selection, int end_selection);
        void randomizeBounds();
        void invert(int start_selection, int end_selection);
        void transposeUp(int start_selection, int end_selection);
        void transposeDown(int start_selection, int end_selection);
        void reverse(int start_selection, int end_selection);
        void sustain(int start_selection, int end_selection);
        void paste(int start_selection, Step values[MAX_STEPS], int len);
        void mute(int start_selection, int end_selection);
        void unmute(int start_selection, int end_selection);
        // remove
        // insert
    };

    struct Track 
    {
        Sequence seqs[MAX_SEQUENCES];

        int currentSequence = 0;

        int step = 0;
        int tick = 0;
        
        int cursor = DEFAULT_CURSOR_POS;
        int cursorLen = DEFAULT_CURSOR_LENGTH;

        int currentNote = 0;

        void nextTick();
        void nextStep();
        void incSequence();
        void decSequence();
        void jump(int pos);
        void setBoundsToCursor();
    };


    struct Set
    {
        Track tracks[MAX_TRACKS];
    
        void setup();
        void nextTick();
        Step getStep(int track);
        int getNote(int track);
    }; 
}