#include "BEString.h"

#include "BEMemory.h"
#include "BEAllocatorPolicy.h"

BEString::BEString()
{
    SmallString = {};
    SetSSO(SmallString, true);
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
    String.SmallString = {};
    SetSSO(String.SmallString, true);
    return *this;
}

void BEString::Clear()
{
    if(!IsEmpty())
    {
        if(!IsSSO(SmallString))
        {
            BE::MainAllocator.Free(Data);
        }
        SmallString = {};
        SetSSO(SmallString, true);
    }
}

bool BEString::IsEmpty() const
{
    return !Data;
}

const CHAR* BEString::CStr() const
{
    return
        IsSSO(SmallString) ?
        SmallString.AsCString() :
        Data;
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
    ThisString.SmallString = {}; // clear data
    if(Length > sizeof(SSO) - 2) // last byte is SSO flag, second last byte is '\0'
    {
        SetSSO(ThisString.SmallString, false);
        Length++; // include last character
        ThisString.Data = static_cast<CHAR*>(BE::MainAllocator.TryMalloc(Length));
        BEMemory::MemCopy(ThisString.Data, CString, Length);
    }
    else
    {
        BEMemory::MemCopy(&ThisString.SmallString, CString, Length);
        SetSSO(ThisString.SmallString, true);
    }
}

void BEString::SetSSO(SSO& SmallString, bool bIsSSO)
{
    // Find the last byte in the SmallString and set it to True or False
    BEMemory::BEByteOperations<int8>::Set(&SmallString, sizeof(SSO),  sizeof(SSO) - 1, bIsSSO ? 1 : 0);
}

bool BEString::IsSSO(const SSO& SmallString)
{
    // Find the last byte in the SmallString and return it
    return BEMemory::BEByteOperations<int8>::Get(&SmallString, sizeof(SSO), sizeof(SSO) - 1);
}
