#pragma once

// All definitions in BETypeTraits.h will be moved into appropriate files / folders when ready

/* Naming Conventions
 * - BE for Objects
 * - T for templates
 * - E for enums
 * - S for structs
 */

// Type trait definitions
#define BE_STRINGIFY(x) #x
#define BE_CONCAT_IMPL(x, y) x##y
#define BE_CONCAT(x, y) BE_CONCAT_IMPL(x, y)
#define BE_FORCEINLINE __forceinline

using int8 = signed char;
using int16 = signed short;
using int32 = signed int;
using int64 = signed long long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;
using ansi = char;
using unicode = wchar_t;
using SizeType = uint64;

struct uint128
{
    uint64 High;
    uint64 Low;
};

#if 1
#define TEXT(x) x
using CHAR = ansi;
#else // Unicode
#define TEXT(x) L ## x
using CHAR = unicode;
#endif

template<class>
struct TGetIntLimit
{
    BE_FORCEINLINE static SizeType GetMin() { return 0; }
    BE_FORCEINLINE static SizeType GetMax() { return 0; }
};

template<class, class>
struct TIsSame { enum { Value = false }; };

template<class>
struct TIsArithmetic { enum { Value = false };};

template<class>
struct TIsFloatingPoint { enum { Value = false }; };

template<class>
struct TIsVoid { enum { Value = false }; };

template<typename T>
struct TIsFundamental { enum { Value = TIsArithmetic<T>::Value || TIsVoid<T>::Value }; };

template<class>
struct TIsFunction { enum { Value = false }; };

template<class>
struct TIsPointer { enum { Value = false }; };

template<class>
struct TNameOf { BE_FORCEINLINE static CHAR const* GetName() { return TEXT("Unknown"); } };

template<class TDerived, class TBase>
struct TIsDerivedFrom
{
    using No = int8;
    using Yes = int16;

    static Yes& Test(TBase*);
    static Yes& Test(const TBase*);
    static No& Test(...);

    const TDerived* const DerivedPtr = nullptr;
    enum { Value = sizeof(Test(DerivedPtr)) == sizeof(Yes) };
};

template<class>
struct TIsConst { enum { Value = false }; };

template<class>
struct TIsVolatile { enum { Value = false }; };

template<typename T>
struct TRemoveConst
{
    using Type = T;
};

template<typename T>
struct TRemoveVolatile
{
    using Type = T;
};

template<typename T>
struct TRemovePointer
{
    using Type = T;
};

template<typename T>
struct TRemoveLReference
{
    using Type = T;
};

template<typename T>
struct TRemoveRReference
{
    using Type = T;
};

template<typename T>
struct TAddConst
{
    using Type = const T;
};

template<typename T>
struct TAddVolatile
{
    using Type = volatile T;
};

template<typename T>
struct TAddPointer
{
    using Type = T*;
};

template<typename T>
struct TAddLReference
{
    using Type = T&;
};

template<typename T>
struct TAddRReference
{
    using Type = T&&;
};

template<bool, class, class>
struct TChooseType
{
    using Type = void;
};

template<bool, typename T, T, T>
struct TChooseConstant
{
    using Type = void;
};

template<class TA, class TB>
struct TChooseType<true, TA, TB> { using Type = TA; };

template<class TA, class TB>
struct TChooseType<false, TA, TB> { using Type = TB; };

template<class TType, TType TA, TType TB>
struct TChooseConstant<true, TType, TA, TB> { enum { Value = TA }; };

template<class TType, TType TA, TType TB>
struct TChooseConstant<false, TType, TA, TB> { enum { Value = TB }; };

using Null = decltype(nullptr);

class BEObject;

template<typename T>
struct TObjectPtr
{
    static_assert(!TIsDerivedFrom<T, BEObject>::Value, "T cannot be a BEObject, please use BEObjectPtr");
    
    TObjectPtr(Null)
        : Pointer(nullptr)
    {
    }

    TObjectPtr()
        : TObjectPtr(nullptr)
    {
    }

