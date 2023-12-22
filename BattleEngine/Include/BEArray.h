#pragma once

#include <ranges> // remove: for std::move
#include <initializer_list>

#include "BEUtilities.h"

template<typename TType>
struct Bad
{
    static TType* GetBadPointer()
    {
        static TType* BadPointer = nullptr;
        return BadPointer;
    }
};

template<typename TElem>
class ConstIter
{
public:
    DEFINE_BASIC_TYPE_TRAITS(TElem)
    
    ConstIter() {}
    ConstIter(Pointer Element) { current = Element; }
    virtual ~ConstIter() { current = {}; }

    static SizeType GetIteratorLength(ConstIter Begin, ConstIter End)
    {
        SizeType length = 0;
        while(Begin++ != End)
        {
            length++;
        }
        return length;
    }

    // A const iterator can be safely iterated through in a 'const ConstIterator' context
    
    const ConstIter& operator++() const
    {
        ++const_cast<ConstIter*>(this)->current;
        return *this;
    }
    ConstIter operator++(int) const
    {
        auto old = *this;
        operator++();
        return old;
    }
    const ConstIter& operator--() const
    {
        --current;
        return *this;
    }
    ConstIter operator--(int) const
    {
        auto old = *this;
        operator--();
        return old;
    }
    bool operator==(const ConstIter& Other) const
    {
        return current == Other.current;
    }
    bool operator!=(const ConstIter& Other) const
    {
        return current != Other.current;
    }
    ConstRef operator*() const
    {
        return *current;
    }
private:
    Pointer current{};
};

template<typename TElem>
class ConstReverseIter
{
public:
    DEFINE_BASIC_TYPE_TRAITS(TElem)

    ConstReverseIter() {}
    ConstReverseIter(Pointer Element) { current = Element; }
    virtual ~ConstReverseIter() { current = {}; }

    static SizeType GetIteratorLength(ConstReverseIter Begin, ConstReverseIter End)
    {
        SizeType length = 0;
        while(Begin++ != End)
        {
            length++;
        }
        return length;
    }

    const ConstReverseIter& operator++() const
    {
        --current;
        return *this;
    }
    ConstReverseIter operator++(int) const
    {
        auto old = *this;
        operator--();
        return old;
    }
    const ConstReverseIter& operator--() const
    {
        ++current;
        return *this;
    }
    ConstReverseIter operator--(int) const
    {
        auto old = *this;
        operator++();
        return old;
    }
    bool operator==(const ConstReverseIter& Other) const
    {
        return current == Other.current;
    }
    bool operator!=(const ConstReverseIter& Other) const
    {
        return current != Other.current;
    }
    ConstRef operator*() const
    {
        return *current;
    }
private:
    Pointer current{};
};

template<typename TElem>
class Iter : public ConstIter<TElem>
{
public:
    DEFINE_INHERITED_TYPE_TRAITS(ConstIter<TElem>)

    Iter() {}
    Iter(Pointer Element) : Base(Element) {}

    Iter& operator++()
    {
        dynamic_cast<Base*>(this)->operator++();
        return *this;
    }
    Iter operator++(int)
    {
        auto old = *this;
        operator++();
        return old;
    }
    Iter& operator--()
    {
        --Base::current;
        return *this;
    }
    Iter operator--(int)
    {
        auto old = *this;
        operator--();
        return old;
    }
};

template<typename TElem>
class ReverseIter : public ConstReverseIter<TElem>
{
public:
    DEFINE_INHERITED_TYPE_TRAITS(ConstReverseIter<TElem>)

    ReverseIter() {}
    ReverseIter(Pointer Element) : Base(Element) {}
    
    ReverseIter& operator++()
    {
        --Base::current;
        return *this;
    }
    ReverseIter operator++(int)
    {
        auto old = *this;
        operator--();
        return old;
    }
    ReverseIter& operator--()
    {
        ++Base::current;
        return *this;
    }
    ReverseIter operator--(int)
    {
        auto old = *this;
        operator++();
        return old;
    }
};

#define FOREACH_ITER iter
#define FOREACH(x) for(auto FOREACH_ITER = (x).Begin(); FOREACH_ITER != (x).End(); ++FOREACH_ITER)
#define FOREACH_VALUE *FOREACH_ITER

template<typename TElem>
class BEAllocator
{
public:
    DEFINE_BASIC_TYPE_TRAITS(TElem)
    
    static_assert(!std::is_const_v<TElem>, "Cannot allocate constant types");

    Pointer Allocate(SizeType Count)
    {
        return static_cast<Pointer>(::operator new(Count * sizeof(ValueType)));
    }

    void Deallocate(Pointer& Ptr, SizeType& Size)
    {
        if(Ptr)
        {
            ::operator delete(Ptr);
            Ptr = nullptr;
        }
        Size = 0;
    }

    void Reallocate(Pointer& Elements, SizeType& OldSize, SizeType NewSize)
    {
        if(NewSize <= OldSize)
        {
            return;
        }
        auto* NewElements = Allocate(NewSize);
        Move(NewElements, Elements, OldSize);
        Deallocate(Elements, OldSize);
        OldSize = NewSize;
        Elements = NewElements;
    }

