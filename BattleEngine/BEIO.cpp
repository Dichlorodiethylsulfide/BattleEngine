#include "BEIO.h"
#include "BETime.h"

#include<iostream>

void BEIO::Output(const char* BEString, const char* Mode)
{
    std::cout << "[" << BETime::Now().GetTimeBuffer() << "] " << Mode << ": " << BEString << std::endl;
}
