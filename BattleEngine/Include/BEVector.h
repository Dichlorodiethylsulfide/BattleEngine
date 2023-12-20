#pragma once
#include "BEArray.h"

template<typename TElem>
class BEVector : public BEArray<TElem>
{
public:
    DEFINE_INHERITED_TYPE_TRAITS(BEArray<TElem>)

    BEVector(SizeType Size) : Base(Size)
    {
    }
    
    BEVector(Pointer Elements, SizeType Size) : Base(Elements, Size)
    {
        m_length = Size;
    }
    
    BEVector(ConstPointer Elements, SizeType Size) : Base(Elements, Size)
    {
        m_length = Size;
    }

    SizeType GetLength() const override
    {
        return m_length;
    }

private:
    SizeType m_length{};
};
