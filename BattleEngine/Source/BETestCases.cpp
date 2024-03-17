#include <iostream>
#include "BEAtomic.h"
#include "BEShared.h"
#include "BEString.h"
#include "BEConsoleIO.h"
#include "BETime.h"
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
    COut << BE_STRINGIFY(x) << TEXT(" failed") << TEXT("\n"); \
}

#define BE_CHECK_TEST(x) \
if(BE_UNLIKELY(x)) \
{ \
COut << BE_STRINGIFY(x) << TEXT(" failed the check") << TEXT("\n"); \
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
    // Make this its own function to ensure the TShared objects are deleted at the end
    auto CustomPtr = MakeSharedPtr<CustomType>();
    BE_REQUIRES_TEST(CustomPtr->Value == 0)
    auto CustomPtr2 = MakeSharedPtr<CustomType>(100);
    BE_REQUIRES_TEST(CustomPtr2->Value == 100)
    BE_REQUIRES_TEST(!TSharedPtr<CustomType>().IsValid())
    BE_REQUIRES_TEST(!TSharedPtr<CustomType>(nullptr).IsValid())
    auto CustomRef = MakeSharedRef<CustomType>();
    BE_REQUIRES_TEST(CustomRef->Value == 0)
    auto CustomRef2 = MakeSharedRef<CustomType>(100);
    BE_REQUIRES_TEST(CustomRef2->Value == 100)
    // TSharedRef<CustomType>() not allowed
    // TSharedRef<CustomType>(nullptr) not allowed
}

#include "BEMalloc.h"

struct TypeA { int A; };
struct TypeB { int B; };

int main(int argc, char* argv[])
{
    // Time
    const auto Now = BETime::Now();
    BETime::WaitFor(BETime::Milliseconds(1000));
    const auto TimeWaited = (BETime::Now() - Now).As<BETime::Milliseconds>();
    // Waiting does not have perfect accuracy
    // However, it is guaranteed to wait at least the amount of time specified
    // On average, waiting for 1000 milliseconds yields 1020 milliseconds so 1100 is a HIGH upper bound
    BE_REQUIRES_TEST(1000 < TimeWaited && TimeWaited < 1100)
    BE_CHECK_TEST(1000 < TimeWaited && TimeWaited < 1030)
    // Time
    // Allocation / Deallocation
    // Double check hashes don't match for near-identical types
    SizeType HashA = typeid(TypeA).hash_code();
    SizeType HashB = typeid(TypeB).hash_code();
    BE_REQUIRES_TEST(HashA != HashB)
    constexpr int Size = 1024;
    int* Datas[Size];
    for (int i = 0; i < Size; i++)
    {
        Datas[i] = BETypedMemoryAllocation::TMalloc<int>();
        *Datas[i] = i;
    }
    for (int i = 0; i < Size; i++)
    {
        BE_REQUIRES_TEST(*Datas[i] == i)
        BETypedMemoryAllocation::TFree(Datas[i]);
    }
    // We freed all the memory we allocated so we should not have any current allocations (of any size)
    BE_REQUIRES_TEST(BETypedMemoryAllocation::GetCurrentAllocations() == 0)
    TestSharedObjects(); // Creating shared objects adds to the BETypedMemoryAllocation table
    BE_REQUIRES_TEST(BETypedMemoryAllocation::GetCurrentAllocations() == 0) // they should all get freed upon exiting the function
    // Allocation / Deallocation
    // BEString
    BEString String(TEXT("Hello World Hello World Hello"));
    BE_REQUIRES_TEST(String.IsSSO())
    BE_REQUIRES_TEST(BEString::GetLength(String.CStr()) == 29)
    BE_REQUIRES_TEST(String.Length() == 30)
    BEString LongString(TEXT("THIS IS A LONG STRING VERY LONG VERY NICE BUT ALSO VERY LONG HAVE I TOLD YOU HOW LONG IT IS"));
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