#include "TVector.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Reflection.h"
#include "TContainer.h"
#include "It_Array.h"

typedef struct TVector
{
    TContainer Super;
    Array_Of(TGeneric) Elements; 
    bool (*Sort)(TGeneric* value);
} TVector;

void* TVector_Container_Realloc(TContainer* container, size_t new_Capacity, TGeneric* arg)
{
    TVector* vector = (TVector*)container;
    vector->Elements = vector->Super.Allocator.Realloc(vector->Elements, new_Capacity * sizeof(TGeneric));
    return vector->Elements;
}

TGeneric TVector_Index_Get(TContainer* container, TGeneric* index);

inline TGeneric TVector_Index_Get(TContainer* container, TGeneric* index)
{
    TVector* vector = (TVector*)container;
    return (TGeneric){ .Data = &vector->Elements[*(size_t*)index->Data], .Rtti_ = Rtti(TGeneric) };
}

TGeneric TVector_Allocator_Free(TContainer* container)
{
    TVector_Free((TVector*)container);
    return (TGeneric){ NULL };
}

Define_Container_Get(TVector, TVector_Get_Info)
Define_Container_Add(TVector, TVector_Add_At)

TVector* TVector_Init(size_t type_Count, size_t value_Count, ...)
{
    TVector* vector = calloc(1, sizeof(TVector));
    Err_Alloc(vector);
    TContainer* super = &vector->Super;
    TContainer_Init(super, value_Count * 2, type_Count, Typed_Container_Get, Typed_Container_Add, TC_Allocator_Basic());
    vector->Elements = super->Allocator.Calloc(super->Capacity, sizeof(TGeneric));
    
    va_list va_Args;
    value_Count += type_Count;
    va_start(va_Args, value_Count);
    value_Count -= type_Count;

    for (size_t i = 0; i < type_Count; i++)
    {
        super->Types[i] = va_arg(va_Args, TRtti);
    }

    for (size_t i = 0; i < value_Count; i++)
    {
        TVector_Push(vector, va_arg(va_Args, TGeneric*));
    }

    va_end(va_Args);
    return vector;
}

bool TVector_Grow(TVector* vector, size_t new_Capacity)
{
    TContainer* super = &vector->Super;
    super->Capacity = new_Capacity;
    Array_Of(TGeneric) temp_Array = realloc(vector->Elements, super->Capacity * sizeof(TGeneric));
    if (temp_Array != NULL)
    {
        vector->Elements = temp_Array;
    }else
    {
        perror("ERROR: TVector->Elements_ could not be reallocated.");
        free(vector->Elements);
        free(temp_Array);
        exit(EXIT_FAILURE);
    }

    for (size_t i = super->Size; i < super->Capacity; i++)
    {
        vector->Elements[i] = (TGeneric){ NULL };
    }

    if (new_Capacity > super->Capacity) return true;

    return false;
}

void TVector_Multi(TVector* vector, size_t value_Count, ...)
{
    va_list va_Args;
    va_start(va_Args, value_Count);
    TContainer* super = &vector->Super;
    
    for (size_t i = 0; i < value_Count; i++)
    {
        super->Size++;
        TGeneric* pushed_Value = va_arg(va_Args, TGeneric*);
        Type_Check(&pushed_Value->Rtti_.Type, super->Types, super->Type_Count);
        if (super->Size == super->Capacity) TVector_Grow(vector, super->Capacity * 2);
        
        vector->Elements[super->Size - 1] = *pushed_Value;
        if (Is_Pointer(pushed_Value->Rtti_) == false)
        {
            vector->Elements[super->Size - 1].Data = super->Allocator.Calloc(1, pushed_Value->Rtti_.Size_Of);
            vector->Elements[super->Size - 1].Is_Allocated = true;
            super->Allocator.Memcpy(vector->Elements[super->Size - 1].Data, pushed_Value->Data, pushed_Value->Rtti_.Size_Of);
        }
    }

    va_end(va_Args);
}

void TVector_Push(TVector* vector, TGeneric* value)
{
    TVector_Multi(vector, 1, value);
}

