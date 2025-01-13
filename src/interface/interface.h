#pragma once

#include <Arduino.h>

#include "../engine/engine.h"
#include "../hardware/hardware.h"

using namespace config;

namespace Interface
{

    struct Ui
    {
        Hardware::Display display;
        Hardware::LCD lcdD;

        Hardware::Encoder encoder;
        Hardware::Button encoderBtn;

        Hardware::Button btn1;
        Hardware::Button btn2;
        Hardware::Button btn3;
        Hardware::Button btn4;
        Hardware::Button btn5;
        Hardware::Button btn6;
        Hardware::Button btn7;
        Hardware::Button btn8;
        Hardware::Button btn9;
        Hardware::Button btn10;

        Engine::Step copyBuffer[MAX_STEPS];

        int selectedTrack = 0;
        int copyLength = 0;
        int page = 0;

        void setup();
        void scan();
        void copyToBuffer(Engine::Track& currentTrack);
        void selectNextTrack();
        void nextPage();

        void handleControls(Engine::Set& seq);
        void handleDraw(Engine::Set& seq); 

        void handleMainControls(Engine::Set& seq); 
        void drawMainPanel(Engine::Set& seq);

        void drawDivider(int yOffset);
        void drawTrack(Engine::Track& track, int yOffset);
        void drawModTrack(Engine::Track& track, int yOffset);
        void drawGateTrack(Engine::Track& track, int yOffset);
        // void drawSequenceLength(Engine::Track& currentTrack, int y);
        void drawSequenceSelector(Engine::Track& track, int yOffset);
        void drawLoopBounds(Engine::Track& track, int yOffset);
        void drawOutputVoltages(int note);

        void drawOutputPanel(Engine::Set& seq);
    };
}
