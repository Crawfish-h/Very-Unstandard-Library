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


TTree_Argument TTree_ArgumentF(ssize_t parent_Index, ssize_t new_Node_Index, TGeneric* new_Value)
{
    return (TTree_Argument){ .Parent_Index = parent_Index };
}

Define_Container_Get(TTree, TTree_Get_Info)
Define_Container_Add(TTree, TTree_Add_Define)

TTree* TTree_Init(size_t type_Count, size_t value_Count, ...)
{
    TTree* tree = calloc(1, sizeof(TTree));
    Err_Alloc(tree);
    TContainer* super = &tree->Super;
    TContainer_Init(super, value_Count * 2, type_Count, Typed_Container_Get, Typed_Container_Add, TC_Allocator_Basic());

    va_list va_Args;
    value_Count += type_Count;
    va_start(va_Args, value_Count);

    for (size_t i = 0; i < type_Count; i++)
    {
        super->Types[i] = va_arg(va_Args, TRtti);
    }

    for (size_t i = 0; i < value_Count - type_Count; i++)
    {
        TTree_Add(tree, va_arg(va_Args, TTree_Argument*));
    }

    va_end(va_Args);
    return tree;
}

void TTree_Get_Node_DFS_Recursive(TTree_Node* current_Node, ssize_t index, TTree_Node** out_Node)
{
    for (size_t i = 0; i < TContainer_Size((TContainer*)current_Node->Children); i++)
    {
        if (*out_Node != NULL) return;

        TTree_Node* node = (TTree_Node*)TVector_Get(current_Node->Children, i); 
        if (node->Index == index)
        {
            *out_Node = node;
            return;
        }

        TTree_Get_Node_DFS_Recursive(node, index, out_Node);
    }
}

// Gets a node using Depth First Search.
TTree_Node* TTree_Get_Node_DFS(TTree* tree, ssize_t index)
{
    if (tree->First->Index == index) return tree->First;
    TTree_Node* found_Node = NULL;
    TTree_Get_Node_DFS_Recursive(tree->First, index, &found_Node);
    if (found_Node == NULL) 
    {
        fprintf(stderr, "ERROR: could not find node with index '%d' in [TTree].\n", index);
        exit(EXIT_FAILURE);
    }

    return found_Node;
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
        super->Size++;
        TTree_Node* first_Node = super->Allocator.Calloc(1, sizeof(TTree_Node));
        tree->First = first_Node;
        first_Node->Children = TVector_Init(1, 0, Rtti(TTree_Node*));
        TTree_Argument* tree_Arg = va_arg(va_Args, TTree_Argument*);
        Type_Check(&tree_Arg->New_Value->Rtti_.Type, super->Types, super->Type_Count);
        first_Node->Index = tree_Arg->New_Node_Index;
        TContainer_Add_If_Pointer(super, &first_Node->Value, tree_Arg->New_Value);
        value_Count--;
    }

    for (size_t i = 0; i < value_Count; i++)
    {
        TTree_Node* node = super->Allocator.Calloc(1, sizeof(TTree_Node));
        node->Children = TVector_Init(1, 0, Rtti(TTree_Node*));
        TTree_Argument* tree_Arg = va_arg(va_Args, TTree_Argument*);
        Type_Check(&tree_Arg->New_Value->Rtti_.Type, super->Types, super->Type_Count);
        node->Index = tree_Arg->New_Node_Index;
        TContainer_Add_If_Pointer(super, &node->Value, tree_Arg->New_Value);
        TTree_Node* parent_Node = TTree_Get_Node_DFS(tree, tree_Arg->Parent_Index);
        TVector_Push(parent_Node->Children, TGL(TTree_Node*, node));
        super->Size++;
    }

    va_end(va_Args);
}

void TTree_Add(TTree* tree, TTree_Argument* new_Value)
{
    TTree_Multi(tree, 1, new_Value);
}

void TTree_Add_Define(TTree* tree, ssize_t index, TGeneric* new_Value)
{
    TTree_Multi(tree, 1, &(TTree_Argument){ .Parent_Index = tree->First->Index, .New_Node_Index = index, .New_Value = new_Value });
}

void* TTree_Get(TTree* tree, ssize_t index)
{
    return TTree_Get_Node_DFS(tree, index)->Value.Data;
}

TGeneric* TTree_Get_Info(TTree* tree, ssize_t index)
{
    return &TTree_Get_Node_DFS(tree, index)->Value;
}