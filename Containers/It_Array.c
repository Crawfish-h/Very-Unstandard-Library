#include "It_Array.h"
#include "TIterator.h"
#include "TContainer.h"
#include  "../Reflection.h"
#include  "../TGeneric.h"

TIterator Make_Iterator(TContainer* container, size_t index);

inline TIterator Make_Iterator(TContainer* container, size_t index)
{
    return (TIterator)
    { 
        .Value = container->Index_Get(container, &(TGeneric){ .Data = &index, .Rtti_ = Rtti(size_t) }),
        .Current_Index = index, 
        .Container_Type = container->Container_Type 
    };
}

inline TIterator It_Array_Begin(TContainer* container)
{
    return Make_Iterator(container, 0);
}

inline TIterator It_Array_At(TContainer* container, size_t index)
{
    return Make_Iterator(container, index);
}

inline TIterator It_Array_End(TContainer* container)
{
    return Make_Iterator(container,  container->Size - 1);
}

inline void It_Array_Next(TContainer* container, TIterator* it)
{
    it->Current_Index++;
    it->Value = container->Index_Get(container, &(TGeneric){ .Data = &it->Current_Index, .Rtti_ = Rtti(size_t) });
}

inline bool It_Array_Cmp(TContainer* container, TIterator* it_0, TIterator* it_1)
{
    if (it_0->Current_Index < it_1->Current_Index + 1)
    {
        return true;
    }
    return false;
}