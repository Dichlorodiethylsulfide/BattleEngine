#pragma once

#include "BEString.h"
#include "BEEngineLoop.h"
#include "BETime.h"

class BEWindow
{
public:
    using Dimensions = BEBox<int>;
    
    BEWindow(BEString WindowTitle, Dimensions Dimensions, unsigned int FrameRate = 60);
    void EnterMainLoop();
    ~BEWindow();
    static void Init();
    static void Quit();
private:
    void EnsureFrameTimeIsKept();
    BEString m_windowTitle{"Default Window"};
    Dimensions m_windowDimensions{};
    External m_windowPointer;
    BECPULoop m_engineLoop{};
    BEGPULoop m_renderLoop{};
    BETime::BETimeKeeper m_frameTimeKeeper{0};
    BETime::SizeType m_millisecondsPerFrame{};
};
