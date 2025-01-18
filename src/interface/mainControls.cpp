#include "interface.h"

using namespace Interface;

void Interface::Ui::drawMainPanel(Engine::Set& seq)
{
    Engine::Track& track = seq.tracks[selectedTrack];

    drawLoopBounds(track, 0);
    drawDivider(2);
    drawTrack(track, 3);
    drawDivider(19);
    drawGateTrack(track, 20);
    drawDivider(22);
    drawModTrack(track, 23);
    drawDivider(28);

    drawSequenceSelector(track, 30);
}

void Interface::Ui::handleMainControls(Engine::Set& seq)
{
    Engine::Track& currentTrack = seq.tracks[selectedTrack];
    Engine::Sequence& currentSequence = currentTrack.seqs[currentTrack.currentSequence];

    // BUTTON COMBOS
    if(btn1.state == HIGH && btn2.state == HIGH) 
    {
        if(encoder.state == 1) currentSequence.randomizeNotes(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
        else if (encoder.state == -1) currentSequence.randomizeGates(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
    }
    else if(btn1.state == HIGH && btn3.state == HIGH) 
    {
        if(encoder.state == 1) currentSequence.increaseStart();
        else if(encoder.state == -1) currentSequence.decreaseStart();
    }
    else if(btn1.state == HIGH && btn4.state == HIGH) 
    {
        if(encoder.state == 1) currentSequence.increaseEnd();
        else if(encoder.state == -1) currentSequence.decreaseEnd();
    }
    else if(btn3.state == HIGH && btn4.state == HIGH) 
    {
        if(encoder.state == 1) 
        {
            currentSequence.increaseStart();
            currentSequence.increaseEnd();
        }
        if(encoder.state == -1) 
        {
            currentSequence.decreaseStart();
            currentSequence.decreaseEnd();
        }
    }

    // WITH ENCODER
    else if(btn1.state == HIGH) 
    {
        if(encoder.state == 1) 
        {
            currentSequence.transposeUp(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
        }
        else if(encoder.state == -1) 
        {
            currentSequence.transposeDown(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
        }
    }
    else if(btn2.state == HIGH) 
    {
        if(encoder.state == 1) currentSequence.mute(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
        else if(encoder.state == -1) currentSequence.unmute(currentTrack.cursor, currentTrack.cursor + currentTrack.cursorLen);
    }
    else if(btn6.state == HIGH) 
    {
        if(encoder.state == 1) currentTrack.incSequence();
        else if(encoder.state == -1) currentTrack.decSequence();
    }
    else if(btn7.state == HIGH) 
    {
        if(encoder.state == 1) currentTrack.incSequence();
        else if(encoder.state == -1) currentTrack.decSequence();
    }
    else if(encoderBtn.state == LOW)    // REVERSE POLARITY FOR SOME REASON... 
    {
        if(encoder.state == 1) currentTrack.cursorLen += 1;
        else if(encoder.state == -1) currentTrack.cursorLen -= 1;
    }
    else if(encoder.state == 1)
    {
        if(currentTrack.cursor >= MAX_STEPS) return;
        currentTrack.cursor += 1;
    }
    else if(encoder.state == -1)
    {
        if(currentTrack.cursor <= 0) return;
        currentTrack.cursor -= 1;
    }
    else if(btn5.state == HIGH)
    {
        currentSequence.toggleLoop();
    }
    else if(btn6.state == HIGH) 
    {
        currentSequence.sustain(currentTrack.cursor, currentTrack.cursorLen);
    }
    else if(btn7.state == HIGH)
    {
        currentTrack.jump(currentTrack.cursor);
    }
    else if(btn9.state == HIGH)
    {
        copyToBuffer(currentTrack);
    }
    else if(btn10.state == HIGH)
    {   
        int len = min(copyLength, currentTrack.cursorLen);
        currentSequence.paste(currentTrack.cursor, copyBuffer, len);
    }
    else
    {
        return;
    }

    handleDraw(seq);
}