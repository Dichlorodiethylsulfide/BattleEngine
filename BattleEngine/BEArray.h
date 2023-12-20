#pragma once

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
    using ValueType = TElem;
    using Pointer = ValueType*;
    using ConstPointer = const ValueType*;
    using Ref = ValueType&;
    using ConstRef = const ValueType&;
    
    BEArray(size_t Capacity)
        : m_capacity(!Capacity ? 1 : Capacity)
    {
        m_elements = new ValueType[m_capacity];
    }
    BEArray()
    : BEArray(1)
    {
    }
    ~BEArray()
    {
        if(m_elements)
        {
            delete [] m_elements;
        }
    }
    Ref operator[](size_t Index)
    {
        if(Index < m_capacity)
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
        return m_elements[m_capacity - 1];
    }
    
    ConstRef operator[](size_t Index) const
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
    size_t m_capacity{};
    ValueType* m_elements{};
};
