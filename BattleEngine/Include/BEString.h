#pragma once

#include "BEVector.h"
#include <ostream>
#include <string>

enum class BECharacterEncoding : int
{
    UTF8 = 1,
    Unicode = 2, // UTF16
    UTF32 = 4
};

#define UNICODE_AS_DEFAULT 0

template<BECharacterEncoding Encoding>
using GetCharEncodingType =
    std::conditional_t<static_cast<int>(Encoding) == 1, char,
    std::conditional_t<static_cast<int>(Encoding) == 2, wchar_t, int>>;

template<BECharacterEncoding Encoding>
static size_t GetStringLength(const GetCharEncodingType<Encoding>* Characters)
{
    if(!Characters)
    {
        return 0;
    }
    size_t length = 0;
    for(; length < static_cast<size_t>(-1); length++)
    {
        if(Characters[length] == '\0')
        {
            return ++length;
        }
    }
    return static_cast<size_t>(-1);
}

template<BECharacterEncoding Encoding>
class BERawString : public BEVector<GetCharEncodingType<Encoding>>
{
public:
    DEFINE_INHERITED_TYPE_TRAITS(BEVector<GetCharEncodingType<Encoding>>)

    static_assert(sizeof(ValueType) < sizeof(SizeType), "You cannot use a char ValueType with size greater or equal to the SizeType size");
    
    BERawString(SizeType Capacity) : Base(Capacity)
    {
    }

    BERawString(ValueType SingleCharacter) : Base(2)
    {
        Base::m_elements[0] = SingleCharacter;
        Base::m_elements[1] = '\0';
    }

    BERawString(ConstPointer String) : Base(String, GetStringLength<Encoding>(String))
    {
    }

    BERawString(Pointer String) : Base(String, GetStringLength<Encoding>(String))
    {
    }

    friend std::ostream&
    operator<<(std::ostream& os, const BERawString& string)
    {
        return os << string.m_elements;
    }

    SizeType GetLength() const override
    {
        return GetStringLength<Encoding>(Base::m_elements);
    }

    void PushBack(ConstRef Element) override
    {
        PushBack(std::move(std::remove_reference_t<ValueType>(Element)));
    }
    
    void PushBack(MovedType Element) override
    {
        SizeType length = GetLength() - 1;
        if(length + 2 < Base::GetCapacity())
        {
            Base::m_elements[length++] = std::move(Element);
            Base::m_elements[length] = '\0';
        }
        else
        {
            Base::Resize(Base::GetCapacity() * 2);
            PushBack(std::move(Element));
        }
    }

    void PopBack() override
    {
        SizeType length = GetLength() - 2;
        Base::m_elements[length] = '\0';
    }

    BERawString& Strip()
    {
        for(SizeType i = 0; i < GetLength(); ++i)
        {
            if(Base::m_elements[i] == '\n')
            {
                Base::RemoveAt(i);
            }
        }
        return *this;
    }

    operator ConstPointer()
    {
        return Base::GetBuffer();
    }

    template<typename TNonElemType>
    static BERawString ToString(TNonElemType&& Value)
    {
        static_assert(!std::is_integral_v<TNonElemType>, "Must be an integral type");
        return std::to_string(Value).c_str();
    }
    
    static constexpr SizeType GetCharEncodingSize() { return sizeof(ValueType); }

private:
    friend struct BETime;
    friend class BEConsoleIO;
    friend class BETimeContainer;
};

#if UNICODE_AS_DEFAULT
using BEUnicodeString = BERawString<BECharacterEncoding::UTF8>;
using BEString = BERawString<BECharacterEncoding::Unicode>;
#else
using BEString = BERawString<BECharacterEncoding::UTF8>;
using BEUnicodeString = BERawString<BECharacterEncoding::Unicode>;
#endif
