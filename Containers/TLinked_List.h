#pragma once
#include <stddef.h>
#include "../TGeneric.h"
#include "TIterator.h"

typedef struct TNode TNode;

typedef struct TNode
{
    TNode* Next;
    TGeneric Value;
} TNode;

typedef struct TLinked_List
{
    TContainer Super;
    TNode* First;
    TNode* Last;
    TIterator It;
} TLinked_List;

// all types need to come before the values.
TLinked_List* TLinked_List_Init(uint32_t type_Count, uint32_t value_Count, ...);

// Adds a node that holds the supplied values >AFTER< the [index] argument and returns true if the new value was allocated.
// if [index] equals [TLinked_List.Super.Size - 1] then [TLinked_List.Last] will equal the new node.
bool TLinked_List_Add(TLinked_List* list, int64_t index, TGeneric* node_Value);

// Adds multiple nodes that hold the supplied values >AFTER< the [index] argument and returns true if the new value was allocated.
// if [index] equals [TLinked_List.Super.Size - 1] then [TLinked_List.Last] will equal the last added node.
bool TLinked_List_Multi(TLinked_List* list, int64_t index, uint32_t value_Count, ...);

// Returns the data at [index].
void* TLinked_List_Get(TLinked_List* list, int64_t index);

// Returns the type information along with the data at [index].
TGeneric* TLinked_List_Get_Info(TLinked_List* list, int64_t index);

void TLinked_List_Free(TLinked_List* list);