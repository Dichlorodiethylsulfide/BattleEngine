#pragma once

#include "BEString.h"

class BEWindow
{
public:
    BEWindow(BEString WindowTitle, BEBox<int> Dimensions);
    ~BEWindow();
private:
    BEString m_windowTitle{"Default Window"};
    BEBox<int> m_windowDimensions{{0, 0}, {0, 0}};
    External m_windowPointer;
};
