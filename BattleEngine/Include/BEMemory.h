#pragma once
#include "BETypeTraits.h"

struct BEMemory
{
    static void MemZero(void* Block, SizeType Bytes);
    static void MemCopy(void* Destination, const void* Source, SizeType Bytes);
    static void MemMove(void* Destination, void* Source, SizeType Bytes);
    static void OnOutOfMemory();
    static void CheckMaxAlloc(SizeType Bytes);


    struct BEByteOperations
    {
        BE_FORCEINLINE static bool CheckByteBoundary(SizeType Limit, SizeType Index)
        {
            return Index < Limit;
        }
        BE_FORCEINLINE static void SetByte(void* Source, SizeType Limit, SizeType Index, int8 Value)
        {
            if(CheckByteBoundary(Limit, Index))
            {
                static_cast<int8*>(Source)[Index] = Value;
            }
        }
        BE_FORCEINLINE static int8 GetByte(const void* Source, SizeType Limit, SizeType Index)
        {
            if(CheckByteBoundary(Limit, Index))
            {
                return static_cast<const int8*>(Source)[Index];
            }
            return 0;
        }
    };
    
};