    static void Copy(Pointer Destination, ConstPointer Source, SizeType Length)
    {
        for(SizeType i = 0; i < Length; i++)
        {
            Destination[i] = Source[i];
        }
    }

    static void Move(Pointer Destination, Pointer Source, SizeType Length)
    {
        for(SizeType i = 0; i < Length; i++)
        {
            Destination[i] = std::move(Source[i]);
        }
    }
};

template<typename TElem>
class BEArray
{
public:
    DEFINE_BASIC_TYPE_TRAITS(TElem)
    DEFINE_ITERATOR_TYPE_TRAITS(TElem)
    using Allocator = BEAllocator<ValueType>;

    static inline bool IsObjectEqual(ConstPointer Elements, ConstPointer OtherElements)
    {
        return (Elements != nullptr && OtherElements != nullptr) && Elements == OtherElements;
    }

    static inline bool AreElementsArrayEqual(ConstPointer Elements, ConstPointer OtherElements, SizeType Size)
    {
        for(SizeType i = 0; i < Size; i++)
        {
            if(Elements[i] != OtherElements[i])
            {
                return false;
            }
        }
        return true;
    }
    
    BEArray(SizeType Capacity)
        : m_capacity(Capacity)
    {
        // Always allocate at least some data, as long as we track it properly, we can deallocate it and move on once we have more elements
        m_elements = m_allocator.Allocate(!Capacity ? 1 : Capacity);
    }
    BEArray()
    : BEArray(1)
    {
    }
    BEArray(ConstPointer Elements, SizeType Length)
        : BEArray(Length)
    {
        Allocator::Copy(m_elements, Elements, Length);
    }
    BEArray(const BEArray& Other)
    {
        *this = Other;
    }
    BEArray(BEArray&& Other) noexcept
    {
        *this = std::move(Other);
    }
    BEArray(std::initializer_list<ValueType> init_list)
        : BEArray(init_list.size())
    {
        SizeType i = 0;
        for(auto& elem : init_list)
        {
            m_elements[i++] = std::move(const_cast<Ref>(elem));
        }
    }
    virtual ~BEArray()
    {
        m_allocator.Deallocate(m_elements, m_capacity);
    }
    BEArray& operator=(const BEArray& Other)
    {
        if(!IsObjectEqual(m_elements, Other.m_elements))
        {
            m_allocator.Deallocate(m_elements, m_capacity);
            m_capacity = Other.m_capacity;
            m_elements = m_allocator.Allocate(m_capacity);
            Allocator::Copy(m_elements, Other.m_elements, m_capacity);
        }
        return *this;
    }
    BEArray& operator=(BEArray&& Other)
    {
        if(!IsObjectEqual(m_elements, Other.m_elements))
        {
            m_capacity = Other.m_capacity;
            Other.m_capacity = 0;
            m_elements = Other.m_elements;
            Other.m_elements = nullptr;
        }
        return *this;
    }
    bool operator==(const BEArray& Other) const
    {
        return AreElementsArrayEqual(m_elements, Other.m_elements, GetLength());
    }
    bool operator!=(const BEArray& Other) const
    {
        return !(*this == Other);
    }
    Ref operator[](SizeType Index)
    {
        if(Index < GetLength())
        {
            return m_elements[Index];
        }
        return *Bad<ValueType>::GetBadPointer();
    }
    void Resize(SizeType NewSize)
    {
        if(!m_capacity && !NewSize)
        {
            NewSize = 1;
        }
        m_allocator.Reallocate(m_elements, m_capacity, NewSize);
    }
    Ref Front()
    {
        return m_elements[0];
    }
    Ref Back()
    {
        return m_elements[GetLength() - 1];
    }
    void RemoveAt(SizeType Index)
    {
        if(Index < GetLength())
        {
            Allocator::Move(&m_elements[Index], &m_elements[Index+1], GetLength() - Index - 1);
        }
    }
    Iterator Begin()
    {
        return m_elements;
    }
    ReverseIterator RBegin()
    {
        return m_elements;
    }
    ConstReverseIterator RBegin() const
    {
        return m_elements;
    }
    ConstIterator Begin() const
    {
        return m_elements;
    }
    Iterator End()
    {
        return &m_elements[GetLength()];
    }
    ReverseIterator REnd()
    {
        return &m_elements[GetLength()]; 
    }
    ConstReverseIterator REnd() const
    {
        return &m_elements[GetLength()];
    }
    ConstIterator End() const
    {
        return &m_elements[GetLength()];
    }
    //
    ConstRef operator[](SizeType Index) const
    {
        return AsNonConst()[Index];
    }
    ConstRef Front() const
    {
        return AsNonConst().Front();
    }
    ConstRef Back() const
    {
        return AsNonConst().Back();
    }
    virtual SizeType GetLength() const
    {
        return m_capacity;
    }
    SizeType GetCapacity() const
    {
        return m_capacity;
    }
protected:
    constexpr BEArray& AsNonConst() const
    {
        return const_cast<BEArray&>(*this);
    }
    Pointer GetBuffer()
    {
        return m_elements;
    }
    ConstPointer GetBuffer() const
    {
        return AsNonConst().GetBuffer();
    }
    SizeType m_capacity{};
    ValueType* m_elements{};
private:
    Allocator m_allocator{};
};
