#include "BEMemory.h"

void BEMemory::MemZero(void* Block, SizeType Bytes)
{
    UInt8* Pointer = static_cast<UInt8*>(Block);
    for(SizeType i = 0; i < Bytes; i++)
    {
        Pointer[i] = '\0';
    }
}

void BEMemory::MemCopy(void* Destination, const void* Source, SizeType Bytes)
{
    UInt8* DestinationPtr = static_cast<UInt8*>(Destination);
    const UInt8* SourcePtr = static_cast<const UInt8*>(Source);
    for(SizeType i = 0; i < Bytes; i++)
    {
        DestinationPtr[i] = SourcePtr[i];
    }
}

void BEMemory::MemMove(void* Destination, void* Source, SizeType Bytes)
{
    UInt8* DestinationPtr = static_cast<UInt8*>(Destination);
    UInt8* SourcePtr = static_cast<UInt8*>(Source);
    for(SizeType i = 0; i < Bytes; i++)
    {
        DestinationPtr[i] = SourcePtr[i];
        SourcePtr[i] = 0;
    }
}

void BEMemory::OnOutOfMemory()
{
    PLATFORM_BREAK("Out of memory")
}

constexpr UInt64 MaxAlloc = static_cast<UInt64>(1024) * 1024 * 1024; // Worry if we're allocating over 1GB at a time
void BEMemory::CheckMaxAlloc(SizeType Bytes)
{
    if(Bytes >= MaxAlloc)
    {
        PLATFORM_BREAK("Allocation is too big")
    }
}
