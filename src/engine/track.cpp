#include "engine.h"

using namespace Engine;


void Engine::Track::nextTick() 
{
    if(tick >= seqs[currentSequence].steps[step].length) 
    {
        tick = 0;
        nextStep();

        if(seqs[currentSequence].steps[step].active)
        {
            currentNote = seqs[currentSequence].steps[step].note;
        }
    } 
    else 
    {
        tick += 1;
    }
}

void Engine::Track::nextStep() 
{
    if(step == seqs[currentSequence].end && seqs[currentSequence].enableLoop) 
    {
        step = seqs[currentSequence].start;
    } 
    else if(step >= MAX_STEPS - 1)
    {
        if(currentSequence >= MAX_SEQUENCES - 1)
        {
            currentSequence = 0;
        }
        else
        {
            currentSequence += 1;
        }
        step = 0;
    }
    else
    {
        step += 1;
    }
}

void Engine::Track::incSequence() 
{
    if (currentSequence >= MAX_SEQUENCES - 1)
    {
        return;
    }
    currentSequence += 1;
}

void Engine::Track::decSequence() 
{
    if (currentSequence <= 0)
    {
        return;
    }
    currentSequence -= 1;
}

void Engine::Track::jump(int pos) 
{
    if (pos >= MAX_STEPS)
    {
        return;
    }
    step = pos;
    tick = 0;
}

void Engine::Track::setBoundsToCursor() 
{
    seqs[currentSequence].start = cursor;
    seqs[currentSequence].end = cursor + cursorLen;
}