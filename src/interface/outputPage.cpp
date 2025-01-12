#include "interface.h"

using namespace Interface;

void Interface::Ui::drawOutputPanel(Engine::Set& seq)
{
    display.drawOutputVoltages(seq.getNote(0));
}