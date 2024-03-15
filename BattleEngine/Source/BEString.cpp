#include "BEString.h"

#include "BEMemory.h"
#include "BEAllocatorPolicy.h"

BEString::BEString() = default;

BEString::BEString(const CHAR* CString)
{
    *this = CString;
}

BEString::BEString(const BEString& String)
{
    *this = String;
}

BEString::BEString(BEString&& String) noexcept
{
    *this = BEMove<BEString>(String);
}

BEString::~BEString()
{
    Clear();
}

BEString& BEString::operator=(const CHAR* CString)
{
    Clear();
    BuildBEString(CString, *this);
    return *this;
}

BEString& BEString::operator=(const BEString& String)
{
    Clear();
    BuildBEString(String.CStr(), *this);
    return *this;
}

BEString& BEString::operator=(BEString&& String) noexcept
{
    InternalStringStack.InternalUnion.Data = String.InternalStringStack.InternalUnion.Data;
    String.InternalStringStack.Clear();
    return *this;
}

void BEString::Clear()
{
    InternalStringStack.Clear();
}

bool BEString::IsEmpty() const
{
    return InternalStringStack.IsEmpty();
}

uint64 BEString::GetLength() const
{
    return InternalStringStack.GetLength();
}

const CHAR* BEString::CStr() const
{
    return InternalStringStack.GetReinterpretedPointer();
}

bool BEString::GetIsSSO() const
{
    return InternalStringStack.GetIsStack();
}

SizeType BEString::GetLength(const CHAR* CString)
{
    SizeType Length = 0;
    while(CString[Length] != '\0' && Length != TGetIntLimit<SizeType>::GetMax())
    {
        Length++;
    }
    return Length;
}

void BEString::BuildBEString(const CHAR* CString, BEString& ThisString)
{
    SizeType Length = GetLength(CString) + 1;
    ThisString.InternalStringStack.SetElements(CString, Length);
}
