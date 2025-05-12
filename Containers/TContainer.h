#pragma once
#include <stddef.h>
#include "../Utility.h"
#include "TC_Allocator.h"

typedef struct TContainer TContainer;
typedef struct TIterator TIterator;
typedef struct TGeneric TGeneric;
typedef struct TRtti TRtti;

// The base struct for container types.
typedef struct TContainer
{
    TC_Allocator Allocator;
    void** Data_Ptr; // Data_Ptr.Data should point to a container of some kind (an array, the start of a linked list, etc).
    uint32_t Size;
    uint32_t Capacity;
    Array_Of(TRtti) Types;
    uint32_t Type_Count;
    uint32_t Type_Capacity;
} TContainer;

void TContainer_Init(TContainer* container, uint32_t capcity, uint32_t type_Count, TC_Allocator allocator);

// Grows or shrinks the vector depending on the value of [new_Capacity]. 
// Will free elements if the new capacity is smaller than the current size.
// Returns old capacity. 
uint32_t TContainer_Grow(TContainer* container, uint32_t new_Capacity, TGeneric* arg);

uint32_t TContainer_Size(TContainer* container);
uint32_t TContainer_Capacity(TContainer* container);
uint32_t TContainer_Type_Count(TContainer* container);
uint32_t TContainer_Type_Capacity(TContainer* container);
uint32_t TContainer_Size(TContainer* container);
void* TContainer_Array_Alloc_Again(TContainer* container, uint32_t new_Capacity, TGeneric* arg);

void TContainer_Add_If_Pointer(TContainer* container, TGeneric* value, TGeneric* new_Value);

// Returns true if a [TRtti] value was added to [container->Types].
bool TContainer_Add_Type(TContainer* container, TRtti* new_Type);

// Returns true if a [TRtti] value was removed from [container->Types].
bool TContainer_Remove_Type(TContainer* container, TRtti* type);

#define Define_Container_Get(container_Type, get_Function) \
static TGeneric* Typed_Container_Get_Info(TIterator* it, int64_t index) \
{ \
    container_Type* casted_Container = (container_Type*)it->Container.Data; \
    return get_Function(casted_Container, index); \
}

#define Define_Container_Add(container_Type, add_Function) \
static void Typed_Container_Add(TIterator* it, int64_t index, TGeneric* value) \
{ \
    container_Type* casted_Container = (container_Type*)it->Container.Data; \
    add_Function(casted_Container, index, value); \
}

