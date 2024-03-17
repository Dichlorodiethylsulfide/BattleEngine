#pragma once

// All definitions in BETypeTraits.h will be moved into appropriate files / folders when ready

#ifdef _WIN32
#include <intrin.h>
// __nop needed for Windows compiler to stop at correct debug location
#define NOP __nop();
#else
#define NOP
#endif
// Avoid at all costs putting includes for other files in this file

#define PLATFORM_BREAK_ { NOP __debugbreak(); }
#define PLATFORM_BREAK(x) PLATFORM_BREAK_

/* Naming Conventions
 * - BE for BE-specific Objects
 * - S for non-BEObjects ???
 * - T for templates
 * - E for enums
 */

#define BE_RUN_TESTS_CASES 1

#define BE_USE_STD_ATOMICS 1 // my intention is to implement my own but use this for now
#define BE_USE_STD_THREADS 1 // my intention is to implement my own but use this for now

// Type trait definitions
#define BE_STRINGIFY(x) #x
#define BE_CONCAT_IMPL(x, y) x##y
#define BE_CONCAT(x, y) BE_CONCAT_IMPL(x, y)
#define BE_FORCEINLINE __forceinline

using Int8 = signed char;
using Int16 = signed short;
using Int32 = signed int;
using Int64 = signed long long;
using UInt8 = unsigned char;
using UInt16 = unsigned short;
using UInt32 = unsigned int;
using UInt64 = unsigned long long;
using Ansi = char;
using Unicode = wchar_t;
using SizeType = UInt64;
using UIntPtr = UInt64;
using Null = decltype(nullptr);

struct UInt128
{
    UInt64 High;
    UInt64 Low;
};

#if 1
#define TEXT(x) x
using Char = Ansi;
#else // Unicode
#define TEXT(x) L ## x
using Char = Unicode;
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
struct TNameOf { BE_FORCEINLINE static Char const* GetName() { return TEXT("Unknown"); } };

