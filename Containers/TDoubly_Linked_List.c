#include "TDoubly_Linked_List.h"
#include <stdarg.h>
#include <stdlib.h>
#include "TContainer.h"
#include "../Reflection.h"
#include "TIterator.h"

Define_Container_Get(TDoubly_Linked_List, TDoubly_Linked_List_Get_Info)
Define_Container_Add(TDoubly_Linked_List, TDoubly_Linked_List_Add)

TDoubly_Linked_List* TDoubly_Linked_List_Init(uint32_t type_Count, uint32_t value_Count, ...)
{
    TDoubly_Linked_List* list = calloc(1, sizeof(TDoubly_Linked_List));
    Err_Alloc(list);
    TContainer* super = &list->Super;
    TContainer_Init(super, value_Count * 2, type_Count, TC_Allocator_Basic());
    TIterator_Init(&list->It, TG(TDoubly_Linked_List*, list), &list->Super.Size, Typed_Container_Get_Info, Typed_Container_Add);
    
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
        TDoubly_Linked_List_Add(list, 0, va_arg(va_Args, TGeneric*));
    }

    for (uint32_t i = type_Count + 1; i < value_Count; i++)
    {
        TDoubly_Linked_List_Multi(list, -1, 1, va_arg(va_Args, TGeneric*));
    }

    va_end(va_Args);
    return list;
}

TDoubly_Node* TDoubly_Linked_List_Get_Node(TDoubly_Linked_List* list, int64_t index)
{
    TDoubly_Node* current_node = NULL;
    TContainer* super = &list->Super;

    if (index < super->Size / 2)
    {
        current_node = list->First;
        for (uint32_t i = 0; i < index; i++)
        {
            if (current_node == NULL)
            {
                fprintf(stderr, "ERORR: invalid index for TDoubly_Linked_List_Get_Node(...). Index argument: %lld. TLinked_List size: %zu.\n", index, super->Size);
                exit(EXIT_FAILURE);
            }

            current_node = current_node->Next;
        }
    }else
    {
        current_node = list->Last;
        for (uint32_t i = super->Size - 1; i > index; i--)
        {
            if (current_node == NULL)
            {
                fprintf(stderr, "ERORR: invalid index for TDoubly_Linked_List_Get_Node(...). Index argument: %lld. TLinked_List size: %zu.\n", index, super->Size);
                exit(EXIT_FAILURE);
            }

            current_node = current_node->Prev;
        }
    }

    return current_node;
}

bool TDoubly_Linked_List_Add(TDoubly_Linked_List* list, int64_t index, TGeneric* node_Value)
{
    TDoubly_Linked_List_Multi(list, index, 1, node_Value);
}

bool TDoubly_Linked_List_Multi(TDoubly_Linked_List* list, int64_t index, uint32_t value_Count, ...)
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
        TDoubly_Node* first_Node = super->Allocator.Calloc(1, sizeof(TDoubly_Node));
        TContainer_Add_If_Pointer(super, &first_Node->Value, va_arg(va_Args, TGeneric*));
        Type_Check(&first_Node->Value.Rtti_.Type, super->Types, super->Type_Count);
        node_Count++;
        super->Size++;
        list->First = first_Node;
        list->Last = first_Node;
    }

    for (; node_Count < value_Count; node_Count++)
    {
        TDoubly_Node* new_Node = super->Allocator.Calloc(1, sizeof(TDoubly_Node));
        TContainer_Add_If_Pointer(super, &new_Node->Value, va_arg(va_Args, TGeneric*));
        Type_Check(&new_Node->Value.Rtti_.Type, super->Types, super->Type_Count);
        int64_t node_Index = index + node_Count;

        TDoubly_Node* indexed_Node = NULL;
        if (node_Index == super->Size)
        {
            indexed_Node = TDoubly_Linked_List_Get_Node(list, node_Index - 1);
            indexed_Node->Next = new_Node;
            new_Node->Prev = indexed_Node;
            list->Last = new_Node;
        }else
        {
            indexed_Node = TDoubly_Linked_List_Get_Node(list, node_Index);
            TDoubly_Node* prev_Node = TDoubly_Linked_List_Get_Node(list, node_Index - 1);

            new_Node->Next = indexed_Node;
            indexed_Node->Prev = new_Node;
            if (node_Index == 0)
            {
                list->First = new_Node;
            }else
            {
                prev_Node->Next = new_Node;
                new_Node->Prev = prev_Node;
            }
        }

        super->Size++;
    }

    va_end(va_Args);
    return did_Allocate;
}

void* TDoubly_Linked_List_Get(TDoubly_Linked_List* list, int64_t index)
{
    return TDoubly_Linked_List_Get_Node(list, index)->Value.Data;
}

TGeneric* TDoubly_Linked_List_Get_Info(TDoubly_Linked_List* list, int64_t index)
{
    return &TDoubly_Linked_List_Get_Node(list, index)->Value;
}

void TDoubly_Linked_List_Free(TDoubly_Linked_List* list)
{
    for (uint32_t i = list->Super.Size - 1; i > -1; i--)
    {
        TDoubly_Node* node = TDoubly_Linked_List_Get_Node(list, i);
        //if (node->Value.Dtor != NULL) node->Value.Dtor(NULL);
        TGeneric_Free(&node->Value);
        free(node);
    }

    free(list->Super.Types);
    free(list);
}