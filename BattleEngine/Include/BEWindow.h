#pragma once

#include "BEString.h"
#include "BEEngineLoop.h"

class BEWindow
{
public:
    BEWindow(BEString WindowTitle, BEBox<int> Dimensions, int FrameRate = 60);
    void EnterMainLoop();
    ~BEWindow();
private:
    BEString m_windowTitle{"Default Window"};
    BEBox<int> m_windowDimensions{{0, 0}, {0, 0}};
    External m_windowPointer;
    BECPULoop m_engineLoop{};
    BEGPULoop m_renderLoop{};
    int m_millisecondsPerFrame{};
};
