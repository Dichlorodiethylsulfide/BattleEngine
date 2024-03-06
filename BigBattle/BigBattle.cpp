
// Based on Touhou Big Big Battle + Unreal Engine

#include "BEString.h"
#include <iostream>

int main(int argc, char* argv[])
{
    /*BEString String("Hello World Hello World HelloAA");
    std::cout << String.CStr() << " " << String.IsEmpty() << std::endl;
    BEString LongString("THIS IS A LONG SENTENCE AHHHHHH OKAY I AM DONE NOW");
    std::cout << LongString.CStr() << " " << String.IsEmpty() << std::endl;
    BEString EmptyString;
    std::cout << EmptyString.IsEmpty() << std::endl;*/
    BEString String = "Hello World";
    BEString MovedString = BEMove<BEString>(String);
    std::cout << String.CStr() << std::endl;
    std::cout << MovedString.CStr() << std::endl;
    return 0;
}