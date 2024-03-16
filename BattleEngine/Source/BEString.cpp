#include "BEString.h"

BEString::BEString()
{
    *this = "";
}

BEString::BEString(const Char* CString)
{
    *this = CString;
}

BEString::BEString(const BEString& String)
{
    *this = String;
}

BEString::BEString(BEString&& String) noexcept
{
    *this = BEMove(String);
}

BEString& BEString::operator=(const Char* CString)
{
    const SizeType Length = GetLength(CString) + 1;
    InternalStack = BESmallObjectOptimizedStack(CString, Length);
    return *this;
}

BEString& BEString::operator=(const BEString& String)
{
    InternalStack = BESmallObjectOptimizedStack(String.CStr(), String.InternalStack.GetLength());
    return *this;
}

BEString& BEString::operator=(BEString&& String) noexcept
{
    InternalStack = BEMove(String.InternalStack);
    String = "";
    return *this;
}

const Char* BEString::CStr() const
{
    return InternalStack.GetReinterpretedPointer();
}

SizeType BEString::GetLength(const Char* CString)
{
    SizeType Length = 0;
    while(CString[Length] != '\0' && Length != TGetIntLimit<SizeType>::GetMax())
    {
        Length++;
    }
    return Length;
}
