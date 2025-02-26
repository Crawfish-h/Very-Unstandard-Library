#pragma once
#include <stddef.h>
#include "../TGeneric.h"

typedef struct TVector TVector;
typedef struct TTree TTree;
typedef struct TTree_Node TTree_Node;
typedef struct TPair TPair;

typedef struct TTree_Node
{
    TTree_Node* Prev;
    TVector* Children; // [TVector] of [TTree_Node*].
    size_t Depth;
    ssize_t Index; // [TTree_Node.Index] is equal to the index of this node inside of [Prev.Children]. < This is WRONG.
    TGeneric Value;
} TTree_Node;

// All types need to come before the values.
TTree* TTree_Init(size_t type_Count, size_t value_Count, ...);

// Adds multiple nodes that hold the supplied values as children of the node at [index] and returns true if the new value was allocated.
void TTree_Multi(TTree* tree, size_t value_Count, ...);

void TTree_Add(TTree* tree, TPair* key_Value_Pair);

// Returns the data at [index].
void* TTree_Get(TTree* tree, ssize_t index);

// Returns the type information along with the data at [index].
TGeneric* TTree_Get1(TTree* tree, ssize_t index);
