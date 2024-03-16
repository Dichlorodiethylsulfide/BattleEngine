#include <iostream>
#include "BEAtomic.h"
#include "BEObject.h"
#include "BEString.h"
#include "BEConsoleIO.h"
// Use this file for running live test cases

static TAtomic<SizeType> Assertions = 0;
static TAtomic<SizeType> Successes = 0;

#define BE_REQUIRES_TEST(x) \
++Assertions; \
if(BE_LIKELY(x)) \
{ \
++Successes; \
} \
else \
{ \
    std::cout << BE_STRINGIFY(x) << " failed" << "\n"; \
}

#define BE_CHECK_ALL_TESTS std::cout << BEIOColorOutputModifier(Assertions == Successes ? BE_FG_GREEN : BE_FG_RED) << Successes << " out of " << Assertions << " assertions were successful" << "\n";

struct CustomType
{
    CustomType() = default;
    CustomType(int value) { Value = value; }
    int Value;
};

void TestSharedObjects()
{
    // Make this its own function to ensure the TSharedObjects are deleted at the end
    auto Custom = MakeShared<CustomType>();
    auto CustomPtr = Custom.Get();
    BE_REQUIRES_TEST(CustomPtr->Value == 0)
    auto Custom2 = MakeShared<CustomType>(100);
    auto CustomPtr2 = Custom2.Get();
    BE_REQUIRES_TEST(CustomPtr2->Value == 100)
}

int main(int argc, char* argv[])
{
    BE_CHECK(true)
    // New / Delete
    TestSharedObjects();
    // New / Delete
    // BEString
    BEString String("Hello World Hello World Hello");
    BE_REQUIRES_TEST(String.IsSSO())
    BE_REQUIRES_TEST(BEString::GetLength(String.CStr()) == 29)
    BE_REQUIRES_TEST(String.Length() == 30)
    BEString LongString("THIS IS A LONG STRING VERY LONG VERY NICE BUT ALSO VERY LONG HAVE I TOLD YOU HOW LONG IT IS");
    BE_REQUIRES_TEST(!LongString.IsSSO())
    BE_REQUIRES_TEST(BEString::GetLength(LongString.CStr()) == 91)
    BE_REQUIRES_TEST(LongString.Length() == 92)
    BEString EmptyString;
    BE_REQUIRES_TEST(EmptyString.IsSSO())
    BE_REQUIRES_TEST(BEString::GetLength(EmptyString.CStr()) == 0)
    BE_REQUIRES_TEST(EmptyString.Length() == 1)
    BEString MovedString = BEMove(LongString);
    BE_REQUIRES_TEST(!MovedString.IsSSO())
    BE_REQUIRES_TEST(BEString::GetLength(MovedString.CStr()) == 91)
    BE_REQUIRES_TEST(MovedString.Length() == 92)
    BE_REQUIRES_TEST(LongString.IsSSO())
    BE_REQUIRES_TEST(BEString::GetLength(LongString.CStr()) == 0)
    BE_REQUIRES_TEST(LongString.Length() == 1)
    // BEString
    BE_CHECK_ALL_TESTS
}