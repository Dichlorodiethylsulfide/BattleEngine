#pragma once

#include<type_traits>

#define STRINGIFY(x) #x

#define DEFINE_BASIC_TYPE_TRAITS(template_type) \
using ValueType = template_type; \
using SizeType = size_t; \
using Pointer = ValueType*; \
using ConstPointer = const ValueType*; \
using Ref = ValueType&; \
using ConstRef = const ValueType&; \
using MovedType = ValueType&&;

#define DEFINE_ITERATOR_TYPE_TRAITS(template_type) \
using Iterator = Iter<template_type>; \
using ConstIterator = ConstIter<template_type>; \
using ReverseIterator = ReverseIter<template_type>; \
using ConstReverseIterator = ConstReverseIter<template_type>;

#define DEFINE_INHERITED_TYPE_TRAITS(base) \
using Base = base; \
using ValueType = typename Base::ValueType; \
using SizeType = typename Base::SizeType; \
using Pointer = typename Base::ValueType*; \
using ConstPointer = const typename Base::ValueType*; \
using Ref = typename Base::ValueType&; \
using ConstRef = const typename Base::ValueType&; \
using MovedType = typename Base::MovedType;

template<typename TElem>
struct BEVector2D
{
    static_assert(std::is_integral_v<TElem>, "Must be an integral type");

    TElem X;
    TElem Y;
};

template<typename TElem>
struct BEBox
{
    static_assert(std::is_integral_v<TElem>, "Must be an integral type");

    BEVector2D<TElem> TopLeft;
    BEVector2D<TElem> Dimensions;

    static BEBox MakeDimensions(TElem X, TElem Y, TElem Width, TElem Height)
    {
        return {{X, Y}, {Width, Height}};
    }

    bool IsValid() const
    {
        return Dimensions.X != 0 && Dimensions.Y != 0;
    }
};

class External
{
public:
    using ValueType = void*;
    using Pointer = ValueType;
    using ConstPointer = const ValueType;

    External()
    {
    }
    
    External(Pointer pointer)
    {
        this->pointer = pointer;
    }
    
    ~External()
    {
        pointer = {}; // do not delete the pointer here
    }

    Pointer Get() const
    {
        return pointer;
    }
    
    Pointer operator->() const
    {
        return pointer;
    }

    operator bool() const
    {
        return pointer;
    }

    operator Pointer() const
    {
        return pointer;
    }

    template<typename TElem>
    operator TElem*()
    {
        return Cast<TElem>();
    }
    
    template<typename TElem>
    TElem* Cast()
    {
        return static_cast<TElem*>(pointer);
    }

private:
    Pointer pointer{};
};
