#pragma once
#include <iostream>

// Basic logging for now

enum BEIOColourCode
{
    // Foreground
    BE_FG_RED      = 31,
    BE_FG_GREEN    = 32,
    BE_FG_YELLOW   = 33,
    BE_FG_BLUE     = 34,
    BE_FG_WHITE    = 37,
    BE_FG_DEFAULT  = 39,
    // Background
    BE_BG_RED      = 41,
    BE_BG_GREEN    = 42,
    BE_BG_YELLOW   = 43,
    BE_BG_BLUE     = 44,
    BE_BG_WHITE    = 47,
    BE_BG_DEFAULT  = 49
};

class BEIOColorOutputModifier {
    BEIOColourCode m_Code;
public:
    BEIOColorOutputModifier(BEIOColourCode pCode) : m_Code(pCode) {}
    
    friend std::ostream&
    operator<<(std::ostream& os, const BEIOColorOutputModifier& mod)
    {
        return os << "\033[" << mod.m_Code << "m";
    }
};