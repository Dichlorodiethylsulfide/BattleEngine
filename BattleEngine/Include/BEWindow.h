#pragma once

#include "BEString.h"
#include "BEEngineLoop.h"

class BEWindow
{
public:
    BEWindow(BEString WindowTitle, BEBox<int> Dimensions);
    void EnterMainLoop();
    ~BEWindow();
private:
    BEString m_windowTitle{"Default Window"};
    BEBox<int> m_windowDimensions{{0, 0}, {0, 0}};
    External m_windowPointer;
    BECPULoop m_engineLoop{};
    BEGPULoop m_renderLoop{};
};
