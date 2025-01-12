#include "interface.h"

using namespace Interface;

void Interface::Ui::selectNextTrack() {
    if(selectedTrack >= MAX_TRACKS - 1)
    {
        selectedTrack = 0;
    } else {
        selectedTrack += 1;
    }
}

void Interface::Ui::nextPage() {
    if(page == MAX_UI_PAGES - 1)
    {
        page = 0;
    }
    else
    {
        page += 1;
    }
    display.clear();
}

void Interface::Ui::handleControls(Engine::Set& seq)
{
    // Hardcoded so page can switch from every page
    if(btn8.state == HIGH)
    {
        nextPage();
    }

    switch(page)
    {
        case 0:
            handleMainControls(seq);
            break;
        default:
            break; 
    }
}

void Interface::Ui::handleDraw(Engine::Set& seq)
{
    if(page == 0)
    {
        drawMainPanel(seq);
    }
    else if(page == 1)
    {
        drawOutputPanel(seq);
    }

    display.finishDraw();
}