    TObjectPtr(T* Ptr)
        : Pointer(Ptr)
    {
    }

    mutable T* Pointer;

    BE_FORCEINLINE bool IsValid() const
    {
        return Pointer != nullptr;
    }

    BE_FORCEINLINE explicit operator bool() const
    {
        return IsValid();
    }

    BE_FORCEINLINE const T* Get() const
    {
        return Pointer;
    }

    BE_FORCEINLINE const T& GetRef() const
    {
        // TObjectRef ?
        return *Pointer;
    }
};

struct BEObjectPtr
{
    BEObject* Pointer;
};

template<class T, SizeType Count>
struct TStackElements { T Data[Count]; };

template<SizeType Count>
using TStackBlock = TStackElements<uint8, Count>;

template<class T>
struct THeapPointer // all heap pointers are 16 bytes
{
    TObjectPtr<T> Pointer;
    SizeType Length; // how many items are available at 'Pointer'
};

template<typename...> struct TUnion;

template<class T, class TT>
struct TUnion<T, TT>
{
    // ReSharper disable once CppPossiblyUninitializedMember -> Fails build if initialized
    TUnion() {}
    union
    {
        T Data;
        TT Other;
    };

    using DataType = T;
    using OtherType = TT;
};

template<class T, class ... TTypes>
struct TUnion<T, TTypes...>
{
    // ReSharper disable once CppPossiblyUninitializedMember -> Fails build if initialized
    TUnion() {}
    union
    {
        T Data;
        TUnion<TTypes...> Other;
    };

    using DataType = T;
    using OtherType = decltype(Other);
};

#if __cplusplus < 20200
#define BE_REQUIRES
#error Code was not compiled prior to C++ 20
#else
/*
 * Follows the same style as UE_REQUIRES
 * template<typename T BE_REQUIRES(sizeof(T) >= 4)> void Function()
 */
template<bool Value>
concept bRequiresTrue = Value;
#define BE_REQUIRES(...) > requires !!((__VA_ARGS__)) && bRequiresTrue<true
#endif

#define BE_T_EXPOSE_NAME(type) \
    template<> \
    struct TNameOf<type> { BE_FORCEINLINE static CHAR const* GetName() { return TEXT(BE_STRINGIFY(type)); } };

#define BE_T_VALID_TRAIT(trait, type) template <> struct trait<type> { enum { Value = true }; };
#define BE_T_VALID_CV_TRAIT(trait, cv) template <typename T> struct trait<cv T> { enum { Value = trait<T>::Value }; };
#define BE_T_VALID_ALL_CV_TRAIT(trait) \
      BE_T_VALID_CV_TRAIT(trait, const) \
      BE_T_VALID_CV_TRAIT(trait, volatile) \
      BE_T_VALID_CV_TRAIT(trait, const volatile)


#define BE_T_CHANGE_TRAIT(trait, sub, res) \
template<typename T> \
struct trait<sub> \
{ \
using Type = res; \
};

#define T_INT_LIMIT_TYPE(type, min, max) \
    template <> struct TGetIntLimit<type> \
    { \
        BE_FORCEINLINE static type GetMin() { return min; }\
        BE_FORCEINLINE static type GetMax() { return max; }\
    };

#define BE_T_ASSERT(assertion_msg, ...) static_assert(__VA_ARGS__, assertion_msg);
#define BE_T_ASSERT_TRAIT(...) BE_T_ASSERT("Failed to substitute type in " BE_STRINGIFY((__VA_ARGS__)), __VA_ARGS__::Value);

#define BE_T_ASSERT_HEAP_PTR(type) BE_T_ASSERT("Heap Pointer must be 16 bytes", sizeof(THeapPointer<type>) == 16);

#define BE_DEFAULT_CONSTRUCTION(type) \
    type() = default; \
    type(const type&) = default; \
    type(type&&) = default;

#define BE_DEFAULT_ASSIGNMENT(type) \
    type operator=(const type&) = default; \
    type operator=(type&&) = default;

