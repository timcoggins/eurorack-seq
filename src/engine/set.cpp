#include "engine.h"

using namespace Engine;

void Engine::Set::randomiseAll() 
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

void Engine::Set::nextTick()
{
    for(int i = 0; i < MAX_TRACKS; i++) 
    {
        tracks[i].nextTick();     
    }
};

Step Engine::Set::getStep(int track) 
{
    int currentStep = tracks[track].step;
    int currentSeq = tracks[track].currentSequence;
    return tracks[track].seqs[currentSeq].steps[currentStep];
}

int Engine::Set::getNote(int track) 
{
    return tracks[track].currentNote;
}