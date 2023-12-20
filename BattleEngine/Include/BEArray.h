#pragma once

#include<ranges> // remove: for std::move

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
class BEArray
{
public:
    using Base = BEArray;
    using SizeType = size_t;
    using ValueType = TElem;
    using Pointer = ValueType*;
    using ConstPointer = const ValueType*;
    using Ref = ValueType&;
    using ConstRef = const ValueType&;

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

    static void Free(Pointer& Source, SizeType& Size)
    {
        if(Source)
        {
            delete [] Source;
            Source = nullptr;
        }
        Size = 0;
    }

    static Pointer Allocate(SizeType Size)
    {
        return new ValueType[Size];
    }

    static inline bool IsObjectEqual(ConstPointer Elements, ConstPointer OtherElements)
    {
        return Elements == OtherElements;
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
        : m_capacity(!Capacity ? 1 : Capacity)
    {
        m_elements = Allocate(Capacity);
    }
    BEArray()
    : BEArray(1)
    {
    }
    BEArray(ConstPointer Elements, SizeType Length)
        : BEArray(Length)
    {
        Copy(m_elements, Elements, Length);
    }
    BEArray(const BEArray& Other)
    {
        *this = Other;
    }
    BEArray(BEArray&& Other) noexcept
    {
        *this = std::move(Other);
    }
    virtual ~BEArray()
    {
        Free(m_elements, m_capacity);
    }
    BEArray& operator=(const BEArray& Other)
    {
        if(!IsObjectEqual(m_elements, Other.m_elements))
        {
            Free(m_elements, m_capacity);
            m_capacity = Other.m_capacity;
            m_elements = Allocate(m_capacity);
            Copy(m_elements, Other.m_elements, m_capacity);
        }
        return *this;
    }
    BEArray& operator=(BEArray&& Other)
    {
        if(!IsObjectEqual(m_elements, Other.m_elements))
        {
            Free(m_elements, m_capacity);
            m_capacity = Other.m_capacity;
            m_elements = Allocate(m_capacity);
            Move(m_elements, Other.m_elements, m_capacity);
            Free(Other.m_elements, Other.m_capacity);
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
            Base::Move(&m_elements[Index], &m_elements[Index+1], GetLength() - Index - 1);
        }
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
};
