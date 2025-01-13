#include "interface.h"

void Interface::Ui::drawDivider(int yOffset)
{
    display.drawLine(0, yOffset, MAX_STEPS - 1, yOffset, Hardware::COLOUR_DIVIDER);
    for(int i = 0; i < MAX_STEPS; i += SHOW_GRID_EVERY)
    {
        display.drawPixel(i, yOffset, Hardware::COLOUR_DIVIDER_GRID);
    }
}

void Interface::Ui::drawTrack(Engine::Track& track, int yOffset) 
{
    Engine::Sequence& currentSequence = track.seqs[track.currentSequence];

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
                    display.drawPixel(i, j + yOffset, Hardware::COLOUR_NOTE_ACTIVE);
                } 
                else 
                {
                    display.drawPixel(i, j + yOffset, Hardware::COLOUR_NOTE_INACTIVE);
                }
            } 
            else 
            {
                // Playhead
                if (i == track.step) 
                {
                    display.drawPixel(i, j + yOffset, Hardware::COLOUR_PLAYHEAD);
                } 
                // Cursor in Bounds
                else if(i == track.cursor && selectedTrack == t && i >= currentSequence.start && i <= currentSequence.end && currentSequence.enableLoop) {
                    display.drawPixel(i, j + yOffset, Hardware::COLOUR_CURSOR_OVERLAP);
                } 
                // Cursor
                else if(i == track.cursor && selectedTrack == t) {
                    display.drawPixel(i, j + yOffset, Hardware::COLOUR_CURSOR);
                } 
                // Selection from Cursor in Bounds
                else if (i > track.cursor && i < track.cursor + track.cursorLen && selectedTrack == t && i >= currentSequence.start && i <= currentSequence.end && currentSequence.enableLoop) 
                {
                    display.drawPixel(i, j + yOffset, Hardware::COLOUR_CURSOR_OVERLAP);
                } 
                // Selection from Cursor
                else if (i > track.cursor && i < track.cursor + track.cursorLen && selectedTrack == t) 
                {
                    display.drawPixel(i, j + yOffset, Hardware::COLOUR_CURSOR);
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
                    display.drawPixel(i, j + yOffset, Hardware::COLOUR_BACKGROUND);
                }
            }
        }
    }
} 



void Interface::Ui::drawModTrack(Engine::Track& track, int yOffset)
{
        Engine::Sequence& currentSequence = track.seqs[track.currentSequence];

        for(int i = 0; i < MAX_STEPS; i++) 
        {
            for(int j = 0; j < MAX_MOD_VALUE; j++) 
            {
                if((MAX_MOD_VALUE - 1 - currentSequence.steps[i].mod) <= j) 
                {
                    display.drawPixel(i, j + yOffset, Hardware::COLOUR_MOD);
                }
                else
                {
                    display.drawPixel(i, j + yOffset, Hardware::COLOUR_BACKGROUND);
                }
            }
        }
}

void Interface::Ui::drawGateTrack(Engine::Track& track, int yOffset)
{
    Engine::Sequence& currentSequence = track.seqs[track.currentSequence];

    for(int i = 0; i < MAX_STEPS; i++) 
    {
        if(currentSequence.steps[i].active == true) 
        {
            if(track.step == i) 
            {
                display.drawLine(i, yOffset, i, yOffset + 1, Hardware::COLOUR_GATE_ACTIVE);
            } 
            else 
            {
                display.drawLine(i, yOffset, i, yOffset + 1, Hardware::COLOUR_GATE);
            }
        } 
        else 
        {
            display.drawLine(i, yOffset, i, yOffset + 1, Hardware::COLOUR_BACKGROUND);
        }
    }  
}

// void Interface::Ui::drawSequenceLength(Engine::Track& currentTrack, int y)
// {
//     Engine::Sequence& currentSequence = currentTrack.seqs[currentTrack.currentSequence];

//     matrix.fillRect(26, y, 11, 8, COLOUR_BACKGROUND);
//     matrix.setCursor(26, y);
//     matrix.setTextSize(0);
//     matrix.print(currentSequence.end - currentSequence.start);
// }

void Interface::Ui::drawSequenceSelector(Engine::Track& track, int yOffset) 
{
    for(int i = 0; i < MAX_SEQUENCES; i++) 
    {
        if(track.currentSequence < i) 
        {
            display.drawLine(i, yOffset, i, yOffset + 1, Hardware::COLOUR_SEQ);
        }
        else 
        {
            display.drawLine(i, yOffset, i, yOffset + 1, Hardware::COLOUR_SEQ_ACTIVE);
        }
    }
}

void Interface::Ui::drawLoopBounds(Engine::Track& track, int yOffset)
{
    Engine::Sequence& currentSequence = track.seqs[track.currentSequence];

    for(int i = 0; i < MAX_STEPS; i++) 
    {
        if(i >= currentSequence.start && i <= currentSequence.end) 
        {
            display.drawLine(i, yOffset, i, yOffset + 1, currentSequence.enableLoop ? Hardware::COLOUR_LOOP_BOUNDS_ACTIVE : Hardware::COLOUR_LOOP_BOUNDS_INACTIVE);
        }
        else 
        {
            display.drawLine(i, yOffset, i, yOffset + 1, Hardware::COLOUR_BACKGROUND);
        }
    }
}

