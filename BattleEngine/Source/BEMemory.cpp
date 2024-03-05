#include "BEMemory.h"

void BEMemory::MemZero(void* Block, SizeType Bytes)
{
    uint8* Pointer = static_cast<uint8*>(Block);
    for(SizeType i = 0; i < Bytes; i++)
    {
        Pointer[i] = 0;
    }
}

void BEMemory::MemCopy(void* Destination, const void* Source, SizeType Bytes)
{
    uint8* DestinationPtr = static_cast<uint8*>(Destination);
    const uint8* SourcePtr = static_cast<const uint8*>(Source);
    for(SizeType i = 0; i < Bytes; i++)
    {
        DestinationPtr[i] = SourcePtr[i];
    }
}

void BEMemory::MemMove(void* Destination, void* Source, SizeType Bytes)
{
    uint8* DestinationPtr = static_cast<uint8*>(Destination);
    uint8* SourcePtr = static_cast<uint8*>(Source);
    for(SizeType i = 0; i < Bytes; i++)
    {
        DestinationPtr[i] = SourcePtr[i];
        SourcePtr[i] = 0;
    }
}

void BEMemory::OnOutOfMemory()
{
#if _WIN32
    __debugbreak();
#else
#error Invalid platform
#endif
}

constexpr uint64 MaxAlloc = static_cast<uint64>(1024) * 1024 * 1024; // Worry if we're allocating over 1GB at a time
void BEMemory::CheckMaxAlloc(SizeType Bytes)
{
    if(Bytes >= MaxAlloc)
    {
#if _WIN32
        __debugbreak();
#else
#error Invalid platform
#endif
    }
}
