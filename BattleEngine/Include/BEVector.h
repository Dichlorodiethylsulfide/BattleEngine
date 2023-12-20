#pragma once
#include "BEArray.h"

template<typename TElem>
class BEVector : public BEArray<TElem>
{
public:
    using ValueType = TElem;
    using Base = BEArray<ValueType>;
    using SizeType = typename Base::SizeType;
    using Pointer = typename Base::Pointer;
    using ConstPointer = typename Base::ConstPointer;

    BEVector(SizeType Size) : Base(Size)
    {
    }
    
    BEVector(Pointer Elements, SizeType Size) : Base(Elements, Size)
    {
    }
    
    BEVector(ConstPointer Elements, SizeType Size) : Base(Elements, Size)
    {
    }

    SizeType GetLength() const override
    {
        return m_length;
    }

private:
    SizeType m_length{};
};
