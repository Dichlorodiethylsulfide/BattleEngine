#pragma once

// Asserts file for testing static conditions, all must pass

#include "BEMemory.h"

//
// Type trait asserts
//
BE_T_ASSERT_TRAIT(TIsSame<Int8, Int8>)
BE_T_ASSERT_TRAIT(!TIsSame<Int8, Int16>)
BE_T_ASSERT_TRAIT(TIsSame<void, void>)

BE_T_ASSERT_TRAIT(TIsArithmetic<Int8>)
BE_T_ASSERT_TRAIT(TIsArithmetic<Int16>)
BE_T_ASSERT_TRAIT(!TIsArithmetic<void>)
BE_T_ASSERT_TRAIT(!TIsArithmetic<void*>)

BE_T_ASSERT_TRAIT(TIsFloatingPoint<float>)
BE_T_ASSERT_TRAIT(!TIsFloatingPoint<void>)

BE_T_ASSERT_TRAIT(TIsVoid<void>)
BE_T_ASSERT_TRAIT(!TIsVoid<Int8>)

BE_T_ASSERT_TRAIT(TIsFunction<void()>)
BE_T_ASSERT_TRAIT(TIsFunction<void()>)
BE_T_ASSERT_TRAIT(TIsFunction<void(Int8)>)
BE_T_ASSERT_TRAIT(TIsFunction<void(Int8, Int8)>)
BE_T_ASSERT_TRAIT(TIsFunction<void(Int8, Int8)>)
BE_T_ASSERT_TRAIT(TIsFunction<Int8()>)
BE_T_ASSERT_TRAIT(!TIsFunction<Int8>)

BE_T_ASSERT_TRAIT(!TIsPointer<Int8>)
BE_T_ASSERT_TRAIT(TIsPointer<Int8*>)
BE_T_ASSERT_TRAIT(TIsPointer<Int8**>)

BE_T_ASSERT_TRAIT(TIsConst<const Int8>)
BE_T_ASSERT_TRAIT(!TIsConst<Int8>)
BE_T_ASSERT_TRAIT(!TIsConst<volatile Int8>)
BE_T_ASSERT_TRAIT(TIsConst<const volatile Int8>)

BE_T_ASSERT_TRAIT(TIsVolatile<volatile Int8>)
BE_T_ASSERT_TRAIT(!TIsVolatile<Int8>)
BE_T_ASSERT_TRAIT(!TIsVolatile<const Int8>)
BE_T_ASSERT_TRAIT(TIsVolatile<const volatile Int8>)

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

BE_T_ASSERT_HEAP_PTR(Char)
BE_T_ASSERT_HEAP_PTR(UInt32)
BE_T_ASSERT_HEAP_PTR(SizeType)
