#pragma once
#include <stddef.h>
#include "../TGeneric.h"
#include "TIterator.h"

typedef struct TDoubly_Node TDoubly_Node;

typedef struct TDoubly_Node
{
    TDoubly_Node* Next;
    TDoubly_Node* Prev;
    TGeneric Value;
} TDoubly_Node;

typedef struct TDoubly_Linked_List
{
    TContainer Super;
    TDoubly_Node* First;
    TDoubly_Node* Last;
    TIterator It;
} TDoubly_Linked_List;

// All types need to come before the values.
TDoubly_Linked_List* TDoubly_Linked_List_Init(uint32_t type_Count, uint32_t value_Count, ...);

// Adds a node that holds the supplied values >AFTER< the [index] argument and returns true if the new value was allocated.
// if [index] equals [TLinked_List.Super.Size - 1] then [TLinked_List.Last] will equal the new node.
bool TDoubly_Linked_List_Add(TDoubly_Linked_List* list, int64_t index, TGeneric* node_Value);

// Adds multiple nodes that hold the supplied values >AFTER< the [index] argument and returns true if the new value was allocated.
// if [index] equals [TLinked_List.Super.Size - 1] then [TLinked_List.Last] will equal the last added node.
bool TDoubly_Linked_List_Multi(TDoubly_Linked_List* list, int64_t index, uint32_t value_Count, ...);

// Returns the data at [index].
void* TDoubly_Linked_List_Get(TDoubly_Linked_List* list, int64_t index);

// Returns the type information along with the data at [index].
const TGeneric* TDoubly_Linked_List_Get_Info(TDoubly_Linked_List* list, int64_t index);

void TDoubly_Linked_List_Free(TDoubly_Linked_List* list);