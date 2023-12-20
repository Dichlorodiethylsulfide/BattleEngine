#pragma once

#include "BEVector.h"
#include <ostream>

enum class BECharacterEncoding : int
{
    UTF8 = 1,
    Unicode = 2
};

#define UNICODE_AS_DEFAULT 0

template<BECharacterEncoding Encoding>
using GetCharEncodingType = std::conditional_t<static_cast<int>(Encoding) == 1, char, wchar_t>;

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
    using ValueType = GetCharEncodingType<Encoding>;
    using Base = BEVector<ValueType>;
    using SizeType = typename Base::SizeType;
    using Pointer = typename Base::Pointer;
    using ConstPointer = typename Base::ConstPointer;
    
    BERawString(SizeType Capacity) : Base(Capacity)
    {
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
    
    static constexpr SizeType GetCharEncodingSize() { return sizeof(ValueType); }

private:
    friend struct BETime;
    friend class BEIO;
    friend class BETimeContainer;
};

#if UNICODE_AS_DEFAULT
using BEUnicodeString = BERawString<BECharacterEncoding::UTF8>;
using BEString = BERawString<BECharacterEncoding::Unicode>;
#else
using BEString = BERawString<BECharacterEncoding::UTF8>;
using BEUnicodeString = BERawString<BECharacterEncoding::Unicode>;
#endif
