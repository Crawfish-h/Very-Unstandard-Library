#pragma once
#include <stddef.h>
#include "../TGeneric.h"

typedef struct TNode TNode;

typedef struct TNode
{
    TNode* Next;
    TGeneric Value;
} TNode;

typedef struct TLinked_List TLinked_List;

// all types need to come before the values.
TLinked_List* TLinked_List_Init(size_t type_Count, size_t value_Count, ...);

// Adds nodes that hold the supplied values >AFTER< the [index] argument and returns true if the new value was allocated.
// Only way to set [TLinked_List.Last].
bool TLinked_List_Add(TLinked_List* list, ssize_t index, size_t value_Count, ...);

// Adds nodes that hold the supplied values >BEFORE< the [index] argument and returns true if the new value was allocated.
// Only way to set [TLinked_List.First].
bool TLinked_List_Add_Before(TLinked_List* list, ssize_t index, size_t value_Count, ...);

// Returns the data at [index].
void* TLinked_List_Get(TLinked_List* list, ssize_t index);

// Returns the type information along with the data at [index].
TGeneric* TLinked_List_Get1(TLinked_List* list, ssize_t index);