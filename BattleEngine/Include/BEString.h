#pragma once

#include "BETypeTraits.h"

class BEString
{
public:
    BEString();
    BEString(const CHAR* CString);
    ~BEString();

    // BEString& operator=(const CHAR* CString);

    void Clear();
    bool IsEmpty() const;
    
    const CHAR* CStr() const;

    static SizeType GetLength(const CHAR* CString);
    
private:
    struct SSO
    {
        uint128 High;
        uint128 Low; 
    };
    static_assert(sizeof(SSO) == 32, "Do not modify the size of the SmallStringOptimisation type");
    union
    {
        SSO SmallString;
        CHAR* Data;
    };
    
    static void SetSSO(SSO& SmallString, bool bIsSSO);
    static bool IsSSO(const SSO& SmallString);
};
