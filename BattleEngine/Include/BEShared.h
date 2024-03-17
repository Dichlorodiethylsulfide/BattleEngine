#pragma once

#include "BEAtomic.h"
#include "BEObject.h"

enum class EThreadMode
{
    ThreadSafe,
    ThreadUnsafe
};

template<EThreadMode Mode = EThreadMode::ThreadSafe>
class TSharedReferenceCounter final
{
    mutable typename TChooseType<Mode == EThreadMode::ThreadSafe, TAtomic<SizeType>, SizeType>::Type StrongRefCount;
    // TODO: Implement weak refs

public:
    TSharedReferenceCounter() = delete;
    TSharedReferenceCounter(const TSharedReferenceCounter&) = delete;
    TSharedReferenceCounter(TSharedReferenceCounter&&) = delete;
    TSharedReferenceCounter(SizeType Refs)
    {
        StrongRefCount = Refs;
    }
    ~TSharedReferenceCounter() = default;
    TSharedReferenceCounter& operator=(const TSharedReferenceCounter&) = delete;
    TSharedReferenceCounter& operator=(TSharedReferenceCounter&&) = delete;
    void AddRef() const
    {
        ++StrongRefCount;
    }
    bool RemoveRef() const
    {
        return --StrongRefCount == 0;
    }
    bool IsUnique() const
    {
        return StrongRefCount == 1;
    }
};

template<typename T, EThreadMode Mode = EThreadMode::ThreadSafe>
class TSharedPtr // const TSharedPtr<T> does not mean T is const, use TSharedPtr<const T>
{
public:
    using RefCounter = TSharedReferenceCounter<Mode>;
    TSharedPtr()
        : Object(nullptr)
        , Counter(nullptr)
    {
    }
    TSharedPtr(Null)
        : Object(nullptr)
        , Counter(nullptr)
    {
    }
    TSharedPtr(const TSharedPtr& Other)
    {
        *this = Other;
    }
    TSharedPtr(TSharedPtr&& Other) noexcept
    {
        *this = BEMove(Other);
    }
    ~TSharedPtr()
    {
        if(Counter && Counter->RemoveRef())
        {
            Delete(Object);
            delete Counter;
            Counter = nullptr;
        }
    }
    T* operator->() const
    {
        BE_CHECK(!IsValid())
        return Object;
    }
    T& operator*() const
    {
        BE_CHECK(!IsValid())
        return Object;
    }
    bool IsValid() const
    {
        return Object != nullptr && Counter != nullptr;
    }
    TSharedPtr& operator=(const TSharedPtr& Other)
    {
        if(*this != Other)
        {
            Object = Other.Object;
            Counter = Other.Counter;
            Counter->AddRef();
        }
        return *this;
    }
    TSharedPtr& operator=(TSharedPtr&& Other) noexcept
    {
        if(*this != Other)
        {
            Object = Other.Object;
            Counter = Other.Counter;
            Other.Object = nullptr;
            Other.Counter = nullptr;
        }
        return *this;
    }
    bool operator==(const TSharedPtr& Other) const
    {
        return IsValid() && Other.IsValid() && Object == Other.Object && Counter == Other.Counter;
    }
    bool operator!=(const TSharedPtr& Other) const
    {
        return !(*this == Other);
    }

    template<typename ... Args>
    static TSharedPtr MakeSharedPtr(Args&&... args)
    {
        return TSharedPtr(New<T>(BEForward<Args>(args)...));
    }
    
private:
    TSharedPtr(T* Ptr)
    {
        Object = Ptr;
        Counter = new RefCounter(1);
    }
    mutable T* Object = nullptr;
    RefCounter* Counter = nullptr;
};

template<typename T, EThreadMode Mode = EThreadMode::ThreadSafe>
class TSharedRef
{
public:
    using RefCounter = TSharedReferenceCounter<Mode>;
    TSharedRef() = delete; // not allowed
    TSharedRef(Null) = delete; // not allowed
    TSharedRef(const TSharedRef& Other)
    {
        *this = Other;
    }
    TSharedRef(TSharedRef&& Other) noexcept
    {
        *this = BEMove(Other);
    }
    ~TSharedRef()
    {
        if(Counter && Counter->RemoveRef())
        {
            Delete(Object);
            delete Counter;
            Counter = nullptr;
        }
    }
    T* operator->() const
    {
        BE_CHECK(!Object)
        return Object;
    }
    T& operator*() const
    {
        BE_CHECK(!Object)
        return Object;
    }
    TSharedRef& operator=(const TSharedRef& Other)
    {
        if(*this != Other)
        {
            Object = Other.Object;
            Counter = Other.Counter;
            Counter->AddRef();
        }
        return *this;
    }
    TSharedRef& operator=(TSharedRef&& Other) noexcept
    {
        if(*this != Other)
        {
            Object = Other.Object;
            Counter = Other.Counter;
            Other.Object = nullptr;
            Other.Counter = nullptr;
        }
        return *this;
    }
    bool operator==(const TSharedRef& Other) const
    {
        return Object == Other.Object && Counter == Other.Counter;
    }
    bool operator!=(const TSharedRef& Other) const
    {
        return !(*this == Other);
    }

    template<typename ... Args>
    static TSharedRef MakeSharedRef(Args&&... args)
    {
        return TSharedRef(New<T>(BEForward<Args>(args)...));
    }
    
private:
    TSharedRef(T* Ptr)
    {
        Object = Ptr;
        Counter = new RefCounter(1);
    }
    mutable T* Object = nullptr;
    RefCounter* Counter = nullptr;
};

template<typename T, EThreadMode Mode = EThreadMode::ThreadSafe, typename ... Args>
auto MakeSharedPtr(Args&&... args)
{
    return TSharedPtr<T, Mode>::MakeSharedPtr(BEForward<Args>(args)...);
}

template<typename T, EThreadMode Mode = EThreadMode::ThreadSafe, typename ... Args>
auto MakeSharedRef(Args&&... args)
{
    return TSharedRef<T, Mode>::MakeSharedRef(BEForward<Args>(args)...);
}