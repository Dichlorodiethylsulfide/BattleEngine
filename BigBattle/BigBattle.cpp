
// Based on Touhou Big Big Battle + Unreal Engine

#include "BEString.h"
#include <iostream>

#include "BEAllocatorPolicy.h"

int main(int argc, char* argv[])
{
    BEString String("Hello World Hello World Hello");
    std::cout << String.CStr() << " " << String.GetIsSSO() << " " << String.GetLength() << std::endl;
    BEString LongString("THIS IS A LONG STRING VERY LONG VERY NICE BUT ALSO VERY LONG HAVE I TOLD YOU HOW LONG IT IS");
    std::cout << LongString.CStr() << " " << LongString.GetIsSSO() << " " << LongString.GetLength() << std::endl;
    BEString EmptyString;
    std::cout << EmptyString.CStr() << "Empty " << EmptyString.GetIsSSO() << " " << EmptyString.GetLength() << std::endl;
    return 0;
}
