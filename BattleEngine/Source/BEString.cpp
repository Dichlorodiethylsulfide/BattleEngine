#include "BEString.h"

#include "BEMemory.h"
#include "BEAllocatorPolicy.h"

BEString::BEString()
{
    SmallString = {true, 0};
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
    SmallString = String.SmallString;
    String.SmallString = {true, 0};
    return *this;
}

void BEString::Clear()
{
    if(!IsEmpty())
    {
        if(!SmallString.IsSSO())
        {
            BE::MainAllocator.Free(Data.Pointer);
        }
        SmallString = {true, 0};
    }
}

bool BEString::IsEmpty() const
{
    return !Data.Pointer;
}

uint64 BEString::GetLength() const
{
    return SmallString.IsSSO() ?
        SmallString.GetLength() :
        Data.Length;
}

const CHAR* BEString::CStr() const
{
    return
        SmallString.IsSSO() ?
        SmallString.AsCString() :
        Data.Pointer;
}

bool BEString::GetIsSSO() const
{
    return SmallString.IsSSO();
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
    ThisString.SmallString = { true, 0 }; // clear data
    if(Length > sizeof(SmallStringStack) - SmallStringStack::EndBytePositionMax)
        // last byte is SmallStringStack flag, second last byte is length, third last byte is '\0'
    {
        ThisString.SmallString.SetSSO(false);
        Length++; // include last character
        ThisString.Data = { static_cast<CHAR*>(BE::MainAllocator.TryMalloc(Length)), Length };
        BEMemory::MemCopy(ThisString.Data.Pointer, CString, Length);
    }
    else
    {
        BEMemory::MemCopy(&ThisString.SmallString, CString, Length);
        ThisString.SmallString.SetSSO(true);
        ThisString.SmallString.SetLength(static_cast<uint8>(Length));
    }
}
