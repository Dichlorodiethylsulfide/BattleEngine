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
    SizeType Length = GetLength(CString);
    SmallString = {}; // clear data
    if(Length > sizeof(SSO) - 2) // last byte is SSO flag, second last byte is '\0'
    {
        SetSSO(SmallString, false);
        Length++; // include last character
        Data = static_cast<CHAR*>(BE::MainAllocator.Malloc(Length));
        BEMemory::MemCopy(Data, CString, Length);
    }
    else
    {
        BEMemory::MemCopy(&SmallString, CString, Length);
        SetSSO(SmallString, true);
    }
}

BEString::~BEString()
{
    Clear();
}

void BEString::Clear()
{
    if(!IsSSO(SmallString))
    {
        BE::MainAllocator.Free(Data);
    }
    SmallString = {};
    SetSSO(SmallString, true);
}

const CHAR* BEString::CStr() const
{
    return
        IsSSO(SmallString) ?
        reinterpret_cast<const CHAR*>(&SmallString.High) :
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

void BEString::SetSSO(SSO& SmallString, bool bIsSSO)
{
    // Find the last byte in the SmallString and set it to True or False
    BEMemory::BEByteOperations::SetByte(&SmallString, sizeof(SSO),  31, bIsSSO ? 1 : 0);
}

bool BEString::IsSSO(const SSO& SmallString)
{
    // Find the last byte in the SmallString and return it
    return BEMemory::BEByteOperations::GetByte(&SmallString, sizeof(SSO), 31);
}
