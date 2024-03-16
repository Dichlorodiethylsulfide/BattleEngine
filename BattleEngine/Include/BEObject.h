#pragma once

#include "BEAllocatorPolicy.h"

// Further implement BEObject and BEObjectPtr

/*
template<typename T BE_REQUIRES(!TIsPointer<T>::Value)>
struct THandle
{
    mutable UIntPtr Handle;

    THandle(T* ptr)
    {
        Handle = reinterpret_cast<UIntPtr>(ptr);
    }

    void Clear() const // assumes object was created via New
    {
        if(BE_LIKELY(*this))
        {
            Delete(reinterpret_cast<T*>(Handle));
            Handle = 0;
        }
    }

    BE_FORCEINLINE explicit operator bool() const
    {
        return Handle != 0;
    }
};
*/

class BEObject
{
    // Empty for now
};

template<typename T>
void Delete(T* Object)
{
    BE_CHECK(!Object)
    const auto& Allocator = GetAllocator();
    Allocator.Free(Object);
}

template<typename T, typename ... Args>
auto* New(Args&&... args)
{
    const auto& Allocator = GetAllocator();
    void* RawObject = Allocator.Malloc(sizeof(T));
    BE_CHECK(!RawObject)
    BEMemory::MemZero(RawObject, sizeof(T));
    T* Object = static_cast<T*>(RawObject);
    *Object = T(BEForward<Args&&>(args)...);
    return Object;
}
