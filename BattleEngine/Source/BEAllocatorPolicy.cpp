#include "BEAllocatorPolicy.h"

#include <cstdlib>

#include "BEMemory.h"

void* BEHeapAllocatorPolicy::Malloc(SizeType Bytes, SizeType Alignment) const
{
    void* Block = _aligned_malloc(Bytes, Alignment);
    if(!Block)
    {
        BEMemory::OnOutOfMemory();
        return nullptr;
    }
    BEMemory::MemZero(Block, Bytes);
    return Block;
}

void* BEHeapAllocatorPolicy::TryMalloc(SizeType Bytes, SizeType Alignment) const
{
    BEMemory::CheckMaxAlloc(Bytes);
    return Malloc(Bytes, Alignment);
}

void* BEHeapAllocatorPolicy::Realloc(void* Block, SizeType Bytes, SizeType Alignment) const
{
    void* ReBlock = _aligned_realloc(Block, Bytes, Alignment);
    if(!ReBlock)
    {
        BEMemory::OnOutOfMemory();
        return nullptr;
    }
    return ReBlock;
}

void* BEHeapAllocatorPolicy::TryRealloc(void* Block, SizeType Bytes, SizeType Alignment) const
{
    BEMemory::CheckMaxAlloc(Bytes);
    return Realloc(Block, Bytes, Alignment);
}

void BEHeapAllocatorPolicy::Free(void* Block) const
{
    if(Block)
    {
        _aligned_free(Block);
    }
}

SizeType BEHeapAllocatorPolicy::GetAllocationSize(void* Block, SizeType Alignment) const
{
    return _aligned_msize(Block, Alignment, 0);
}

const BEHeapAllocatorPolicy MainAllocator = BEHeapAllocatorPolicy();
const BEAllocatorPolicy& GetAllocator()
{
    return MainAllocator;
}
