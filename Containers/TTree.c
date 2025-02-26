#include "TTree.h"
#include <stdarg.h>
#include <stdlib.h>
#include "TVector.h"
#include "TContainer.h"
#include "../Reflection.h"
#include "../TPair.h"

typedef struct TTree
{
    TContainer Super;
    TTree_Node* First;
    TVector* Ends;
    bool (*Sort)(size_t index_0, size_t idnex_1);
} TTree;

TTree* TTree_Init(size_t type_Count, size_t value_Count, ...)
{
    TTree* tree = calloc(1, sizeof(TTree));
    Err_Alloc(tree);
    TContainer* super = &tree->Super;
    TContainer_Init(super, type_Count, value_Count);
    super->Container_Type = Rtti(TTree);
    //super->Get = Typed_Container_Get;

    va_list va_Args;
    value_Count += type_Count;
    va_start(va_Args, value_Count);

    for (size_t i = 0; i < type_Count; i++)
    {
        super->Types[i] = va_arg(va_Args, TRtti);
    }

    for (size_t i = 0; i < value_Count - type_Count; i++)
    {
        TTree_Add(tree, va_arg(va_Args, TPair*));
    }

    va_end(va_Args);
    return tree;
}

TTree_Node* TTree_Find_Node_Loop(TTree* tree, ssize_t index, TTree_Node* current_Node)
{
    for (size_t i = 0; i < TContainer_Size((TContainer*)current_Node->Children); i++)
    {
        if (current_Node->Children == NULL)
        {
            return NULL;
        }

        TTree_Node* node = (TTree_Node*)TVector_Get(current_Node->Children, i)->Data;
        if (node->Index == index || TTree_Find_Node_Loop(tree, index, node) != NULL)
        {
            return node;
        }
    }
}

// Gets a node using Depth First Search.
TTree_Node* TTree_Get_Node_DFS(TTree* tree, ssize_t index)
{
    TContainer* super = &tree->Super;
    TTree_Node* current_Node = tree->First;
    size_t current_Depth = 0;

    if (tree->First == NULL || super->Size == 0)
    {
        fprintf(stderr, "ERROR: tried to run function [TTree_Get_Node_DFS] but [tree->First == NULL] or [super->Size == 0]\n");
        exit(EXIT_FAILURE);
    }

    if (current_Node == NULL)
    {
        fprintf(stderr, "ERROR: [current_Node] is [NULL] in function [TTree_Get_Node_DFS]\n");
        exit(EXIT_FAILURE);
    }

    if (current_Node->Index == index)
    {
        return current_Node;
    }

    TTree_Node* find_Node_Result = TTree_Find_Node_Loop(tree, index, tree->First);
    if (find_Node_Result == NULL)
    {
        fprintf(stderr, "ERROR: could not find a [TTree_Node*] with an index of [%lld] in function [TTree_Get_Node_DFS]\n", index);
        exit(EXIT_FAILURE);
    }

    return find_Node_Result;
}

// Gets a node using Breadth First Search.
TTree_Node* TTree_Get_Node_BFS(TTree* tree, ssize_t index)
{

}

void TTree_Multi(TTree* tree, size_t value_Count, ...)
{
    va_list va_Args;
    va_start(va_Args, value_Count);
    TContainer* super = &tree->Super;
    
    size_t node_Count = 0;
    if (super->Size == 0)
    {
        node_Count++;
        super->Size++;
        TTree_Node* first_Node = super->Allocator.Calloc(1, sizeof(TTree_Node));
        TPair* key_Value_Pair = va_arg(va_Args, TPair*);
        first_Node->Index = (ssize_t)key_Value_Pair->First.Data;
        first_Node->Value = key_Value_Pair->Second;
        tree->First = first_Node;
        first_Node->Children = TVector_Init(4, 1, Rtti(TTree_Node*));
    }

    for (size_t i = 0; i < value_Count; i++)
    {
        TTree_Node* new_Node = super->Allocator.Calloc(1, sizeof(TTree_Node));
        TPair* key_Value_Pair = va_arg(va_Args, TPair*);
        new_Node->Index = (size_t)key_Value_Pair->First.Data;
        new_Node->Value = key_Value_Pair->Second;
        TTree_Node* indexed_Node = TTree_Get_Node_DFS(tree, new_Node->Index);
        TVector_Push(indexed_Node->Children, TG(TTree_Node*, new_Node));
        new_Node->Prev = indexed_Node;
        new_Node->Children = TVector_Init(4, 1, Rtti(TTree_Node*));
    }

    va_end(va_Args);
}

void TTree_Add(TTree* tree, TPair* key_Value_Pair)
{
    TTree_Multi(tree, 1, key_Value_Pair);
}

void* TTree_Get(TTree* tree, ssize_t index)
{
    return TTree_Get_Node_DFS(tree, index)->Value.Data;
}

TGeneric* TTree_Get1(TTree* tree, ssize_t index)
{
    return &TTree_Get_Node_DFS(tree, index)->Value;
}