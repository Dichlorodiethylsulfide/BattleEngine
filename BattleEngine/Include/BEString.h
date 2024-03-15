#pragma once

#include "BEAllocatorPolicy.h"
#include "BEMemory.h"

class BEString
{
public:
    BEString();
    BEString(const CHAR* CString);
    BEString(const BEString& String);
    BEString(BEString&& String) noexcept;
    ~BEString();
    
    BEString& operator=(const CHAR* CString);
    BEString& operator=(const BEString& String);
    BEString& operator=(BEString&& String) noexcept;

    void Clear();
    bool IsEmpty() const;

    uint64 GetLength() const;
    const CHAR* CStr() const;
    bool GetIsSSO() const;

    static SizeType GetLength(const CHAR* CString);
    
private:
    BESmallObjectOptimizedStack<CHAR> InternalStringStack;
    static void BuildBEString(const CHAR* CString, BEString& ThisString);
};
