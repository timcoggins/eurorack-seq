#include "engine.h"

using namespace Engine;

void Engine::Sequence::toggleLoop()
{
    enableLoop = !enableLoop;
}

void Engine::Sequence::setStart(int pos) 
{
    if (pos >= MAX_STEPS) return;
    if (pos >= end) return;
    if (pos < 0) return;
    start = pos;
}

void Engine::Sequence::increaseStart() 
{
    setStart(start + 1);
}

void Engine::Sequence::decreaseStart() 
{
    setStart(start - 1);
}

void Engine::Sequence::setEnd(int pos) 
{
    if (pos > MAX_STEPS) return;
    if (pos <= start) return;
    end = pos;
}

void Engine::Sequence::increaseEnd() 
{
    setEnd(end + 1);
}

void Engine::Sequence::decreaseEnd() 
{
    setEnd(end - 1);
}

void Engine::Sequence::randomize(int start_selection, int end_selection) 
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

        int accent = random(0, 100);

        if(accent < 50)
        {
            steps[i].accent = true;
        }
        else
        {
            steps[i].accent = false;
        }
    }   
}

void Engine::Sequence::randomizeNotes(int start_selection, int end_selection) 
{
    int s = max(start_selection, 0);
    int e = min(end_selection, MAX_STEPS);
    for(int i = s; i < e; i++) 
    {
        steps[i].note = random(RANDOM_MIN_NOTE, RANDOM_MAX_NOTE);
    }   
}

void Engine::Sequence::randomizeGates(int start_selection, int end_selection) 
{
    int s = max(start_selection, 0);
    int e = min(end_selection, MAX_STEPS);
    for(int i = s; i < e; i++) 
    {
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


void Engine::Sequence::randomizeBounds()
{
    start = random(0, MAX_STEPS / 2);
    end = random(MAX_STEPS / 2 + 1, MAX_STEPS);
}

void Engine::Sequence::invert(int start_selection, int end_selection) 
{
    int s = max(start_selection, 0);
    int e = min(end_selection, MAX_STEPS);

    for(int i = s; i < e; i++) 
    {
        steps[i].note = steps[i].note * -1 + MAX_NOTE_VALUE;
    }
}

void Engine::Sequence::transposeUp(int start_selection, int end_selection) 
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

void Engine::Sequence::transposeDown(int start_selection, int end_selection) 
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

void Engine::Sequence::reverse(int start_selection, int end_selection) 
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

void Engine::Sequence::sustain(int start_selection, int end_selection) 
{
    int s = max(start_selection, 0);
    int e = min(end_selection, MAX_STEPS);

    for(int i = s + 1; i < e; i++) 
    {
        steps[i].note = steps[s].note; 
    } 
}

void Engine::Sequence::paste(int start_selection, Step values[MAX_STEPS], int len) 
{
    int s = max(start_selection, 0);

    int j = 0;
    for(int i = s; i < s + len; i++) 
    {
        steps[i] = values[j]; 
        j += 1;
    } 
}

void Engine::Sequence::mute(int start_selection, int end_selection) 
{
    int s = max(start_selection, 0);
    int e = min(end_selection, MAX_STEPS);

    for(int i = s; i < e; i++) 
    {
        steps[i].active = true; 
    }
}

void Engine::Sequence::unmute(int start_selection, int end_selection) 
{
    int s = max(start_selection, 0);
    int e = min(end_selection, MAX_STEPS);

    for(int i = s; i < e; i++) 
    {
        steps[i].active = false; 
    }
}
