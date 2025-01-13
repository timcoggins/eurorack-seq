#include "interface.h"

using namespace Interface;

void Interface::Ui::drawOutputPanel(Engine::Set& seq)
{
    drawOutputVoltages(seq.getNote(0));
}