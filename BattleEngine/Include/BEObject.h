#pragma once

#include "BEAllocatorPolicy.h"
#include "BEAtomic.h"


// Further implement BEObject and BEObjectPtr

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

template<typename T BE_REQUIRES(!TIsPointer<T>::Value)>
struct THandle
{
    mutable UIntPtr Handle;

    THandle(T* ptr)
    {
        Handle = reinterpret_cast<UIntPtr>(ptr);
    }

    void Clear() const
    {
        if(this->operator bool())
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

template<typename T>
struct BESharedReference
{
    THandle<T> ReferencedObject = nullptr;
    mutable TAtomic<SizeType> StrongRefCount = 0;

    BESharedReference() = default;
    BESharedReference(T* ptr)
    {
        ReferencedObject = ptr;
        AddRef();
    }

    BE_FORCEINLINE void AddRef() const
    {
        ++StrongRefCount;
    }
    
    BE_FORCEINLINE void RemoveRef() const
    {
        if(--StrongRefCount == 0)
        {
            ReferencedObject.Clear();
            delete this;
        }
    }

    BE_FORCEINLINE bool IsValid() const
    {
        return StrongRefCount > 0;
    }
};


template<typename T BE_REQUIRES(!TIsPointer<T>::Value)>
class TReference
{
    BESharedReference<T>* Counter;

public:
    TReference(BESharedReference<T>* counter)
    {
        Counter = counter;
        if(Counter)
        {
            Counter->AddRef();
        }
    }

    ~TReference()
    {
        if(Counter)
        {
            Counter->RemoveRef();
        }
    }

    BE_FORCEINLINE auto& operator*()
    {
        BE_CHECK(!Counter)
        BE_CHECK(!Counter->ReferencedObject)
        return reinterpret_cast<T&>(Counter->ReferencedObject.Handle);
    }
};

template<typename T BE_REQUIRES(!TIsPointer<T>::Value)>
class TPointer
{
    BESharedReference<T>* Counter;

public:
    TPointer(BESharedReference<T>* counter)
    {
        Counter = counter;
        if(Counter)
        {
            Counter->AddRef();
        }
    }

    ~TPointer()
    {
        if(Counter)
        {
            Counter->RemoveRef();
        }
    }

    BE_FORCEINLINE auto* operator->()
    {
        BE_CHECK(!Counter)
        BE_CHECK(!Counter->ReferencedObject)
        return reinterpret_cast<T*>(Counter->ReferencedObject.Handle);
    }
};

template<typename T BE_REQUIRES(!TIsPointer<T>::Value)>
struct TSharedObject
{
    // At the moment keep this restriction, it does not matter yet because BEObject is not properly implemented
    static_assert(!TIsDerivedFrom<T, BEObject>::Value, "T cannot be a BEObject, please use BEObjectPtr");

    TSharedObject(T* ptr) // assumes ownership
    {
        ReferenceCounter = new BESharedReference<T>(ptr);
    }

    ~TSharedObject()
    {
        if(ReferenceCounter)
        {
            ReferenceCounter->RemoveRef();
        }
    }

    BE_FORCEINLINE TPointer<T> Get()
    {
        return ReferenceCounter;
    }

    BE_FORCEINLINE TReference<T> GetRef()
    {
        return ReferenceCounter;
    }
    
private:
    BESharedReference<T>* ReferenceCounter = nullptr;
};

template<typename T, typename ... Args>
auto* New(Args&&... args)
{
    const auto& Allocator = GetAllocator();
    void* RawObject = Allocator.Malloc(sizeof(T));
    BE_CHECK(!RawObject);
    BEMemory::MemZero(RawObject, sizeof(T));
    T* Object = static_cast<T*>(RawObject);
    *Object = T(BEForward<Args&&>(args)...);
    return Object;
}

template<typename T, typename ... Args>
auto MakeShared(Args&&... args)
{
    return TSharedObject<T>(New<T>(BEForward<Args>(args)...));
}
