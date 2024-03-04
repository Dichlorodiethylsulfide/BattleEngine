#pragma once

/* Naming Conventions
 * - BE for Objects
 * - T for templates
 * - E for enums
 * - S for structs
 */

// Type trait definitions

#define STRINGIFY(x) #x
#define FORCE_INLINE __forceinline

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

#if 1
#define TEXT(x) x
using CHAR = ansi;
#else // Unicode
#define TEXT(x) L ## x
using CHAR = unicode;
#endif

template<class, class>
struct TIsSame { enum { Value = false }; };

template<class TA>
struct TIsSame<TA, TA> { enum { Value = true }; };

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
struct TNameOf { FORCE_INLINE static CHAR const* GetName() { return TEXT("Unknown"); } };

#define T_EXPOSE_NAME(type) \
    template<> \
    struct TNameOf<type> { FORCE_INLINE static CHAR const* GetName() { return TEXT(STRINGIFY(type)); } };

#define T_VALID_TRAIT(trait, type) template <> struct trait<type> { enum { Value = true }; };
#define T_VALID_CV_TRAIT(trait, cv) template <typename T> struct trait<cv T> { enum { Value = trait<T>::Value }; };
#define T_VALID_ALL_CV_TRAIT(trait, ptr) \
      T_VALID_CV_TRAIT(trait, const) \
      T_VALID_CV_TRAIT(trait, volatile) \
      T_VALID_CV_TRAIT(trait, const volatile)

#define T_ASSERT_TRAIT(...) static_assert(__VA_ARGS__::Value, "Failed to substitute type in " STRINGIFY(__VA_ARGS__));

//
// Type trait substitutions
T_VALID_TRAIT(TIsArithmetic, ansi)
T_VALID_TRAIT(TIsArithmetic, unicode)
T_VALID_TRAIT(TIsArithmetic, int8)
T_VALID_TRAIT(TIsArithmetic, int16)
T_VALID_TRAIT(TIsArithmetic, int32)
T_VALID_TRAIT(TIsArithmetic, int64)
T_VALID_TRAIT(TIsArithmetic, long)
T_VALID_TRAIT(TIsArithmetic, uint8)
T_VALID_TRAIT(TIsArithmetic, uint16)
T_VALID_TRAIT(TIsArithmetic, uint32)
T_VALID_TRAIT(TIsArithmetic, uint64)
T_VALID_TRAIT(TIsArithmetic, unsigned long)
T_VALID_TRAIT(TIsArithmetic, float)
T_VALID_TRAIT(TIsArithmetic, double)
T_VALID_TRAIT(TIsArithmetic, long double)
T_VALID_ALL_CV_TRAIT(TIsArithmetic)

T_VALID_TRAIT(TIsFloatingPoint, float)
T_VALID_TRAIT(TIsFloatingPoint, double)
T_VALID_TRAIT(TIsFloatingPoint, long double)
T_VALID_ALL_CV_TRAIT(TIsFloatingPoint)

T_VALID_TRAIT(TIsVoid, void)

template<typename TRet, typename ... TParams>
struct TIsFunction<TRet(TParams...)> { enum { Value = true }; };

template<typename T>
struct TIsPointer<T*> { enum { Value = true }; };

T_VALID_ALL_CV_TRAIT(TIsPointer)

T_EXPOSE_NAME(int8)
T_EXPOSE_NAME(int16)
T_EXPOSE_NAME(int32)
T_EXPOSE_NAME(int64)
T_EXPOSE_NAME(uint8)
T_EXPOSE_NAME(uint16)
T_EXPOSE_NAME(uint32)
T_EXPOSE_NAME(uint64)
T_EXPOSE_NAME(long)
T_EXPOSE_NAME(unsigned long)
T_EXPOSE_NAME(float)
T_EXPOSE_NAME(double)
T_EXPOSE_NAME(long double)
T_EXPOSE_NAME(ansi)
T_EXPOSE_NAME(unicode)
//
// Type trait asserts
T_ASSERT_TRAIT(TIsSame<int8, int8>)
T_ASSERT_TRAIT(!TIsSame<int8, int16>)
T_ASSERT_TRAIT(TIsSame<void, void>)

T_ASSERT_TRAIT(TIsArithmetic<int8>)
T_ASSERT_TRAIT(TIsArithmetic<int16>)
T_ASSERT_TRAIT(!TIsArithmetic<void>)
T_ASSERT_TRAIT(!TIsArithmetic<void*>)

T_ASSERT_TRAIT(TIsFloatingPoint<float>)
T_ASSERT_TRAIT(!TIsFloatingPoint<void>)

T_ASSERT_TRAIT(TIsVoid<void>)
T_ASSERT_TRAIT(!TIsVoid<int8>)

T_ASSERT_TRAIT(TIsFunction<void()>)
T_ASSERT_TRAIT(TIsFunction<void()>)
T_ASSERT_TRAIT(TIsFunction<void(int8)>)
T_ASSERT_TRAIT(TIsFunction<void(int8, int8)>)
T_ASSERT_TRAIT(TIsFunction<int8()>)
T_ASSERT_TRAIT(!TIsFunction<int8>)

T_ASSERT_TRAIT(!TIsPointer<int8>)
T_ASSERT_TRAIT(TIsPointer<int8*>)
//