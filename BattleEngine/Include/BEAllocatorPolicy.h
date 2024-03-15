#pragma once
#include "BEMemory.h"

struct BEMallocAlignment
{
    enum
    {
        Value = 16
    };
};

struct BESmallAllocationAmount
{
    enum
    {
        Count = 8,
        Value = sizeof(uint64) * Count
    };
};

enum class BEAllocationMethod
{
    Heap,
    SmallAllocationOptimized
};

class BEAllocatorPolicy
{
public:
    virtual ~BEAllocatorPolicy() = default;
    BE_DEFAULT_CONSTRUCTION(BEAllocatorPolicy)
    virtual void* Malloc(SizeType Bytes, SizeType Alignment = BEMallocAlignment::Value) const = 0;
    virtual void* TryMalloc(SizeType Bytes, SizeType Alignment = BEMallocAlignment::Value) const = 0;
    virtual void* Realloc(void* Block, SizeType Bytes, SizeType Alignment = BEMallocAlignment::Value) const = 0;
    virtual void* TryRealloc(void* Block, SizeType Bytes, SizeType Alignment = BEMallocAlignment::Value) const = 0;
    virtual void Free(void* Block) const = 0;
    virtual SizeType GetAllocationSize(void* Block, SizeType Alignment = BEMallocAlignment::Value) const = 0;
};

class BEHeapAllocatorPolicy : public BEAllocatorPolicy
{
public:
    ~BEHeapAllocatorPolicy() override = default;
    BE_DEFAULT_CONSTRUCTION(BEHeapAllocatorPolicy)
    void* Malloc(SizeType Bytes, SizeType Alignment = BEMallocAlignment::Value) const override;
    void* TryMalloc(SizeType Bytes, SizeType Alignment = BEMallocAlignment::Value) const override;
    void* Realloc(void* Block, SizeType Bytes, SizeType Alignment = BEMallocAlignment::Value) const override;
    void* TryRealloc(void* Block, SizeType Bytes, SizeType Alignment = BEMallocAlignment::Value) const override;
    void Free(void* Block) const override;
    SizeType GetAllocationSize(void* Block, SizeType Alignment = BEMallocAlignment::Value) const override;
};

extern const BEHeapAllocatorPolicy MainAllocator;
extern const BEAllocatorPolicy& GetAllocator();


template<typename T>
struct BESmallObjectOptimizedStack
{
    TUnion<TStackBlock<BESmallAllocationAmount::Value>, THeapPointer<T>> InternalUnion;

    using Type = BESmallObjectOptimizedStack;
    
    enum // End Byte Position - EnumValue
    {
        IsStack = 1,
        Length = 2,
        EndBytePositionMax
    };

    BESmallObjectOptimizedStack()
    {
        Clear();
    }

    BE_FORCEINLINE static constexpr uint64 GetActualSizeLimit()
    {
        // We can contain up to 'Data' bytes (64) but are limited to 64 - EndBytePositionMax because the last bytes are used for flags + buffers
        return sizeof(Type) - EndBytePositionMax;
    }

    BE_FORCEINLINE bool IsEmpty() const
    {
        return GetLength() == 0;
    }

    BE_FORCEINLINE void Clear()
    {
        if(!GetIsStack())
        {
            // Change -> cannot guarantee allocator in the future, move to ObjectPtr?
            GetAllocator().Free(InternalUnion.Other.Pointer.Get());
        }
        BEMemory::MemZero(this, sizeof(Type));
        SetIsStack(true);
    }

    // Might move this logic to THeapPointer (re-implemented as TPointer to hide raw pointers for safety)
    BE_FORCEINLINE T* GetReinterpretedPointer()
    {
        return GetIsStack() ? 
            reinterpret_cast<T*>(this) :
            InternalUnion.Other.Pointer.Get();
    }

    BE_FORCEINLINE const T* GetReinterpretedPointer() const
    {
        return GetIsStack() ? 
            reinterpret_cast<const T*>(this) :
            InternalUnion.Other.Pointer.Get();
    }
    // Might move this logic to THeapPointer (re-implemented as TPointer to hide raw pointers for safety)

    BE_FORCEINLINE void SetIsStack(bool bIsStack)
    {
        BEMemory::BEByteOperations<uint8>::Set(this, sizeof(Type),  sizeof(Type) - IsStack, bIsStack ? 1 : 0);
    }

    BE_FORCEINLINE bool GetIsStack() const
    {
        return BEMemory::BEByteOperations<uint8>::Get(this, sizeof(Type), sizeof(Type) - IsStack);
    }

    BE_FORCEINLINE void SetLength(SizeType Length)
    {
        if(GetIsStack())
        {
            if(Length * sizeof(T) > GetActualSizeLimit())
            {
                // Cannot set the Stack to contain more elements than is available in 64 bytes
                __debugbreak();
            }
            BEMemory::BEByteOperations<uint8>::Set(this, sizeof(Type),  sizeof(Type) - BESmallObjectOptimizedStack::Length, static_cast<uint8>(Length));
        }
        else
        {
            InternalUnion.Other.Length = Length;
        }
    }

    BE_FORCEINLINE SizeType GetLength() const
    {
        return GetIsStack() ?
            BEMemory::BEByteOperations<uint8>::Get(this, sizeof(Type), sizeof(Type) - Length) :
            InternalUnion.Other.Length;
    }

    BE_FORCEINLINE void SetElements(const T* Elements, SizeType Length)
    {
        Clear();
        SetIsStack(Length < GetActualSizeLimit()); 
        T* ThisElementsPointer;
        SizeType Bytes = sizeof(T) * Length;
        if(GetIsStack())
        {
            ThisElementsPointer = GetReinterpretedPointer();
        }
        else
        {
            const auto& Allocator = GetAllocator();
            ThisElementsPointer = static_cast<T*>(Allocator.Malloc(Bytes));
            BE_CHECK(!ThisElementsPointer);
            BEMemory::MemZero(ThisElementsPointer, Bytes);
            InternalUnion.Other.Pointer = ThisElementsPointer;
        }
        SetLength(Length);
        BEMemory::MemCopy(ThisElementsPointer, Elements, Bytes);
    }
};

/*
template<typename T, BEAllocationMethod AllocMethod =
    TChooseConstant<( // Try very hard to get away with doing an initial stack allocation
        sizeof(T) > BESmallAllocationAmount::Value ||
        sizeof(T) * BESmallAllocationAmount::Count > BESmallAllocationAmount::Value),
    BEAllocationMethod,
    BEAllocationMethod::Heap,
    BEAllocationMethod::SmallAllocationOptimized>::Value>
struct BEAllocator
{
    typename TChooseType<(AllocMethod == BEAllocationMethod::SmallAllocationOptimized), BEStackAllocatorPolicy<T, BESmallAllocationAmount::Count>, BEHeapAllocatorPolicy>::Type
    AllocationPolicy = {};
};

std::cout << sizeof(BEAllocator<uint64, BEAllocationMethod::Stack>::AllocationPolicy) << std::endl;
*/
// Decide how the policy and Allocator themselves interact
// 