void TVector_Add_At(TVector* vector, ssize_t index, TGeneric* value)
{
    TContainer* super = &vector->Super;

    if (index < super->Size)
    {
        Type_Check(&value->Rtti_.Type, super->Types, super->Type_Count);
        super->Size++;
        if (super->Size == super->Capacity) TVector_Grow(vector, super->Capacity * 2);
        Array_Of(TGeneric) temp_Array = super->Allocator.Calloc(super->Capacity, sizeof(TGeneric));
        size_t j = 0;
        
        for (size_t i = 0; i < super->Size; i++)
        {
            if (i != index)
            {
                temp_Array[i] = vector->Elements[j];
                j++;
            }else
            {
                printf("value added\n");
                temp_Array[i] = *value;
                if (Is_Pointer(value->Rtti_) == false)
                {
                    temp_Array[index].Data = super->Allocator.Calloc(1, value->Rtti_.Size_Of);
                    temp_Array[index].Is_Allocated = true;
                    super->Allocator.Memcpy(temp_Array[index].Data, value->Data, value->Rtti_.Size_Of);
                }
            }
        }

        free(vector->Elements);
        vector->Elements = temp_Array;
    } else if (index == super->Size)
    {
        TVector_Push(vector, value);
    }else
    {
        fprintf(stderr, "ERROR: in functon [TVector_Add_At], argument [index] is greater than [TVector.Super.Size].\n");
        exit(EXIT_FAILURE);
    }
}

void TVector_Clear(TVector* vector)
{
    TContainer* super = &vector->Super;
    for (size_t i = 0; i < super->Size; i++)
    {
        if (vector->Elements[i].Dtor == NULL)
        {
            if (vector->Elements[i].Is_Allocated == true)
            {
                free(vector->Elements[i].Data);
            }
        } else
        {
            vector->Elements[i].Dtor(&vector->Elements[i]);
        }
    }

    free(vector->Elements);
}

void TVector_Free(TVector* vector)
{
    TVector_Clear(vector);
    free(vector->Super.Types);
    free(vector);
}

void TVector_Remove_At_Internal(TVector* vector, ssize_t index, bool free_Allocated)
{
    TContainer* super = &vector->Super;
    if (index > super->Size - 1)
    {
        fprintf(stderr, "ERROR: vector index out of range. Index argument: %lld. Vector size: %zu.\n", index, super->Size);
        exit(EXIT_FAILURE);
    }

    size_t initial_Index = 0;
    if (index < 0)
    {
        initial_Index = super->Size - index;
    }


    /*TGeneric return_Gen = (TGeneric){  };
    for (size_t i = initial_Index; i < super->Size; i++)
    {
        if (i == index)
        {
            return_Gen = vector->Elements[i];

            if (vector->Elements[i].Is_Allocated == true && free_Allocated == true)
            {
                free(vector->Elements[i].Data);
            }

            super->Size--;
            vector->Elements[i] = (TGeneric){  };
        }
    }*/

    
    if (index < super->Size - 1)
    {
        Array_Of(TGeneric) temp_Array = super->Allocator.Calloc(super->Capacity, sizeof(TGeneric));

        size_t j = 0;
        for (size_t i = 0; i < super->Size; i++)
        {
            if (i != index)
            {
                temp_Array[j] = vector->Elements[i];
                j++;
            }
        }

        if (free_Allocated == true)
        {
            if (vector->Elements[index].Dtor == NULL)
            {
                if (vector->Elements[index].Is_Allocated == true)
                {
                    free(vector->Elements[index].Data);
                }
            } else
            {
                vector->Elements[index].Dtor(&vector->Elements[index]);
            }
        }

        super->Size--;
        free(vector->Elements);
        vector->Elements = temp_Array;
    }
}

void TVector_Remove_At(TVector* vector, ssize_t index)
{
    TVector_Remove_At_Internal(vector, index, false);
}

void TVector_Remove_At1(TVector* vector, ssize_t index)
{
    TVector_Remove_At_Internal(vector, index, true);
}

void TVector_Pop(TVector* vector)
{
    TContainer* super = &vector->Super;
    if (vector->Elements[super->Size - 1].Is_Allocated == true)
    {
        free(vector->Elements[super->Size - 1].Data);
        vector->Elements[super->Size - 1] = (TGeneric){ NULL };
    }

    super->Size--;
}

TGeneric TVector_Pop1(TVector* vector)
{
    TContainer* super = &vector->Super;
    TGeneric return_Gen = vector->Elements[super->Size - 1];
    vector->Elements[super->Size - 1] = (TGeneric){ NULL };
    super->Size--;
    return return_Gen;
}

TGeneric* TVector_Get_Info(TVector* vector, size_t index)
{
    return &vector->Elements[index];
}

void* TVector_Get(TVector* vector, size_t index)
{
    return vector->Elements[index].Data;
}