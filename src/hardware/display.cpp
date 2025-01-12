#include "hardware.h"

using namespace Hardware;


void Hardware::Display::setup()
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

void Hardware::Display::drawDivider(int yOffset)
{
    matrix.drawLine(0, yOffset, MAX_STEPS - 1, yOffset, COLOUR_DIVIDER);
    for(int i = 0; i < MAX_STEPS; i += SHOW_GRID_EVERY)
    {
        matrix.drawPixel(i, yOffset, COLOUR_DIVIDER_GRID);
    }
}

void Hardware::Display::drawTrack(Engine::Track& track, int yOffset) 
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


void Hardware::Display::drawModTrack(Engine::Track& track, int yOffset)
{
        Engine::Sequence& currentSequence = track.seqs[track.currentSequence];

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

void Hardware::Display::drawGateTrack(Engine::Track& track, int yOffset)
{
    Engine::Sequence& currentSequence = track.seqs[track.currentSequence];

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

void Hardware::Display::drawSequenceLength(Engine::Track& currentTrack, int y)
{
    Engine::Sequence& currentSequence = currentTrack.seqs[currentTrack.currentSequence];

    matrix.fillRect(26, y, 11, 8, COLOUR_BACKGROUND);

    matrix.setCursor(26, y);
    matrix.setTextSize(0);
    matrix.print(currentSequence.end - currentSequence.start);
}

void Hardware::Display::drawSequenceSelector(Engine::Track& track, int yOffset) 
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

void Hardware::Display::drawLoopBounds(Engine::Track& track, int yOffset)
{
    Engine::Sequence& currentSequence = track.seqs[track.currentSequence];

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

void Hardware::Display::drawOutputVoltages(int note) {
    for(int i = 0; i < 16; i += 1) 
    {
        matrix.fillRect(i * 4 , 0, i * 4 + 2, i * 2, note == i ? COLOUR_GREEN : COLOUR_BLUE);
    }
}

void Hardware::Display::finishDraw() 
{
    matrix.show();
}

void Hardware::Display::clear() 
{
    matrix.fillRect(0, 0, 64, 32, COLOUR_BACKGROUND);
}