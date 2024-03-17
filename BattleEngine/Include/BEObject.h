#pragma once

#include "BEMalloc.h"

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
    BETypedMemoryAllocation::TFree<T>(Object);
}

template<typename T, typename ... Args>
auto* New(Args&&... args)
{
    T* RawObject = BETypedMemoryAllocation::TMalloc<T>();
    BE_CHECK(!RawObject)
    *RawObject = T(BEForward<Args&&>(args)...);
    return RawObject;
}
