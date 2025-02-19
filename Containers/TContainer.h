#pragma once
#include <stddef.h>
#include "../Utility.h"
#include "../TGeneric.h"
#include "../Reflection.h"
#include "TC_Allocator.h"

typedef struct TIterator TIterator;
typedef struct TContainer TContainer;

// The base struct for container types.
typedef struct TContainer
{
    TC_Allocator Allocator;
    void** Data_Ptr; // Data_Ptr.Data should point to a container of some kind (an array, the start of a linked list, etc).
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
    void* (*Container_Realloc)(TContainer* container, size_t new_Capacity, TGeneric* arg);
    TGeneric* (*Get)(TContainer* container, ssize_t index);
    TRtti Container_Type;
} TContainer;

// Grows or shrinks the vector depending on the value of [new_Capacity]. 
// Will free elements if the new capacity is smaller than the current size.
// Returns old capacity. 
size_t TContainer_Grow(TContainer* container, size_t new_Capacity, TGeneric* arg);

size_t TContainer_Size(TContainer* container);
size_t TContainer_Capacity(TContainer* container);
size_t TContainer_Type_Count(TContainer* container);
size_t TContainer_Type_Capacity(TContainer* container);
size_t TContainer_Size(TContainer* container);
void* TContainer_Array_Alloc_Again(TContainer* container, size_t new_Capacity, TGeneric* arg);

#define Define_Container_Get(container_Type, get_Function) \
static TGeneric* Typed_Container_Get(TContainer* container, ssize_t index) \
{ \
    container_Type* casted_Container = (container_Type*)container; \
    return get_Function(casted_Container, index); \
}

