#include "TTree.h"
#include <stdarg.h>
#include <stdlib.h>
#include "TVector.h"
#include "TContainer.h"
#include "../Reflection.h"
#include "../TPair.h"

TTree_Argument TTree_ArgumentF(int64_t parent_Index, int64_t new_Node_Index, TGeneric* new_Value)
{
    return (TTree_Argument){ .Parent_Index = parent_Index };
}

Define_Container_Get(TTree, TTree_Vector_Get_Info)
Define_Container_Add(TTree, TTree_Add_Define)

TTree* TTree_Init(uint32_t type_Count, uint32_t value_Count, ...)
{
    TTree* tree = calloc(1, sizeof(TTree));
    Err_Alloc(tree);
    TContainer* super = &tree->Super;
    TContainer_Init(super, value_Count * 2, type_Count, TC_Allocator_Basic());
    TIterator_Init(&tree->It, TG(TTree*, tree), &tree->Super.Size, Typed_Container_Get_Info, Typed_Container_Add);
    tree->Nodes = TVector_Init(1, 0, Rtti(TTree_Node*));

    va_list va_Args;
    value_Count += type_Count;
    va_start(va_Args, value_Count);

    for (uint32_t i = 0; i < type_Count; i++)
    {
        super->Types[i] = *va_arg(va_Args, TRtti*);
    }

    for (uint32_t i = 0; i < value_Count - type_Count; i++)
    {
        TTree_Add(tree, va_arg(va_Args, TTree_Argument*));
    }

    va_end(va_Args);
    return tree;
}

void TTree_Get_Node_DFS_Recursive(TTree_Node* current_Node, int64_t index, TTree_Node** out_Node)
{
    for (uint32_t i = 0; i < TContainer_Size((TContainer*)current_Node->Children); i++)
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
TTree_Node* TTree_Get_Node_DFS(TTree* tree, int64_t index)
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
TTree_Node* TTree_Get_Node_BFS(TTree* tree, int64_t index)
{

}

void TTree_Multi(TTree* tree, uint32_t value_Count, ...)
{
    va_list va_Args;
    va_start(va_Args, value_Count);
    TContainer* super = &tree->Super;
    
    uint32_t node_Count = 0;
    if (super->Size == 0)
    {
        super->Size++;
        TTree_Node* first_Node = super->Allocator.Calloc(1, sizeof(TTree_Node));
        TVector_Push(tree->Nodes, TG(TTree_Node*, first_Node));
        tree->First = first_Node;
        first_Node->Children = TVector_Init(1, 0, Rtti(TTree_Node*));
        TTree_Argument* tree_Arg = va_arg(va_Args, TTree_Argument*);
        Type_Check(&tree_Arg->New_Value->Rtti_.Type, super->Types, super->Type_Count);
        first_Node->Index = tree_Arg->New_Node_Index;
        TContainer_Add_If_Pointer(super, &first_Node->Value, tree_Arg->New_Value);
        value_Count--;
    }

    for (uint32_t i = 0; i < value_Count; i++)
    {
        TTree_Argument* tree_Arg = va_arg(va_Args, TTree_Argument*);
        TTree_Node* parent_Node = TTree_Get_Node_DFS(tree, tree_Arg->Parent_Index);
        for (uint32_t i = 0; i < TContainer_Size((TContainer*)parent_Node->Children); i++)
        {
            if (((TTree_Node*)TVector_Get(parent_Node->Children, i))->Index == tree_Arg->New_Node_Index)
            {
                fprintf(stderr, "ERROR: tried adding a node to a parent node that has a child node with the same index.\n");
            }
        }

        TTree_Node* node = super->Allocator.Calloc(1, sizeof(TTree_Node));
        TVector_Push(tree->Nodes, TG(TTree_Node*, node));
        node->Children = TVector_Init(1, 0, Rtti(TTree_Node*));
        Type_Check(&tree_Arg->New_Value->Rtti_.Type, super->Types, super->Type_Count);
        node->Index = tree_Arg->New_Node_Index;
        

        TContainer_Add_If_Pointer(super, &node->Value, tree_Arg->New_Value);
        TVector_Push(parent_Node->Children, TG(TTree_Node*, node));
        super->Size++;
    }

    va_end(va_Args);
}

void TTree_Add(TTree* tree, TTree_Argument* new_Value)
{
    TTree_Multi(tree, 1, new_Value);
}

void TTree_Add_Define(TTree* tree, int64_t index, TGeneric* new_Value)
{
    int64_t parent_Index = 0;
    if (tree->First != NULL) parent_Index = ((TTree_Node*)TVector_Get(tree->Nodes, index - 1))->Index;
    TTree_Multi(tree, 1, &(TTree_Argument){ .Parent_Index = parent_Index, .New_Node_Index = tree->Super.Size, .New_Value = new_Value });
}

void* TTree_Get(TTree* tree, int64_t index)
{
    return TTree_Get_Node_DFS(tree, index)->Value.Data;
}

const TGeneric* TTree_Get_Info(TTree* tree, int64_t index)
{
    return &TTree_Get_Node_DFS(tree, index)->Value;
}

void TTree_Free_Node(TTree_Node* node)
{
    TGeneric_Free(&node->Value);
    TVector_Free(node->Children);
    free(node);
}

void* TTree_Vector_Get(TTree* tree, int64_t index)
{
    return ((TTree_Node*)TVector_Get(tree->Nodes, index))->Value.Data;
}

TGeneric* TTree_Vector_Get_Info(TTree* tree, int64_t index)
{
    return &((TTree_Node*)TVector_Get(tree->Nodes, index))->Value;
}

void Tree_Remove(TTree* tree, int64_t index)
{
    TTree_Node* found_Node = NULL;
    TTree_Node* replacement_Node = NULL;
    if (tree->First->Index == index)
    {
        if (TContainer_Size((TContainer*)tree->First->Children) > 0)
        {
            replacement_Node = TVector_Get(tree->First->Children, 0);
        }else
        {
            TTree_Free_Node(tree->First);
            return;
        }

        found_Node = tree->First;
    }else
    {
        found_Node = TTree_Get(tree, index);
        replacement_Node = found_Node->Parent;
    }

    if (TContainer_Size((TContainer*)found_Node->Children) > 0)
    {
        for (uint32_t i = 0; i < TContainer_Size((TContainer*)found_Node->Children); i++)
        {
            TVector_Push(replacement_Node->Children, TG(TTree_Node*, TVector_Get(found_Node->Children, i)));
        }
    }

    for (uint32_t i = 0; i < TContainer_Size((TContainer*)replacement_Node->Children); i++)
    {
        if (((TTree_Node*)TVector_Get(replacement_Node->Children, i))->Index == index)
        {
            TVector_Remove_At(replacement_Node->Children, i);
            break;
        }
    }
    
    TTree_Free_Node(found_Node);
}

void TTree_Free(TTree* tree)
{
    for (uint32_t i = 0; i < TContainer_Size((TContainer*)tree->Nodes); i++)
    {
        TTree_Free_Node(TVector_Get(tree->Nodes, i));
    }

    TVector_Free(tree->Nodes);
    free(tree->Super.Types);
    free(tree);
}