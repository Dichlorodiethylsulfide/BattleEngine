#pragma once
#include <vcruntime_typeinfo.h>
#include "BEAllocatorPolicy.h"

struct BETypedMemoryAllocation
{
    static void* Malloc(SizeType Hash, SizeType Size);
    static void Free(SizeType Hash, void* Object);
    static bool CheckAddressFast(SizeType Hash, void* Object);
    static bool CheckAddressSlow(void* Object);
    static SizeType GetCurrentAllocations();
    static SizeType GetCurrentTotalAllocations();

private:
    // Assumes 'Hash' is valid
    static bool CheckAddressInternal(SizeType Hash, void* Object);
    
public:
    template<typename T BE_REQUIRES(!TIsConst<T>::Value && !TIsReference<T>::Value && !TIsVolatile<T>::Value)>
    static T* TMalloc()
    {
        return static_cast<T*>(BETypedMemoryAllocation::Malloc(typeid(T).hash_code(), sizeof(T)));
    }

    template<typename T BE_REQUIRES(!TIsConst<T>::Value && !TIsReference<T>::Value && !TIsVolatile<T>::Value)>
    static void TFree(T* Object)
    {
        BETypedMemoryAllocation::Free(typeid(T).hash_code(), Object);
    }

    template<typename T BE_REQUIRES(!TIsConst<T>::Value && !TIsReference<T>::Value && !TIsVolatile<T>::Value)>
    static bool TCheckAddress(T* Object)
    {
        return BETypedMemoryAllocation::CheckAddressFast(typeid(T).hash_code(), Object);
    }
};