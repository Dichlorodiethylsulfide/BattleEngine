#include "BEString.h"

BEString::BEString()
{
    *this = TEXT("");
}

BEString::BEString(SizeType Length)
    : Base(Length)
{
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
    m_InternalStack = BESmallObjectOptimizedStack(CString, Length);
    return *this;
}

BEString& BEString::operator=(const BEString& String)
{
    m_InternalStack = BESmallObjectOptimizedStack(String.CStr(), String.m_InternalStack.GetLength());
    return *this;
}

BEString& BEString::operator=(BEString&& String) noexcept
{
    m_InternalStack = BEMove(String.m_InternalStack);
    String = TEXT("");
    return *this;
}

const Char* BEString::CStr() const
{
    return m_InternalStack.GetReinterpretedPointer();
}

Char* BEString::CBuffer()
{
    return m_InternalStack.GetReinterpretedPointer();
}

SizeType BEString::GetLength(const Char* CString)
{
    SizeType Length = 0;
    while(CString[Length] != TEXT('\0') && Length != TGetIntLimit<SizeType>::GetMax())
    {
        Length++;
    }
    return Length;
}
