#pragma once

#include "BETypeTraits.h"

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
    
    const CHAR* CStr() const;

    static SizeType GetLength(const CHAR* CString);
    
private:
    struct SSO
    {
        uint128 High;
        uint128 Low;

        BE_FORCEINLINE const CHAR* AsCString() const
        {
            return reinterpret_cast<const CHAR*>(&High);
        }
    };
    static_assert(sizeof(SSO) == 32, "Do not modify the size of the SmallStringOptimisation type");
    union
    {
        SSO SmallString;
        CHAR* Data;
    };
    
    static void BuildBEString(const CHAR* CString, BEString& ThisString);
    
    static void SetSSO(SSO& SmallString, bool bIsSSO);
    static bool IsSSO(const SSO& SmallString);
};
