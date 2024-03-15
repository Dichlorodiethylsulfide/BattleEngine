#pragma once

#include "BEArray.h"

class BEString : public BEArray<CHAR>
{
public:
    BEString();
    BEString(const CHAR* CString);
    BEString(const BEString& String);
    BEString(BEString&& String) noexcept;
    
    BEString& operator=(const CHAR* CString);
    BEString& operator=(const BEString& String);
    BEString& operator=(BEString&& String) noexcept;

    const CHAR* CStr() const;

    static SizeType GetLength(const CHAR* CString);
};
