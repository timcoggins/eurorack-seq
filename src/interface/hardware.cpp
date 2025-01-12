#include "interface.h"

using namespace Interface;

void Interface::Ui::setup()
{
    display.setup();
    lcdD.setup();

    encoder.setup();
    encoderBtn.setup(ENC_BUTTON_PIN);
    btn1.setup(B1_PIN);
    btn2.setup(B2_PIN);
    btn3.setup(B3_PIN);
    btn4.setup(B4_PIN);
    btn5.setup(B5_PIN);
    btn6.setup(B6_PIN);
    btn7.setup(B7_PIN);
    btn8.setup(B8_PIN);
    btn9.setup(B9_PIN);
    btn10.setup(B10_PIN);
}

void Interface::Ui::scan() 
{
    encoder.read();
    encoderBtn.read();

    btn1.read();
    btn2.read();
    btn3.read();
    btn4.read();
    btn5.readDebounced();
    btn6.readDebounced();
    btn7.read();
    btn8.readDebounced();
    btn9.readDebounced();
    btn10.readDebounced();
}

void Interface::Ui::copyToBuffer(Engine::Track& currentTrack)
{
    Engine::Sequence& currentSequence = currentTrack.seqs[currentTrack.currentSequence];
    int j = 0;
    for(int i = currentTrack.cursor; i < currentTrack.cursor + currentTrack.cursorLen; i++)
    {
        copyBuffer[j] = currentSequence.steps[i];
        j += 1;
    }
    copyLength = j;
}