
// Based on Touhou Big Big Battle + Unreal Engine

#include "BEString.h"
#include <iostream>

int main(int argc, char* argv[])
{
    BEString String("Hello World Hello World Hello");
    std::cout << String.CStr() << " " << String.GetIsSSO() << " " << String.GetLength() << std::endl;
    BEString LongString("THIS IS A LONG SENTENCE AHHHHHH OKAY I AM DONE NOW");
    std::cout << LongString.CStr() << " " << LongString.GetIsSSO() << " " << LongString.GetLength() << std::endl;
    BEString EmptyString;
    std::cout << EmptyString.CStr() << " " << EmptyString.GetIsSSO() << " " << EmptyString.GetLength() << std::endl;
    return 0;
}