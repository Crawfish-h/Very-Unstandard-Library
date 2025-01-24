#pragma once
#include <stddef.h>
#include "../Utility.h"
#include "../TGeneric.h"

typedef struct TIterator TIterator;

// The base struct for container types.
typedef struct TContainer
{
    TGeneric Data_Ptr; // Data_Ptr.Data should point to a container of some kind (an array, the start of a linked list, etc).
    size_t Size;
    size_t Capacity;
    Array_Of(TRtti) Types;
    size_t Type_Count;
    size_t Type_Capacity;
    TIterator (*C_It_Begin)(TContainer* container);
    TIterator (*C_It_At)(TContainer* container, size_t index);
    TIterator (*C_It_End)(TContainer* container);
    void (*C_It_Next)(TContainer* container, TIterator* it);
    bool (*C_It_Cmp)(TContainer* container, TIterator* it_0, TIterator* it_1);
} TContainer;

// Grows or shrinks the vector depending on the value of [new_Capacity]. 
// Will free elements if the new capacity is smaller than the current size.
// Returns old capacity. 
size_t TContainer_Grow(TContainer* container, size_t new_Capacity);

size_t TContainer_Size(TContainer* container);
size_t TContainer_Capacity(TContainer* container);
size_t TContainer_Type_Count(TContainer* container);
size_t TContainer_Type_Capacity(TContainer* container);
size_t TContainer_Size(TContainer* container);