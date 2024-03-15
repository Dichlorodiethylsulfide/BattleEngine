#include "BEString.h"

BEString::BEString()
{
    *this = "";
}

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

BEString& BEString::operator=(const CHAR* CString)
{
    Clear();
    const SizeType Length = GetLength(CString) + 1;
    InternalStack = BESmallObjectOptimizedStack(CString, Length);
    return *this;
}

BEString& BEString::operator=(const BEString& String)
{
    Clear();
    InternalStack = BESmallObjectOptimizedStack(String.CStr(), String.InternalStack.GetLength());
    return *this;
}

BEString& BEString::operator=(BEString&& String) noexcept
{
    InternalStack = BEMove<BESmallObjectOptimizedStack<CHAR>>(String.InternalStack);
    String.InternalStack.Clear();
    return *this;
}

const CHAR* BEString::CStr() const
{
    return InternalStack.GetReinterpretedPointer();
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
