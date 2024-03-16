#pragma once
#include "BETypeTraits.h"

template<class T, SizeType Count>
struct TStackElements { T Data[Count]; };

template<SizeType Count>
using TStackBlock = TStackElements<UInt8, Count>;

template<class T>
struct THeapPointer // all heap pointers are 16 bytes
{
    T* Pointer;
    SizeType Length; // how many items are available at 'Pointer'
};


struct BEMemory
{
    static void MemZero(void* Block, SizeType Bytes);
    static void MemCopy(void* Destination, const void* Source, SizeType Bytes);
    static void MemMove(void* Destination, void* Source, SizeType Bytes);
    static void OnOutOfMemory();
    static void CheckMaxAlloc(SizeType Bytes);

    template<typename T BE_REQUIRES(TIsArithmetic<T>::Value || TIsPointer<T>::Value)>
    struct BEByteOperations
    {
        BE_FORCEINLINE static bool CheckBoundary(SizeType Limit, SizeType Index)
        {
            return Index < (Limit / sizeof(T));
        }
        BE_FORCEINLINE static void Set(void* Source, SizeType Limit, SizeType Index, T Value)
        {
            if(CheckBoundary(Limit, Index))
            {
                static_cast<T*>(Source)[Index] = Value;
            }
        }
        BE_FORCEINLINE static T Get(const void* Source, SizeType Limit, SizeType Index)
        {
            if(CheckBoundary(Limit, Index))
            {
                return static_cast<const T*>(Source)[Index];
            }
            return 0;
        }
    };
};
