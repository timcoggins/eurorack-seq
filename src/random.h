#include <Arduino.h>

namespace patterns 
{

    class Gen
    {
        bool coinToss(int weight)
        {
            if(random(0, 100) < weight)
            {
                return true;
            }
            return false;
        }

        void randomize(int start_selection, int end_selection) 
        {
            // int s = max(start_selection, 0);
            // int e = min(end_selection, MAX_STEPS);
            // for(int i = s; i < e; i++) 
            // {
            //     steps[i].note = random(RANDOM_MIN_NOTE, RANDOM_MAX_NOTE);
            //     steps[i].length = random(RANDOM_MIN_NOTE_LENGTH, RANDOM_MAX_NOTE_LENGTH + 1);
            //     steps[i].mod = random(0, MAX_MOD_VALUE + 1);

            //     int temp = random(0, 100);

            //     if(temp < RANDOM_GATE_PROBABILITY)
            //     {
            //         steps[i].active = true;
            //     }
            //     else
            //     {
            //         steps[i].active = false;
            //     }
            // }   
        }
        

    };
}