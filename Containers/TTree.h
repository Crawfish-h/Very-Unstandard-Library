#pragma once
#include <stddef.h>
#include "../TGeneric.h"
#include "TIterator.h"

typedef struct TVector TVector;
typedef struct TTree TTree;
typedef struct TTree_Node TTree_Node;
typedef struct TPair TPair;

typedef struct TTree_Node
{
    TTree_Node* Parent;
    TVector* Children; // [TVector] of [TTree_Node*].
    uint32_t Depth;
    int64_t Index; // [TTree_Node.Index] is equal to the index of this node inside of [Prev.Children]. < This is WRONG.
    TGeneric Value;
} TTree_Node;

typedef struct TTree_Argument
{
    int64_t Parent_Index;
    int64_t New_Node_Index;
    TGeneric* New_Value;
} TTree_Argument;

typedef struct TTree
{
    TContainer Super;
    TTree_Node* First;
    TTree_Node* Last;
    TVector* Nodes;
    TIterator It;
} TTree;

// TTree_Argument TTree_Arg(int64_t parent_Index, int64_t new_Node_Index, TGeneric* new_Value);

#define TTree_Arg(parent_Index, new_Node_Index, type_Of_Value, the_Value) \
    &(TTree_Argument) \
    { \
        .Parent_Index = parent_Index, \
        .New_Node_Index = new_Node_Index, \
        .New_Value = TG(type_Of_Value, (the_Value)) \
    }

// All types need to come before the values.
TTree* TTree_Init(uint32_t type_Count, uint32_t value_Count, ...);

// Adds multiple nodes that hold the supplied values as children of the node at [index] and returns true if the new value was allocated.
void TTree_Multi(TTree* tree, uint32_t value_Count, ...);

void TTree_Add(TTree* tree, TTree_Argument* new_Value);

void TTree_Add_Define(TTree* tree, int64_t index, TGeneric* tree_Arg);

// Returns the data at [index] using depth first search.
void* TTree_Get(TTree* tree, int64_t index);

// Returns the type information along with the data at [index].
TGeneric* TTree_Get_Info(TTree* tree, int64_t index);

// Returns the data at [index] using a [TVector].
void* TTree_Vector_Get(TTree* tree, int64_t index);

TGeneric* TTree_Vector_Get_Info(TTree* tree, int64_t index);

void Tree_Remove(TTree* tree, int64_t index);

void TTree_Free(TTree* tree);