#define BE_DISABLE_CONSTRUCTION(type) \
    type##(const type&) = delete; \
    type##(type&&) = delete;

#define BE_DISABLE_ASSIGNMENT(type) \
    type operator=(const type&) = delete; \
    type operator=(type&&) = delete;

//
// Type trait substitutions
//
// Set valid traits

template<class TA>
struct TIsSame<TA, TA> { enum { Value = true }; };

BE_T_VALID_TRAIT(TIsArithmetic, ansi)
BE_T_VALID_TRAIT(TIsArithmetic, unicode)
BE_T_VALID_TRAIT(TIsArithmetic, int8)
BE_T_VALID_TRAIT(TIsArithmetic, int16)
BE_T_VALID_TRAIT(TIsArithmetic, int32)
BE_T_VALID_TRAIT(TIsArithmetic, int64)
BE_T_VALID_TRAIT(TIsArithmetic, long)
BE_T_VALID_TRAIT(TIsArithmetic, uint8)
BE_T_VALID_TRAIT(TIsArithmetic, uint16)
BE_T_VALID_TRAIT(TIsArithmetic, uint32)
BE_T_VALID_TRAIT(TIsArithmetic, uint64)
BE_T_VALID_TRAIT(TIsArithmetic, unsigned long)
BE_T_VALID_TRAIT(TIsArithmetic, float)
BE_T_VALID_TRAIT(TIsArithmetic, double)
BE_T_VALID_TRAIT(TIsArithmetic, long double)
BE_T_VALID_ALL_CV_TRAIT(TIsArithmetic)

BE_T_VALID_TRAIT(TIsFloatingPoint, float)
BE_T_VALID_TRAIT(TIsFloatingPoint, double)
BE_T_VALID_TRAIT(TIsFloatingPoint, long double)
BE_T_VALID_ALL_CV_TRAIT(TIsFloatingPoint)

BE_T_VALID_TRAIT(TIsVoid, void)

template<typename TRet, typename ... TParams>
struct TIsFunction<TRet(TParams...)> { enum { Value = true }; };

template<typename T>
struct TIsPointer<T*> { enum { Value = true }; };

BE_T_VALID_ALL_CV_TRAIT(TIsPointer)

template<typename T>
struct TIsConst<const T> { enum { Value = true } ; };

template<typename T>
struct TIsConst<const volatile T> { enum { Value = true } ; };

template<typename T>
struct TIsVolatile<volatile T> { enum { Value = true } ; };

template<typename T>
struct TIsVolatile<const volatile T> { enum { Value = true } ; };
//
// Expose names
//
BE_T_EXPOSE_NAME(int8)
BE_T_EXPOSE_NAME(int16)
BE_T_EXPOSE_NAME(int32)
BE_T_EXPOSE_NAME(int64)
BE_T_EXPOSE_NAME(uint8)
BE_T_EXPOSE_NAME(uint16)
BE_T_EXPOSE_NAME(uint32)
BE_T_EXPOSE_NAME(uint64)
BE_T_EXPOSE_NAME(long)
BE_T_EXPOSE_NAME(unsigned long)
BE_T_EXPOSE_NAME(float)
BE_T_EXPOSE_NAME(double)
BE_T_EXPOSE_NAME(long double)
BE_T_EXPOSE_NAME(ansi)
BE_T_EXPOSE_NAME(unicode)
//
// Change traits depending
//
BE_T_CHANGE_TRAIT(TRemoveConst, const T, T)
BE_T_CHANGE_TRAIT(TRemoveConst, volatile T, volatile T)
BE_T_CHANGE_TRAIT(TRemoveConst, const volatile T, const volatile T)

BE_T_CHANGE_TRAIT(TRemoveVolatile, const T, const T)
BE_T_CHANGE_TRAIT(TRemoveVolatile, volatile T, T)
BE_T_CHANGE_TRAIT(TRemoveVolatile, const volatile T, const T)

BE_T_CHANGE_TRAIT(TRemovePointer, T*, T)
BE_T_CHANGE_TRAIT(TRemovePointer, const T*, const T)
BE_T_CHANGE_TRAIT(TRemovePointer, const volatile T*, const volatile T)

