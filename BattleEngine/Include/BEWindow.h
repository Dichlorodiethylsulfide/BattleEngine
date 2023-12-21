#pragma once

#include "BEString.h"
#include "BEEngineLoop.h"
#include "BETime.h"

class BEWindow
{
public:
    BEWindow(BEString WindowTitle, BEBox<int> Dimensions, unsigned int FrameRate = 60);
    void EnterMainLoop();
    ~BEWindow();
private:
    void EnsureFrameTimeIsKept();
    BEString m_windowTitle{"Default Window"};
    BEBox<int> m_windowDimensions{{0, 0}, {0, 0}};
    External m_windowPointer;
    BECPULoop m_engineLoop{};
    BEGPULoop m_renderLoop{};
    BETime::BETimeKeeper m_frameTimeKeeper{0};
    BETime::SizeType m_millisecondsPerFrame{};
};
