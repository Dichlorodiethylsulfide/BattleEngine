#pragma once

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
    struct SmallStringStack
    {
        enum // End Byte Position - EnumValue
        {
            SS0 = 1,
            Length = 2,
            EndBytePositionMax
        };
        uint128 High;
        uint128 Low;

        SmallStringStack(bool bIsSSO, uint8 Length)
            : High(), Low()
        {
            if(bIsSSO)
            {
                SetSSO(true);
                SetLength(Length);
            }
        }

        BE_FORCEINLINE const CHAR* AsCString() const
        {
            return reinterpret_cast<const CHAR*>(this);
        }

        BE_FORCEINLINE void SetSSO(bool bIsSSO)
        {
            BEMemory::BEByteOperations<uint8>::Set(this, sizeof(SmallStringStack),  sizeof(SmallStringStack) - SS0, bIsSSO ? 1 : 0);
        }

        BE_FORCEINLINE bool IsSSO() const
        {
            return BEMemory::BEByteOperations<uint8>::Get(this, sizeof(SmallStringStack), sizeof(SmallStringStack) - SS0);
        }

        BE_FORCEINLINE void SetLength(uint8 Length)
        {
            BEMemory::BEByteOperations<uint8>::Set(this, sizeof(SmallStringStack),  sizeof(SmallStringStack) - SmallStringStack::Length, Length);
        }

        BE_FORCEINLINE uint8 GetLength() const
        {
            return BEMemory::BEByteOperations<uint8>::Get(this, sizeof(SmallStringStack), sizeof(SmallStringStack) - Length);
        }
    };
    struct StringHeap
    {
        CHAR* Pointer;
        SizeType Length;
    };
    union
    {
        SmallStringStack SmallString;
        StringHeap Data;
    };
    static_assert(sizeof(SmallStringStack) == 32, "Do not modify the size of the SmallStringOptimisation type");
    static_assert(sizeof(StringHeap) <= sizeof(SmallStringStack), "StringHeap must be smaller than or equal to the size of SmallStringOptimisation");

    BE_FORCEINLINE void SetLength(SizeType Length)
    {
        if(SmallString.IsSSO())
        {
            SmallString.SetLength(static_cast<uint8>(Length));
        }
        else
        {
            Data.Length = Length;
        }
    }
    
    static void BuildBEString(const CHAR* CString, BEString& ThisString);
};
