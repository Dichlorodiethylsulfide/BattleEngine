#pragma once

#include "BEArray.h"

class BEString : public BEArray<Char>
{
public:
    using Base = BEArray<Char>;
    
    BEString();
    BEString(SizeType Length);
    BEString(const Char* CString);
    BEString(const BEString& String);
    BEString(BEString&& String) noexcept;
    
    BEString& operator=(const Char* CString);
    BEString& operator=(const BEString& String);
    BEString& operator=(BEString&& String) noexcept;

    const Char* CStr() const;
    Char* CBuffer();

    static SizeType GetLength(const Char* CString);
};
