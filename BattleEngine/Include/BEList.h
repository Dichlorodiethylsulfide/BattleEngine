#pragma once

#include "BEArray.h"
#include "BEConsoleIO.h"
#include "BEUtilities.h"

template<typename TElem>
class BEList
{
public:
    DEFINE_BASIC_TYPE_TRAITS(TElem)
    
    struct ElementNode
    {
        struct ElementNode* Next;
        ValueType Value;
        struct ElementNode* Previous;
    };

    using Allocator = BEAllocator<ElementNode>;
    using ValueAllocator = BEAllocator<ValueType>;

    BEList()
    {
        Start = Allocator::Allocate(1);
        Start->Previous = nullptr;
        Start->Next = nullptr;
        Current = Start;
    }

    void PushBack(ConstRef Element)
    {
        EmplaceBack(Element);
    }

    void PushBack(MovedType Element)
    {
        EmplaceBack(std::move(Element));
    }

    template<typename ... Type>
    void EmplaceBack(Type&&... Args)
    {
        EmplaceBack(std::move(std::forward<Type>(Args)...));
    }

    void PopBack()
    {
        
    }

    ElementNode* Begin()
    {
        return Start;
    }

    ElementNode* End()
    {
        return Current;
    }
    
private:
    ElementNode* Start{};
    ElementNode* Current{};
    SizeType ElementCount{};
    
    template<typename Type>
    void EmplaceBack(Type&& Element)
    {
        if(InitialiseStart(std::forward<Type>(Element)))
        {
            Current->Next = Allocator::Allocate(1);
            Current->Next->Previous = Current;
            Current = Current->Next;
            Current->Next = Start;
            ValueAllocator::ConstructNew(&Current->Value, std::forward<Type>(Element));
        }
        ElementCount++;
    }
    template<typename Type>
    bool InitialiseStart(Type&& Args)
    {
        if(!Current->Next)
        {
            Current->Next = Start;
            DEBUG_ENSURE(Current->Previous, "This should be nullptr");
            ValueAllocator::ConstructNew(&Start->Value, std::forward<Type>(Args));
            return false;
        }
        return true;
    }
};