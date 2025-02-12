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
        TLinked_List_Add(list, 0, 1, va_arg(va_Args, TGeneric*));
    }

    for (size_t i = type_Count + 1; i < value_Count; i++)
    {
        TLinked_List_Add(list, super->Size - 1, 1, va_arg(va_Args, TGeneric*));
    }

    va_end(va_Args);
    return list;
}

TNode* TLinked_List_Get_Node(TLinked_List* list, ssize_t index)
{
    TNode* current_node = list->First;
    for (size_t j = 0; j < index; j++)
    {
        if (current_node == NULL)
        {
            perror("ERORR: invalid index for TLinked_List_Get_Node(...)");
            exit(EXIT_FAILURE);
        }

        if (j != index)
        {
            current_node = current_node->Next;
        }
    }

    return current_node;
}

bool TLinked_List_Add(TLinked_List* list, ssize_t index, size_t value_Count, ...)
{
    va_list va_Args;
    va_start(va_Args, value_Count);
    TContainer* super = &list->Super;
    bool did_Allocate = false;

    if (index < 0)
    {
        index = super->Size + index;
    }
    
    if ((index > super->Size - 1 || index < 0))
    {
        fprintf(stderr, "ERROR: TLinked_List index out of range. Index argument: %lld. TLinked_List size: %zu.\n", index, super->Size);
        exit(EXIT_FAILURE);
    }

    size_t node_Count = 0;
    if (super->Size == 0)
    {
        TNode* first_Node = super->Allocator.Calloc(1, sizeof(TNode));
        list->First = first_Node;
        list->Last = first_Node;
        first_Node->Value = *va_arg(va_Args, TGeneric*);
        super->Size++;
        node_Count++;
        printf("added node val type: %s\n", first_Node->Value.Rtti_.Type.Str);
        va_end(va_Args);
        return did_Allocate;
    }

    for (; node_Count < value_Count; node_Count++)
    {
        TNode* new_Node = super->Allocator.Calloc(1, sizeof(TNode));
        TNode* indexed_Node = TLinked_List_Get_Node(list, index);
        TNode* old_Next = indexed_Node->Next;
        indexed_Node->Next = new_Node;
        new_Node->Next = old_Next;
        new_Node->Value = *va_arg(va_Args, TGeneric*);
        
        printf("added node val type: %s\n", new_Node->Value.Rtti_.Type.Str);

        if (index == super->Size - 1)
        {
            list->Last = new_Node;
        }

        super->Size++;
    }

    va_end(va_Args);
    return did_Allocate;
}

bool TLinked_List_Add_Before(TLinked_List* list, ssize_t index, size_t value_Count, ...)
{
    va_list va_Args;
    va_start(va_Args, value_Count);
    TContainer* super = &list->Super;
    bool did_Allocate = false;

    if (index < 0)
    {
        index = super->Size - index;
    }

    if (index > super->Size - 1 || index < 0)
    {
        fprintf(stderr, "ERROR: TLinked_List index out of range. Index argument: %lld. TLinked_List size: %zu.\n", index, super->Size);
        exit(EXIT_FAILURE);
    }

    size_t node_Count = 0;
    if (super->Size == 0)
    {
        TNode* first_Node = super->Allocator.Calloc(1, sizeof(TNode));
        list->First = first_Node;
        list->Last = first_Node;
        first_Node->Value = *va_arg(va_Args, TGeneric*);
        super->Size++;
        node_Count++;
        printf("added node val type: %s\n", first_Node->Value.Rtti_.Type.Str);
        va_end(va_Args);
        return did_Allocate;
    }

    for (; node_Count < value_Count; node_Count++)
    {
        TNode* new_Node = super->Allocator.Calloc(1, sizeof(TNode));
        TNode* indexed_Node = TLinked_List_Get_Node(list, index);
        TNode* old_Next = indexed_Node->Next;
        indexed_Node->Next = new_Node;
        new_Node->Next = old_Next;
        new_Node->Value = *va_arg(va_Args, TGeneric*);

        if (index == super->Size - 1)
        {
            list->Last = new_Node;
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