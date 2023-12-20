#pragma once

#include "BEArray.h"
#include <type_traits>

enum class BECharacterEncoding : int
{
    UTF8 = 1,
    Unicode = 2
};

#define UNICODE_AS_DEFAULT 0

template<BECharacterEncoding Encoding>
using GetCharEncodingType = std::conditional_t<static_cast<int>(Encoding) == 1, char, wchar_t>;

template<BECharacterEncoding Encoding>
class BERawString : public BEArray<GetCharEncodingType<Encoding>>
{
public:
    using ValueType = GetCharEncodingType<Encoding>;
    using Base = BEArray<ValueType>;
    
    BERawString(size_t Capacity) : Base(Capacity)
    {
    }
    
    static constexpr size_t GetCharEncodingSize() { return sizeof(ValueType); }

private:
    friend struct BETime;
    friend class BETimeContainer;
};

#if UNICODE_AS_DEFAULT
using BEUnicodeString = BERawString<BECharacterEncoding::UTF8>;
using BEString = BERawString<BECharacterEncoding::Unicode>;
#else
using BEString = BERawString<BECharacterEncoding::UTF8>;
using BEUnicodeString = BERawString<BECharacterEncoding::Unicode>;
#endif