template<class TDerived, class TBase>
struct TIsDerivedFrom
{
    using No = Int8;
    using Yes = Int16;

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

template<class>
struct TIsLReference { enum { Value = false }; };

template<class>
struct TIsRReference { enum { Value = false }; };

template<typename T>
struct TIsReference { enum { Value = TIsLReference<T>::Value || TIsRReference<T>::Value }; };

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
struct TRemoveReference
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

// All interfaces should be considered IInterfaces
class IInterface
{
};

class IConstCollection : IInterface
{
public:
    IConstCollection() {}
    virtual ~IConstCollection() {}
    virtual bool IsEmpty() const = 0;
    virtual SizeType Length() const = 0;
};

// All non-constant collections can be considered read-only (constant)
// However, constant collections cannot be considered read-write (non-constant)
class ICollection : public IConstCollection
{
public:
    ICollection() {}
    virtual ~ICollection() {}
    virtual void Clear() = 0;
};
// IInterfaces

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
    struct TNameOf<type> { BE_FORCEINLINE static Char const* GetName() { return TEXT(BE_STRINGIFY(type)); } };

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
#define BE_T_ASSERT_SAME_SIZE(type, type2) BE_T_ASSERT(BE_STRINGIFY(type) " must be the same size as " BE_STRINGIFY(type2), sizeof(type) == sizeof(type2))

#define BE_LIKELY(x) (!!(x))
#define BE_UNLIKELY(x) (!(BE_LIKELY(x)))

#define BE_CHECK(x) \
    if(BE_LIKELY(x)) \
    { \
        PLATFORM_BREAK("Failed check") \
    }

#define BE_CHECK_LOG(x, y) BE_CHECK(x) // add logging to this macro

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

#define BE_T_DEFINE_TRAIT_TYPES(x) template<typename T> using x##T = typename x##<T>::Type;

//
// Type trait substitutions
//
// Set valid traits

template<class TA>
struct TIsSame<TA, TA> { enum { Value = true }; };

BE_T_VALID_TRAIT(TIsArithmetic, Ansi)
BE_T_VALID_TRAIT(TIsArithmetic, Unicode)
BE_T_VALID_TRAIT(TIsArithmetic, Int8)
BE_T_VALID_TRAIT(TIsArithmetic, Int16)
BE_T_VALID_TRAIT(TIsArithmetic, Int32)
BE_T_VALID_TRAIT(TIsArithmetic, Int64)
BE_T_VALID_TRAIT(TIsArithmetic, long)
BE_T_VALID_TRAIT(TIsArithmetic, UInt8)
BE_T_VALID_TRAIT(TIsArithmetic, UInt16)
BE_T_VALID_TRAIT(TIsArithmetic, UInt32)
BE_T_VALID_TRAIT(TIsArithmetic, UInt64)
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

template<typename T>
struct TIsLReference<T&> { enum { Value = true } ; };

template<typename T>
struct TIsRReference<T&&> { enum { Value = true } ; };
//
// Expose names
//
BE_T_EXPOSE_NAME(Int8)
BE_T_EXPOSE_NAME(Int16)
BE_T_EXPOSE_NAME(Int32)
BE_T_EXPOSE_NAME(Int64)
BE_T_EXPOSE_NAME(UInt8)
BE_T_EXPOSE_NAME(UInt16)
BE_T_EXPOSE_NAME(UInt32)
BE_T_EXPOSE_NAME(UInt64)
BE_T_EXPOSE_NAME(long)
BE_T_EXPOSE_NAME(unsigned long)
BE_T_EXPOSE_NAME(float)
BE_T_EXPOSE_NAME(double)
BE_T_EXPOSE_NAME(long double)
BE_T_EXPOSE_NAME(Ansi)
BE_T_EXPOSE_NAME(Unicode)
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

BE_T_CHANGE_TRAIT(TRemoveReference, T&&, T)
BE_T_CHANGE_TRAIT(TRemoveReference, T&, T)

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

T_INT_LIMIT_TYPE(UInt8, 0, 0xffui8)
T_INT_LIMIT_TYPE(Int8, (-127i8 - 1), 127i8)
T_INT_LIMIT_TYPE(UInt16, 0, 0xffffui16)
T_INT_LIMIT_TYPE(Int16, (-32767i16 - 1), 32767i16)
T_INT_LIMIT_TYPE(UInt32, 0, 0xffffffffui32)
T_INT_LIMIT_TYPE(Int32, (-2147483647i32 - 1), 2147483647i32)
T_INT_LIMIT_TYPE(UInt64, 0, 0xffffffffffffffffui64)
T_INT_LIMIT_TYPE(Int64, (-9223372036854775807i64 - 1), 9223372036854775807i64)

BE_T_DEFINE_TRAIT_TYPES(TRemoveConst)
BE_T_DEFINE_TRAIT_TYPES(TRemoveVolatile)
BE_T_DEFINE_TRAIT_TYPES(TRemovePointer)
BE_T_DEFINE_TRAIT_TYPES(TRemoveReference)
BE_T_DEFINE_TRAIT_TYPES(TAddConst)
BE_T_DEFINE_TRAIT_TYPES(TAddVolatile)
BE_T_DEFINE_TRAIT_TYPES(TAddPointer)
BE_T_DEFINE_TRAIT_TYPES(TAddLReference)
BE_T_DEFINE_TRAIT_TYPES(TAddRReference)
//
// Function definitions
//
template<typename T>
BE_FORCEINLINE constexpr T&& BEForward(TRemoveReferenceT<T>& Object) noexcept
{
    return static_cast<T&&>(Object);
}

template<typename T>
BE_FORCEINLINE constexpr T&& BEForward(TRemoveReferenceT<T>&& Object) noexcept
{
    BE_T_ASSERT("Bad forward call", !TIsLReference<T>::Value)
    return static_cast<T&&>(Object);
}

template<typename T>
BE_FORCEINLINE constexpr TRemoveReferenceT<T>&& BEMove(T&& Object) noexcept
{
    return static_cast<TRemoveReferenceT<T>&&>(Object);
}
/*_EXPORT_STD template <class _Ty>
_NODISCARD _MSVC_INTRINSIC constexpr remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept {
    return static_cast<remove_reference_t<_Ty>&&>(_Arg);
}*/
//
