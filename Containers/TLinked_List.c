#include "TLinked_List.h"
#include <stdarg.h>
#include <stdlib.h>
#include "TContainer.h"
#include "../Reflection.h"
#include "TIterator.h"

Define_Container_Get(TLinked_List, TLinked_List_Get_Info)
Define_Container_Add(TLinked_List, TLinked_List_Add)

TLinked_List* TLinked_List_Init(uint32_t type_Count, uint32_t value_Count, ...)
{
    TLinked_List* list = calloc(1, sizeof(TLinked_List));
    Err_Alloc(list);
    TContainer* super = &list->Super;
    TContainer_Init(super, value_Count * 2, type_Count, TC_Allocator_Basic());
    TIterator_Init(&list->It, TG(TLinked_List*, list), &list->Super.Size, Typed_Container_Get_Info, Typed_Container_Add);

    va_list va_Args;
    value_Count += type_Count;
    va_start(va_Args, value_Count);

    for (uint32_t i = 0; i < type_Count; i++)
    {
        super->Types[i] = *va_arg(va_Args, TRtti*);
    }

    list->First = NULL;
    list->Last = NULL;

    if (value_Count - type_Count > 0)
    {
        TLinked_List_Add(list, 0, va_arg(va_Args, TGeneric*));
    }

    for (uint32_t i = type_Count + 1; i < value_Count; i++)
    {
        TLinked_List_Multi(list, -1, 1, va_arg(va_Args, TGeneric*));
    }

    va_end(va_Args);
    return list;
}

TNode* TLinked_List_Get_Node(TLinked_List* list, int64_t index)
{
    TNode* current_node = list->First;
    TContainer* super = &list->Super;

    for (uint32_t i = 0; i < index; i++)
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

bool TLinked_List_Add(TLinked_List* list, int64_t index, TGeneric* node_Value)
{
    TLinked_List_Multi(list, index, 1, node_Value);
}

bool TLinked_List_Multi(TLinked_List* list, int64_t index, uint32_t value_Count, ...)
{
    va_list va_Args;
    va_start(va_Args, value_Count);
    TContainer* super = &list->Super;
    bool did_Allocate = false;

    if (index < 0)
    {
        index = super->Size + 1 + index;
    }

    uint32_t node_Count = 0;
    if (super->Size == 0)
    {
        TNode* first_Node = super->Allocator.Calloc(1, sizeof(TNode));
        TContainer_Add_If_Pointer(super, &first_Node->Value, va_arg(va_Args, TGeneric*));
        Type_Check(&first_Node->Value.Rtti_.Type, super->Types, super->Type_Count);
        node_Count++;
        super->Size++;
        list->First = first_Node;
        list->Last = first_Node;
    }

    for (; node_Count < value_Count; node_Count++)
    {
        TNode* new_Node = super->Allocator.Calloc(1, sizeof(TNode));
        TContainer_Add_If_Pointer(super, &new_Node->Value, va_arg(va_Args, TGeneric*));
        Type_Check(&new_Node->Value.Rtti_.Type, super->Types, super->Type_Count);
        int64_t node_Index = index + node_Count;

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

void* TLinked_List_Get(TLinked_List* list, int64_t index)
{
    return TLinked_List_Get_Node(list, index)->Value.Data;
}

TGeneric* TLinked_List_Get_Info(TLinked_List* list, int64_t index)
{
    return &TLinked_List_Get_Node(list, index)->Value;
}

void TLinked_List_Set(TLinked_List* list, int64_t index, TGeneric* new_Value)
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

void TLinked_List_Free(TLinked_List* list)
{
    for (uint32_t i = list->Super.Size - 1; i > -1; i--)
    {
        TNode* node = TLinked_List_Get_Node(list, i);
        //if (node->Value.Dtor != NULL) node->Value.Dtor(NULL);
        TGeneric_Free(&node->Value);
        free(node);
    }

    free(list->Super.Types);
    free(list);
}