#include "TVector.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Reflection.h"
#include "TContainer.h"
#include "TIterator.h"

void* TVector_Container_Realloc(TContainer* container, uint32_t new_Capacity, TGeneric* arg)
{
    TVector* vector = (TVector*)container;
    vector->Elements = vector->Super.Allocator.Realloc(vector->Elements, new_Capacity * sizeof(TGeneric));
    return vector->Elements;
}

TGeneric TVector_Index_Get(TContainer* container, TGeneric* index);

inline TGeneric TVector_Index_Get(TContainer* container, TGeneric* index)
{
    TVector* vector = (TVector*)container;
    return (TGeneric){ .Data = &vector->Elements[*(uint32_t*)index->Data], .Rtti_ = *Rtti(TGeneric) };
}

TGeneric TVector_Allocator_Free(TContainer* container)
{
    TVector_Free((TVector*)container);
    return (TGeneric){ NULL };
}

Define_Container_Get(TVector, TVector_Get_Info)
Define_Container_Add(TVector, TVector_Add_At)

TVector* TVector_Init(uint32_t type_Count, uint32_t value_Count, ...)
{
    TVector* vector = calloc(1, sizeof(TVector));
    Err_Alloc(vector);
    TContainer* super = &vector->Super;
    TContainer_Init(super, value_Count * 2, type_Count, TC_Allocator_Basic());
    TIterator_Init(&vector->It, TG(TVector*, vector), &vector->Super.Size, Typed_Container_Get_Info, Typed_Container_Add);
    vector->Elements = super->Allocator.Calloc(super->Capacity, sizeof(TGeneric));
    
    va_list va_Args;
    value_Count += type_Count;
    va_start(va_Args, value_Count);
    value_Count -= type_Count;

    for (uint32_t i = 0; i < type_Count; i++)
    {
        super->Types[i] = *va_arg(va_Args, TRtti*);
    }

    for (uint32_t i = 0; i < value_Count; i++)
    {
        TVector_Push(vector, va_arg(va_Args, TGeneric*));
    }

    va_end(va_Args);
    return vector;
}

bool TVector_Grow(TVector* vector, uint32_t new_Capacity)
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

    for (uint32_t i = super->Size; i < super->Capacity; i++)
    {
        vector->Elements[i] = (TGeneric){ NULL };
    }

    if (new_Capacity > super->Capacity) return true;

    return false;
}

void TVector_Multi(TVector* vector, uint32_t value_Count, ...)
{
    va_list va_Args;
    va_start(va_Args, value_Count);
    TContainer* super = &vector->Super;
    
    for (uint32_t i = 0; i < value_Count; i++)
    {
        super->Size++;
        TGeneric* pushed_Value = va_arg(va_Args, TGeneric*);
        Type_Check(&pushed_Value->Rtti_.Type, super->Types, super->Type_Count);
        if (super->Size == super->Capacity) TVector_Grow(vector, super->Capacity * 2);
        
        vector->Elements[super->Size - 1] = *pushed_Value;
        if (Is_Pointer(&pushed_Value->Rtti_) == false)
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

void TVector_Add_At(TVector* vector, int64_t index, TGeneric* value)
{
    TContainer* super = &vector->Super;

    if (index < super->Size)
    {
        Type_Check(&value->Rtti_.Type, super->Types, super->Type_Count);
        super->Size++;
        if (super->Size == super->Capacity) TVector_Grow(vector, super->Capacity * 2);
        Array_Of(TGeneric) temp_Array = super->Allocator.Calloc(super->Capacity, sizeof(TGeneric));
        uint32_t j = 0;
        
        for (uint32_t i = 0; i < super->Size; i++)
        {
            if (i != index)
            {
                temp_Array[i] = vector->Elements[j];
                j++;
            }else
            {
                temp_Array[i] = *value;
                if (Is_Pointer(&value->Rtti_) == false)
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
    for (uint32_t i = 0; i < super->Size; i++)
    {
       TGeneric_Free(&vector->Elements[i]);
    }

    free(vector->Elements);
}

void TVector_Free(TVector* vector)
{
    TVector_Clear(vector);
    free(vector->Super.Types);
    free(vector);
}

void TVector_Remove_At_Internal(TVector* vector, int64_t index, bool free_Allocated)
{
    TContainer* super = &vector->Super;
    if (index > super->Size - 1)
    {
        fprintf(stderr, "ERROR: vector index out of range. Index argument: %lld. Vector size: %zu.\n", index, super->Size);
        exit(EXIT_FAILURE);
    }

    uint32_t initial_Index = 0;
    if (index < 0)
    {
        initial_Index = super->Size - index;
    }

    if (index < super->Size - 1)
    {
        Array_Of(TGeneric) temp_Array = super->Allocator.Calloc(super->Capacity, sizeof(TGeneric));

        uint32_t j = 0;
        for (uint32_t i = 0; i < super->Size; i++)
        {
            if (i != index)
            {
                temp_Array[j] = vector->Elements[i];
                j++;
            }
        }

        if (free_Allocated == true)
        {
            /*if (vector->Elements[index].Dtor == NULL)
            {
                if (vector->Elements[index].Is_Allocated == true)
                {
                    free(vector->Elements[index].Data);
                }
            } else
            {
                vector->Elements[index].Dtor(&vector->Elements[index]);
            }*/
            TGeneric_Free(&vector->Elements[index]);
        }

        super->Size--;
        free(vector->Elements);
        vector->Elements = temp_Array;
    }else
    {
        TVector_Pop(vector);
    }
}

void TVector_Remove_At(TVector* vector, int64_t index)
{
    TVector_Remove_At_Internal(vector, index, false);
}

void TVector_Remove_At1(TVector* vector, int64_t index)
{
    TVector_Remove_At_Internal(vector, index, true);
}

void TVector_Pop(TVector* vector)
{
    TContainer* super = &vector->Super;
    TGeneric_Free(&vector->Elements[super->Size - 1]);
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

TGeneric* TVector_Get_Info(TVector* vector, int64_t index)
{
    return &vector->Elements[index];
}

void* TVector_Get(TVector* vector, int64_t index)
{
    Index_Check(&index, vector->Super.Size);
    return vector->Elements[index].Data;
}