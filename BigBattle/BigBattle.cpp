
// Based on Touhou Big Big Battle + Unreal Engine

#include "BEString.h"
#include <iostream>



int main(int argc, char* argv[])
{
    BEString String("Hello World Hello World Hello");
    std::cout << String.CStr() << " " << String.GetIsSSO() << " " << String.Length() << "\n";
    BEString LongString("THIS IS A LONG STRING VERY LONG VERY NICE BUT ALSO VERY LONG HAVE I TOLD YOU HOW LONG IT IS");
    std::cout << LongString.CStr() << " " << LongString.GetIsSSO() << " " << LongString.Length() << "\n";
    BEString EmptyString;
    std::cout << EmptyString.CStr() << "Empty " << EmptyString.GetIsSSO() << " " << EmptyString.Length() << "\n";
    return 0;
}
