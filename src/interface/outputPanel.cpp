#include "interface.h"

using namespace Interface;

void Interface::Ui::drawOutputVoltages(int note) {
    for(int i = 0; i < 16; i += 1) 
    {
        display.fillRect(i * 4 , 0, i * 4 + 2, i * 2, note == i ? Hardware::COLOUR_GREEN : Hardware::COLOUR_BLUE);
    }
}