#pragma once

#include "BEString.h"

class BECPULoop // main engine loop, performed on the CPU, might need a GPU loop for GPU operations such as rendering
{
public:
    void Tick();
    operator bool() const
    {
        return !m_shouldStopTicking;        
    }
private:
    bool m_shouldStopTicking{};
};

class BEWindow
{
public:
    BEWindow(BEString WindowTitle, BEBox<int> Dimensions);
    void EnterLoop();
    ~BEWindow();
private:
    BEString m_windowTitle{"Default Window"};
    BEBox<int> m_windowDimensions{{0, 0}, {0, 0}};
    External m_windowPointer;
    BECPULoop m_engineLoop{};
};