BE_T_CHANGE_TRAIT(TRemoveLReference, const T&, const T)
BE_T_CHANGE_TRAIT(TRemoveLReference, volatile T&, volatile T)
BE_T_CHANGE_TRAIT(TRemoveLReference, const volatile T&, const volatile T)

BE_T_CHANGE_TRAIT(TRemoveRReference, T&&, T)
BE_T_CHANGE_TRAIT(TRemoveRReference, volatile T&&, volatile T)
BE_T_CHANGE_TRAIT(TRemoveRReference, const volatile T&&, const volatile T)

BE_T_CHANGE_TRAIT(TAddConst, const T, const T)
BE_T_CHANGE_TRAIT(TAddConst, volatile T, const volatile T)
BE_T_CHANGE_TRAIT(TAddConst, const volatile T, const volatile T)

BE_T_CHANGE_TRAIT(TAddVolatile, const T, const volatile T)
BE_T_CHANGE_TRAIT(TAddVolatile, volatile T, volatile T)
BE_T_CHANGE_TRAIT(TAddVolatile, const volatile T, const volatile T)

BE_T_CHANGE_TRAIT(TAddPointer, T*, T*)
BE_T_CHANGE_TRAIT(TAddPointer, const T, const T*)
BE_T_CHANGE_TRAIT(TAddPointer, const volatile T, const volatile T*)

BE_T_CHANGE_TRAIT(TAddLReference, const T, const T&)
BE_T_CHANGE_TRAIT(TAddLReference, volatile T, volatile T&)
BE_T_CHANGE_TRAIT(TAddLReference, const volatile T, const volatile T&)

BE_T_CHANGE_TRAIT(TAddRReference, const T, const T&&)
BE_T_CHANGE_TRAIT(TAddRReference, volatile T, volatile T&&)
BE_T_CHANGE_TRAIT(TAddRReference, const volatile T, const volatile T&&)

T_INT_LIMIT_TYPE(uint8, 0, 0xffui8)
T_INT_LIMIT_TYPE(int8, (-127i8 - 1), 127i8)
T_INT_LIMIT_TYPE(uint16, 0, 0xffffui16)
T_INT_LIMIT_TYPE(int16, (-32767i16 - 1), 32767i16)
T_INT_LIMIT_TYPE(uint32, 0, 0xffffffffui32)
T_INT_LIMIT_TYPE(int32, (-2147483647i32 - 1), 2147483647i32)
T_INT_LIMIT_TYPE(uint64, 0, 0xffffffffffffffffui64)
T_INT_LIMIT_TYPE(int64, (-9223372036854775807i64 - 1), 9223372036854775807i64)
//
// Type trait asserts
//
BE_T_ASSERT_TRAIT(TIsSame<int8, int8>)
BE_T_ASSERT_TRAIT(!TIsSame<int8, int16>)
BE_T_ASSERT_TRAIT(TIsSame<void, void>)

BE_T_ASSERT_TRAIT(TIsArithmetic<int8>)
BE_T_ASSERT_TRAIT(TIsArithmetic<int16>)
BE_T_ASSERT_TRAIT(!TIsArithmetic<void>)
BE_T_ASSERT_TRAIT(!TIsArithmetic<void*>)

BE_T_ASSERT_TRAIT(TIsFloatingPoint<float>)
BE_T_ASSERT_TRAIT(!TIsFloatingPoint<void>)

BE_T_ASSERT_TRAIT(TIsVoid<void>)
BE_T_ASSERT_TRAIT(!TIsVoid<int8>)

BE_T_ASSERT_TRAIT(TIsFunction<void()>)
BE_T_ASSERT_TRAIT(TIsFunction<void()>)
BE_T_ASSERT_TRAIT(TIsFunction<void(int8)>)
BE_T_ASSERT_TRAIT(TIsFunction<void(int8, int8)>)
BE_T_ASSERT_TRAIT(TIsFunction<void(int8, int8)>)
BE_T_ASSERT_TRAIT(TIsFunction<int8()>)
BE_T_ASSERT_TRAIT(!TIsFunction<int8>)

