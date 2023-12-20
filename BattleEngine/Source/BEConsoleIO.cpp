﻿#include "BEConsoleIO.h"
#include "BETime.h"

#include<iostream>
#include <ostream>

class BEIOColorOutputModifier {
    BEIOColourCode code;
public:
    BEIOColorOutputModifier(BEIOColourCode pCode) : code(pCode) {}
    
    friend std::ostream&
    operator<<(std::ostream& os, const BEIOColorOutputModifier& mod)
    {
        return os << "\033[" << mod.code << "m";
    }
};

void BEConsoleIO::Output(BEString& BEString, const BEIOReporterSpecifier& Reporter)
{
    if(Reporter.Mode < Get().ModesUnderDisabled)
    {
        return;
    }
    std::cout << BEIOColorOutputModifier(Reporter.Code);
    std::cout << BETime::Now() << Reporter.BEReportString << ": " << BEString.Strip() << std::endl;
}