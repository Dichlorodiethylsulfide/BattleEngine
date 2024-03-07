#include "BEString.h"

#include "BEMemory.h"
#include "BEAllocatorPolicy.h"

BEString::BEString()
{
    Union.Data = {true, 0};
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
    Union.Data = String.Union.Data;
    String.Union.Data = {true, 0};
    return *this;
}

void BEString::Clear()
{
    if(!IsEmpty())
    {
        if(!Union.Data.IsSSO())
        {
            BE::MainAllocator.Free(Union.Other.Pointer);
        }
        Union.Data = {true, 0};
    }
}

bool BEString::IsEmpty() const
{
    return !Union.Other.Pointer;
}

uint64 BEString::GetLength() const
{
    return Union.Data.IsSSO() ?
        Union.Data.GetLength() :
        Union.Other.Length;
}

const CHAR* BEString::CStr() const
{
    return
        Union.Data.IsSSO() ?
        Union.Data.AsCString() :
        Union.Other.Pointer;
}

bool BEString::GetIsSSO() const
{
    return Union.Data.IsSSO();
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
    SizeType Length = GetLength(CString);
    ThisString.Union.Data = { true, 0 }; // clear data
    if(Length > sizeof(SmallStringStack) - SmallStringStack::EndBytePositionMax)
        // last byte is SmallStringStack flag, second last byte is length, third last byte is '\0'
    {
        ThisString.Union.Data.SetSSO(false);
        Length++; // include last character
        ThisString.Union.Other = { static_cast<CHAR*>(BE::MainAllocator.TryMalloc(Length)), Length };
        BEMemory::MemCopy(ThisString.Union.Other.Pointer, CString, Length);
    }
    else
    {
        BEMemory::MemCopy(&ThisString.Union.Data, CString, Length);
        ThisString.Union.Data.SetSSO(true);
        ThisString.Union.Data.SetLength(static_cast<uint8>(Length));
    }
}
