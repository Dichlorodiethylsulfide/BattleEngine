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

    BEVector(std::initializer_list<ValueType> init_list)
        : Base(init_list)
    {
        m_length = Base::GetCapacity();
    }

    virtual void PushBack(ConstRef Element)
    {
        if(m_length + 1 >= Base::GetCapacity())
        {
            Base::m_elements[m_length++] = Element;
        }
        else
        {
            Base::Resize(Base::GetCapacity() * 2);
            PushBack(Element);
        }
    }

    virtual void PushBack(MovedType Element)
    {
        if(m_length < Base::GetCapacity())
        {
            Base::m_elements[m_length++] = std::move(Element);
        }
        else
        {
            Base::Resize(Base::GetCapacity() * 2);
            PushBack(std::move(Element));
        }
    }

    virtual void PopBack()
    {
        --m_length;
    }

    SizeType GetLength() const override
    {
        return m_length;
    }

private:
    SizeType m_length{};
};
