#pragma once

#include "BEAllocatorPolicy.h"
#include "BEAtomic.h"

class BEObject
{
    // Empty for now
};


template<typename T, typename ... Args>
T* New(Args&&... args)
{
    const auto& Allocator = GetAllocator();
    void* RawObject = Allocator.Malloc(sizeof(T));
    BE_CHECK(!RawObject);
    BEMemory::MemZero(RawObject, sizeof(T));
    T* Object = static_cast<T*>(RawObject);
    *Object = T(BEForward<Args&&>(args)...);
    return Object;
}

template<typename T>
void Delete(T* Object)
{
    BE_CHECK(!Object)
    const auto& Allocator = GetAllocator();
    Allocator.Free(Object);
}

///////////////////////////////////////

/*
template<typename T BE_REQUIRES(!TIsPointer<T>::Value)>
struct THandle
{
    mutable UIntPtr Handle;

    THandle(T* ptr)
    {
        Handle = reinterpret_cast<UIntPtr>(ptr);
    }

    BE_FORCEINLINE void Free() const
    {
        // Change -> Dispatch new allocations and clear up old allocations
        GetAllocator().Free(reinterpret_cast<T*>(Handle));
        Handle = 0;
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
            ReferencedObject.Free();
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
        BE_CHECK(Counter)
        BE_CHECK(Counter->ReferencedObject.Handle)
        return reinterpret_cast<T&>(Counter->ReferencedObject);
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

    BE_FORCEINLINE auto* operator*()
    {
        BE_CHECK(Counter)
        BE_CHECK(Counter->ReferencedObject.Handle)
        return reinterpret_cast<T*>(Counter->ReferencedObject);
    }
};

template<typename T BE_REQUIRES(!TIsPointer<T>::Value)>
struct TObject
{
    static_assert(!TIsDerivedFrom<T, BEObject>::Value, "T cannot be a BEObject, please use BEObjectPtr");

    TObject(T* ptr) // assumes ownership
    {
        ReferenceCounter = ptr;
    }

    ~TObject()
    {
        ReferenceCounter.RemoveRef();
    }

    BE_FORCEINLINE TPointer<T> Get()
    {
        return &ReferenceCounter;
    }

    BE_FORCEINLINE TReference<T> GetRef()
    {
        return &ReferenceCounter;
    }
    
private:
    BESharedReference<T> ReferenceCounter = nullptr;
};
*/


///// OLD //////////////////////////////////////////////////////////////////////////

/*
// Can be a Pointer or Reference -> UE FObjectHandle
struct BEObjectHandle
{
    UIntPtr Handle;

    BEObjectHandle(void* ptr)
    {
        Handle = reinterpret_cast<UIntPtr>(ptr);
    }

    BE_FORCEINLINE void* AsPointer()
    {
        return reinterpret_cast<void*>(Handle);
    }

    BE_FORCEINLINE explicit operator bool() const
    {
        return Handle != 0;
    }
};

template<typename T>
struct TObjectRef
{
    static_assert(!TIsDerivedFrom<T, BEObject>::Value, "T cannot be a BEObject, please use BEObjectPtr");

    mutable BEObjectHandle Handle;

    TObjectRef(Null)
        : Handle(nullptr)
    {
    }

    TObjectRef()
        : Handle(nullptr)
    {
    }

    TObjectRef(T& ref)
        : Handle(reinterpret_cast<T&>(ref))
    {
    }

    
};

template<typename T>
struct TObjectPtr
{
    static_assert(!TIsDerivedFrom<T, BEObject>::Value, "T cannot be a BEObject, please use BEObjectPtr");

    mutable BEObjectHandle Handle;
    
    TObjectPtr(Null)
        : Handle(nullptr)
    {
    }

    TObjectPtr()
        : TObjectPtr(nullptr)
    {
    }

    TObjectPtr(T* Ptr)
        : Handle(Ptr)
    {
    }

    BE_FORCEINLINE explicit operator bool() const
    {
        return Handle.Handle;
    }

    BE_FORCEINLINE const T* Get() const
    {
        return static_cast<T*>(Handle.AsPointer());
    }

    BE_FORCEINLINE T* Get()
    {
        return static_cast<T*>(Handle.AsPointer());
    }

    BE_FORCEINLINE T& GetRef()
    {
        // TObjectRef ?
        return *Get();
    }

    BE_FORCEINLINE T* operator->()
    {
        return Get(); 
    }
    
    BE_FORCEINLINE T& operator*()
    {
        return GetRef();
    }
};

template<typename T>
struct TObject
{
    
};
*/
/*
template<typename BEType = BEObject>
struct BEObjectPtr
{
    BEType* Pointer;
};
*/
