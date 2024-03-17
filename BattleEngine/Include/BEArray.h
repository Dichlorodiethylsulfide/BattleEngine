#pragma once
#include "BEAllocatorPolicy.h"

template<typename T>
class BEArray : public ICollection
{
public:
    BEArray() = default;
    BEArray(SizeType Length);
    BEArray(const T* Elements, SizeType Length);
    BEArray(const BEArray& Array) { *this = Array; }
    BEArray(BEArray&& Array) noexcept { *this = BEMove<BEArray>(Array); }
    
    BEArray& operator=(const BEArray& Array);
    BEArray& operator=(BEArray&& Array) noexcept;

    void Clear() override;
    bool IsSSO() const;
    bool IsEmpty() const override;

    SizeType Length() const override;
    
protected:
    BESmallObjectOptimizedStack<T> InternalStack;
};

template <typename T>
BEArray<T>::BEArray(SizeType Length)
{
    InternalStack = BESmallObjectOptimizedStack<T>(Length);
}

template <typename T>
BEArray<T>::BEArray(const T* Elements, SizeType Length)
{
    InternalStack = BESmallObjectOptimizedStack<T>(Elements, Length);
}

template <typename T>
BEArray<T>& BEArray<T>::operator=(const BEArray& Array)
{
    InternalStack = Array.InternalStack;
    return *this;
}

template <typename T>
BEArray<T>& BEArray<T>::operator=(BEArray&& Array) noexcept
{
    InternalStack = BEMove<BESmallObjectOptimizedStack<T>>(Array.InternalStack);
    return *this;
}

template <typename T>
void BEArray<T>::Clear()
{
    InternalStack.Clear();
}

template <typename T>
bool BEArray<T>::IsSSO() const
{
    return InternalStack.GetIsStack();
}

template <typename T>
bool BEArray<T>::IsEmpty() const
{
    return InternalStack.IsEmpty();
}

template <typename T>
UInt64 BEArray<T>::Length() const
{
    return InternalStack.GetLength();
}