BE_T_ASSERT_TRAIT(!TIsPointer<int8>)
BE_T_ASSERT_TRAIT(TIsPointer<int8*>)
BE_T_ASSERT_TRAIT(TIsPointer<int8**>)

BE_T_ASSERT_TRAIT(TIsConst<const int8>)
BE_T_ASSERT_TRAIT(!TIsConst<int8>)
BE_T_ASSERT_TRAIT(!TIsConst<volatile int8>)
BE_T_ASSERT_TRAIT(TIsConst<const volatile int8>)

BE_T_ASSERT_TRAIT(TIsVolatile<volatile int8>)
BE_T_ASSERT_TRAIT(!TIsVolatile<int8>)
BE_T_ASSERT_TRAIT(!TIsVolatile<const int8>)
BE_T_ASSERT_TRAIT(TIsVolatile<const volatile int8>)

// Macro to stop naming conflicts in case this is included anywhere else
#define BE_TYPE_TRAIT_DERIVED_TEST(x) BE_Special_Derived_Name_Var_##x
//

struct BE_TYPE_TRAIT_DERIVED_TEST(A) {};
struct BE_TYPE_TRAIT_DERIVED_TEST(B) : BE_TYPE_TRAIT_DERIVED_TEST(A) {};
struct BE_TYPE_TRAIT_DERIVED_TEST(C) {};

BE_T_ASSERT_TRAIT(TIsDerivedFrom<BE_TYPE_TRAIT_DERIVED_TEST(B), BE_TYPE_TRAIT_DERIVED_TEST(A)>)
BE_T_ASSERT_TRAIT(!TIsDerivedFrom<BE_TYPE_TRAIT_DERIVED_TEST(B), BE_TYPE_TRAIT_DERIVED_TEST(C)>)
BE_T_ASSERT_TRAIT(!TIsDerivedFrom<BE_TYPE_TRAIT_DERIVED_TEST(A), BE_TYPE_TRAIT_DERIVED_TEST(B)>)
BE_T_ASSERT_TRAIT(!TIsDerivedFrom<BE_TYPE_TRAIT_DERIVED_TEST(A), BE_TYPE_TRAIT_DERIVED_TEST(C)>)

BE_T_ASSERT_TRAIT(!TIsPointer<TRemovePointer<int*>::Type>)
BE_T_ASSERT_TRAIT(!TIsConst<TRemoveConst<const int>::Type>)
BE_T_ASSERT_TRAIT(!TIsVolatile<TRemoveVolatile<volatile int>::Type>)

BE_T_ASSERT_TRAIT(TIsSame<TRemovePointer<int*>, TRemovePointer<int*>>)

BE_T_ASSERT_TRAIT(TIsPointer<TAddPointer<int>::Type>)
BE_T_ASSERT_TRAIT(TIsConst<TAddConst<int>::Type>)
BE_T_ASSERT_TRAIT(TIsVolatile<TAddVolatile<int>::Type>)

BE_T_ASSERT_TRAIT(TIsSame<TAddPointer<int>, TAddPointer<int>>)

BE_T_ASSERT_TRAIT(TIsPointer<decltype(TUnion<int*, int>().Data)>);
BE_T_ASSERT_TRAIT(TIsPointer<decltype(TUnion<int, int*>().Other)>);
BE_T_ASSERT_TRAIT(TIsPointer<decltype(TUnion<int, int, int*>().Other.Other)>);

BE_T_ASSERT_HEAP_PTR(CHAR)
BE_T_ASSERT_HEAP_PTR(uint32)
BE_T_ASSERT_HEAP_PTR(SizeType)
//
// Function definitions
//
template<typename T>
BE_FORCEINLINE typename TAddRReference<T>::Type BEMove(typename TAddLReference<T>::Type Object)
{
    return static_cast<typename TAddRReference<T>::Type>(Object);
}
//