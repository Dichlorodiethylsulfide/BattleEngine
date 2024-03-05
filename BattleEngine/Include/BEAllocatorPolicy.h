#pragma once
#include "BETypeTraits.h"

struct BEMallocAlignment
{
    enum
    {
        Value = 16
    };
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


class BEMainAllocatorPolicy : public BEAllocatorPolicy
{
public:
    ~BEMainAllocatorPolicy() override = default;
    BE_DEFAULT_CONSTRUCTION(BEMainAllocatorPolicy)
    void* Malloc(SizeType Bytes, SizeType Alignment = BEMallocAlignment::Value) const override;
    void* TryMalloc(SizeType Bytes, SizeType Alignment = BEMallocAlignment::Value) const override;
    void* Realloc(void* Block, SizeType Bytes, SizeType Alignment = BEMallocAlignment::Value) const override;
    void* TryRealloc(void* Block, SizeType Bytes, SizeType Alignment = BEMallocAlignment::Value) const override;
    void Free(void* Block) const override;
    SizeType GetAllocationSize(void* Block, SizeType Alignment = BEMallocAlignment::Value) const override;
};

namespace BE
{
    static const BEMainAllocatorPolicy MainAllocator = BEMainAllocatorPolicy();
}
