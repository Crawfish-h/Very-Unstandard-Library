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

TVector* TVector_Init(size_t capcity, size_t type_Count, ...)
{
    va_list va_Args;
    va_start(va_Args, type_Count);
    TVector* vector = malloc(sizeof(TVector));
    Err_Alloc(vector);
    vector->Super.Data_Ptr = (void**)&vector->Elements;
    TContainer* super = &vector->Super;
    super->Allocator = TC_Allocator_Basic();
    super->Allocator.Free = TVector_Allocator_Free;
    super->Size = 0;
    super->Capacity = capcity;
    vector->Elements = super->Allocator.Calloc(super->Capacity, sizeof(TGeneric));
    super->Type_Count = type_Count;
    super->Type_Capacity = type_Count * 2;
    super->Types = super->Allocator.Calloc(super->Type_Capacity, type_Count * sizeof(TRtti));
    Err_Alloc(super->Types);
    super->C_It_Begin = It_Array_Begin;
    super->C_It_At = It_Array_At;
    super->C_It_End = It_Array_End;
    super->C_It_Next = It_Array_Next;
    super->C_It_Cmp = It_Array_Cmp;
    super->Container_Realloc = TVector_Container_Realloc;
    super->Index_Get = TVector_Index_Get;
    super->Container_Type = Rtti(TVector);

    for (size_t i = 0; i < type_Count; i++)
    {
        super->Types[i] = va_arg(va_Args, TRtti);
    }

    va_end(va_Args);
    return vector;
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
        if (super->Size == super->Capacity)
        {
            super->Capacity *= 2;
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
        }

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

void TVector_Free(TVector* vector)
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
        }else
        {
            vector->Elements[i].Dtor(&vector->Elements[i]);
        }
    }

    free(vector->Elements);
    free(super->Types);
    free(vector);
}

TGeneric TVector_Remove_At_Internal(TVector* vector, ssize_t index, bool free_Allocated)
{
    TContainer* super = &vector->Super;
    if (index > super->Size - 1)
    {
        fprintf(stderr, "ERROR: vector index out of range. Index argument: %lld. Vector size: %zu.\n", index, super->Size);
        exit(EXIT_FAILURE);
    }

    size_t initial_Index = 0;
    int index_Change = 1;
    if (index < 0)
    {
        initial_Index = super->Size - index;
        index_Change = -1;
    }

    for (size_t i = initial_Index; i < super->Size;)
    {
        if (i == index)
        {
            TGeneric return_Gen = vector->Elements[i];

            if (vector->Elements[i].Is_Allocated == true && free_Allocated == true)
            {
                free(vector->Elements[i].Data);
            }

            super->Size--;
            vector->Elements[i] = (TGeneric){ NULL };
            return return_Gen;
        }

        i += index_Change;
    }
}

void TVector_Remove_At(TVector* vector, ssize_t index)
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

TGeneric TVector_Remove_At1(TVector* vector, ssize_t index)
{
    TVector_Remove_At_Internal(vector, index, false);
}

TGeneric* TVector_Get(TVector* vector, size_t index)
{
    return &vector->Elements[index];
}