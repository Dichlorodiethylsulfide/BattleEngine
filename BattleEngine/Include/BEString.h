#pragma once

#include "BETypeTraits.h"

class BEString
{
public:
    BEString();
    BEString(const CHAR* CString);
    ~BEString();

    void Clear();
    
    const CHAR* CStr() const;

    static SizeType GetLength(const CHAR* CString);
    
private:
    struct SSO
    {
        uint128 High;
        uint128 Low; 
    };
    union
    {
        SSO SmallString;
        CHAR* Data;
    };
    
    static void SetSSO(SSO& SmallString, bool bIsSSO);
    static bool IsSSO(const SSO& SmallString);
};
