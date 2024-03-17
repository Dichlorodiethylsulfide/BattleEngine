#include "BEMalloc.h"
#include "BEAtomic.h"
#include "BEMemory.h"
#include <map>
#include <ranges>

constexpr SizeType ElementCount = (sizeof(SizeType) * 8);

/*
 * 32 byte overhead for Block allocation
 */
struct BlockDescriptor
{
    void* Block;
    SizeType TotalSize;
    SizeType ElementSize;
    SizeType ElementsAvailable = 0; // this is not a traditional counter, this is a bitwise operation

    BE_FORCEINLINE bool ContainsObject(void* Object) const
    {
        const UIntPtr BlockAddr = reinterpret_cast<UIntPtr>(Block);
        const UIntPtr ObjectAddr = reinterpret_cast<UIntPtr>(Object);
        return ObjectAddr >= BlockAddr && ObjectAddr < BlockAddr + TotalSize;
    }

    BE_FORCEINLINE SizeType FindObjectIndex(void* Object) const
    {
        if(ContainsObject(Object))
        {
            const UIntPtr BlockAddr = reinterpret_cast<UIntPtr>(Block);
            const UIntPtr ObjectAddr = reinterpret_cast<UIntPtr>(Object);
            return ObjectAddr - BlockAddr;
        }
        return TGetIntLimit<SizeType>::GetMax(); // return an invalid index 
    }
    
    BE_FORCEINLINE SizeType FindNextAvailableIndex() const
    {
        if(!IsFull())
        {
            for(SizeType index = 0; index < ElementCount; index++)
            {
                if(!IsIndexUnavailable(index))
                {
                    return index;
                }
            }
        }
        return TGetIntLimit<SizeType>::GetMax(); // return an invalid index
    }

    BE_FORCEINLINE void SetIndex(SizeType Index, bool bIsUnavailable)
    {
        if(Index > ElementCount)
        {
            return;
        }
        bIsUnavailable ? ElementsAvailable |= (static_cast<SizeType>(1) << Index) : ElementsAvailable ^= (static_cast<SizeType>(1) << Index);
    }
    
    BE_FORCEINLINE bool IsIndexUnavailable(SizeType Index) const
    {
        if(Index > ElementCount)
        {
            return false;
        }
        return ElementsAvailable & (static_cast<SizeType>(1) << Index); // e.g. b1000 & (1 << 4) returns 4 so index 4 is unavailable
    }

    BE_FORCEINLINE bool IsFull() const
    {
        return Block && ElementsAvailable == TGetIntLimit<SizeType>::GetMax();
    }
    
    BE_FORCEINLINE bool IsEmpty() const
    {
        return !IsFull();
    }
};

struct BlockDirectory
{
    BlockDescriptor Descriptor;
    BlockDirectory* Next;
};

static TAtomic<SizeType> TotalAllocations = 0;
static std::map<SizeType, BlockDirectory> ObjectAllocationHashMap = {};

void* BETypedMemoryAllocation::Malloc(SizeType Hash, SizeType Size)
{
    if(ObjectAllocationHashMap.contains(Hash))
    {
        BlockDirectory* BDir = &ObjectAllocationHashMap[Hash];
        while(BDir->Descriptor.IsFull())
        {
            if(BDir->Next)
            {
                BDir = BDir->Next;
                continue;
            }
            BDir->Next = new BlockDirectory();
        }
        BlockDescriptor& BDesc = BDir->Descriptor;
        if(!BDesc.Block)
        {
            const SizeType TotalSize = ElementCount * Size;
            const auto& Allocator = GetAllocator();
            BDesc.Block = Allocator.Malloc(TotalSize);
            TotalAllocations += TotalSize;
            BDesc.ElementSize = Size;
            BDesc.TotalSize = TotalSize;
        }
        SizeType Index = BDesc.FindNextAvailableIndex();
        if(Index != TGetIntLimit<SizeType>::GetMax())
        {
            BDesc.SetIndex(Index, true);
            UInt8* FreePtr = static_cast<UInt8*>(BDesc.Block) + (Index * BDesc.ElementSize);
            BEMemory::MemZero(FreePtr, BDesc.ElementSize);
            return FreePtr;
        }
        PLATFORM_BREAK
        return nullptr;
    }
    ObjectAllocationHashMap.insert(std::make_pair(Hash, BlockDirectory()));
    return Malloc(Hash, Size);
}

void BETypedMemoryAllocation::Free(SizeType Hash, void* Object)
{
    // TODO: free very old blocks to save memory, for the time being old blocks are reused anyway ?
    if(ObjectAllocationHashMap.contains(Hash))
    {
        BlockDirectory* BDir = &ObjectAllocationHashMap[Hash];
        SizeType Index;
        while((Index = BDir->Descriptor.FindObjectIndex(Object)) == TGetIntLimit<SizeType>::GetMax())
        {
            BDir = BDir->Next;
        }
        TotalAllocations -= BDir->Descriptor.ElementSize;
        BDir->Descriptor.SetIndex(Index, false);
        return;
    }
    // We cannot free an object if its type does not exist in the hash map
    PLATFORM_BREAK
}

bool BETypedMemoryAllocation::CheckAddressInternal(SizeType Hash, void* Object)
{
    BlockDirectory* BDir = &ObjectAllocationHashMap[Hash];
    while(BDir->Next)
    {
        if(BDir->Descriptor.ContainsObject(Object))
        {
            return true;
        }
        BDir = BDir->Next;
    }
    return false;
}

bool BETypedMemoryAllocation::CheckAddressFast(SizeType Hash, void* Object)
{
    if(ObjectAllocationHashMap.contains(Hash))
    {
        return CheckAddressInternal(Hash, Object);
    }
    return false;
}

bool BETypedMemoryAllocation::CheckAddressSlow(void* Object)
{
    for(const auto key : ObjectAllocationHashMap | std::views::keys)
    {
        if(CheckAddressInternal(key, Object))
        {
            return true;
        }
    }
    return false;
}

SizeType BETypedMemoryAllocation::GetCurrentAllocations()
{
    return TotalAllocations;
}
