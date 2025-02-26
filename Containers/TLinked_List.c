#include "TLinked_List.h"
#include <stdarg.h>
#include <stdlib.h>
#include "TContainer.h"
#include "../Reflection.h"

typedef struct TLinked_List
{
    TContainer Super;
    TNode* First;
    TNode* Last;
} TLinked_List;

Define_Container_Get(TLinked_List, TLinked_List_Get1)

TLinked_List* TLinked_List_Init(size_t type_Count, size_t value_Count, ...)
{
    TLinked_List* list = malloc(sizeof(TLinked_List));
    Err_Alloc(list);
    TContainer* super = &list->Super;
    super->Allocator = TC_Allocator_Basic();
    //super->Allocator.Free = TVector_Allocator_Free;
    super->Size = 0;
    super->Capacity = value_Count * 2;
    super->Type_Count = type_Count;
    super->Type_Capacity = type_Count * 2;
    super->Types = super->Allocator.Calloc(super->Type_Capacity, type_Count * sizeof(TRtti));
    Err_Alloc(super->Types);
    super->Container_Type = Rtti(TLinked_List);
    super->Get = Typed_Container_Get;
    
    va_list va_Args;
    value_Count += type_Count;
    va_start(va_Args, value_Count);

    for (size_t i = 0; i < type_Count; i++)
    {
        super->Types[i] = va_arg(va_Args, TRtti);
    }

    list->First = NULL;
    list->Last = NULL;

    if (value_Count - type_Count > 0)
    {
        TLinked_List_Add(list, 0, va_arg(va_Args, TGeneric*));
    }

    for (size_t i = type_Count + 1; i < value_Count; i++)
    {
        TLinked_List_Multi(list, 1, 1, va_arg(va_Args, TGeneric*));
    }

    va_end(va_Args);
    return list;
}

TNode* TLinked_List_Get_Node(TLinked_List* list, ssize_t index)
{
    TNode* current_node = list->First;
    TContainer* super = &list->Super;

    for (size_t i = 0; i < index; i++)
    {
        if (current_node == NULL)
        {
            fprintf(stderr, "ERORR: invalid index for TLinked_List_Get_Node(...). Index argument: %lld. TLinked_List size: %zu.\n", index, super->Size);
            exit(EXIT_FAILURE);
        }

        current_node = current_node->Next;
    }

    return current_node;
}

bool TLinked_List_Add(TLinked_List* list, ssize_t index, TGeneric* node_Value)
{
    TLinked_List_Multi(list, index, 1, node_Value);
}

bool TLinked_List_Multi(TLinked_List* list, ssize_t index, size_t value_Count, ...)
{
    va_list va_Args;
    va_start(va_Args, value_Count);
    TContainer* super = &list->Super;
    bool did_Allocate = false;

    if (index < 0)
    {
        index = super->Size + 1 + index;
    }

    size_t node_Count = 0;
    if (super->Size == 0)
    {
        TNode* first_Node = super->Allocator.Calloc(1, sizeof(TNode));
        Type_Check(&first_Node->Value.Rtti_.Type, super->Types, super->Type_Count);
        first_Node->Value = *va_arg(va_Args, TGeneric*);
        node_Count++;
        super->Size++;
        list->First = first_Node;
        list->Last = first_Node;
    }

    for (; node_Count < value_Count; node_Count++)
    {
        TNode* new_Node = super->Allocator.Calloc(1, sizeof(TNode));
        Type_Check(&new_Node->Value.Rtti_.Type, super->Types, super->Type_Count);
        new_Node->Value = *va_arg(va_Args, TGeneric*);
        ssize_t node_Index = index + node_Count;

        TNode* indexed_Node = NULL;
        if (node_Index == super->Size)
        {
            indexed_Node = TLinked_List_Get_Node(list, node_Index - 1);
            indexed_Node->Next = new_Node;
            list->Last = new_Node;
        }else
        {
            indexed_Node = TLinked_List_Get_Node(list, node_Index);
            new_Node->Next = indexed_Node;
            if (node_Index == 0)
            {
                list->First = new_Node;
            }else
            {
                TNode* prev_Node = TLinked_List_Get_Node(list, node_Index - 1);
                prev_Node->Next = new_Node;
            }
        }

        super->Size++;
    }

    va_end(va_Args);
    return did_Allocate;
}

void* TLinked_List_Get(TLinked_List* list, ssize_t index)
{
    return TLinked_List_Get_Node(list, index)->Value.Data;
}

TGeneric* TLinked_List_Get1(TLinked_List* list, ssize_t index)
{
    return &TLinked_List_Get_Node(list, index)->Value;
}

void TLinked_List_Set(TLinked_List* list, ssize_t index, TGeneric* new_Value)
{   
    TGeneric* value = &TLinked_List_Get_Node(list, index)->Value;

    if (value->Dtor == NULL)
    {
        if (value->Is_Allocated == true)
        {
            free(value->Data);
        }
    }else
    {
        value->Dtor(value);
    }

    *value = *new_Value;
}