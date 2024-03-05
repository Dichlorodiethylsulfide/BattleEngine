#include "BEAllocatorPolicy.h"

#include <cstdlib>

#include "BEMemory.h"

void* BEMainAllocatorPolicy::Malloc(SizeType Bytes, SizeType Alignment) const
{
    void* Block = _aligned_malloc(Bytes, Alignment);
    if(!Block)
    {
        BEMemory::OnOutOfMemory();
        return nullptr;
    }
    return Block;
}

void* BEMainAllocatorPolicy::TryMalloc(SizeType Bytes, SizeType Alignment) const
{
    BEMemory::CheckMaxAlloc(Bytes);
    return Malloc(Bytes, Alignment);
}

void* BEMainAllocatorPolicy::Realloc(void* Block, SizeType Bytes, SizeType Alignment) const
{
    void* ReBlock = _aligned_realloc(Block, Bytes, Alignment);
    if(!ReBlock)
    {
        BEMemory::OnOutOfMemory();
        return nullptr;
    }
    return ReBlock;
}

void* BEMainAllocatorPolicy::TryRealloc(void* Block, SizeType Bytes, SizeType Alignment) const
{
    BEMemory::CheckMaxAlloc(Bytes);
    return Realloc(Block, Bytes, Alignment);
}

void BEMainAllocatorPolicy::Free(void* Block) const
{
    if(Block)
    {
        _aligned_free(Block);
    }
}

SizeType BEMainAllocatorPolicy::GetAllocationSize(void* Block, SizeType Alignment) const
{
    return _aligned_msize(Block, Alignment, 0);
}
