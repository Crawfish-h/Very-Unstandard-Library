#include "TVector.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Reflection.h"



void TVector_Init(TVector* vector, const size_t capcity, const size_t type_Count, ...)
{
    va_list va_Args;
    va_start(va_Args, type_Count);
    vector->Size_ = 0;
    vector->Capacity_ = capcity;
    vector->Elements_ = calloc(vector->Capacity_, sizeof(TGeneric));
    vector->Type_Count_ = type_Count;
    vector->Type_Capacity_ = type_Count * 2;
    vector->Types_ = malloc(type_Count * sizeof(TRtti));

    for (size_t i = 0; i < type_Count; i++)
    {
        vector->Types_[i] = va_arg(va_Args, TRtti);
    }

    va_end(va_Args);
}

void TVector_Multi(TVector* vector, const size_t value_Count, ...)
{
    va_list va_Args;
    va_start(va_Args, value_Count);

    for (size_t i = 0; i < value_Count; i++)
    {
        vector->Size_++;
        TGeneric* pushed_Value = va_arg(va_Args, TGeneric*);
        Type_Check(&pushed_Value->Rtti_.Type, vector->Types_, vector->Type_Count_);
        if (vector->Size_ == vector->Capacity_)
        {
            vector->Capacity_ *= 2;
            Array_Of(TGeneric) temp_Array = realloc(vector->Elements_, vector->Capacity_ * sizeof(TGeneric));
            if (temp_Array != NULL)
            {
                vector->Elements_ = temp_Array;
            }else
            {
                perror("ERROR: vector->Elements_ could nto be reallocated.");
                free(vector->Elements_);
                free(temp_Array);
                exit(EXIT_FAILURE);
            }
        }

        vector->Elements_[vector->Size_ - 1] = *pushed_Value;
        if (Is_Pointer(pushed_Value->Rtti_) == false)
        {
            vector->Elements_[vector->Size_ - 1].Data = calloc(1, pushed_Value->Rtti_.Size_Of);
            vector->Elements_[vector->Size_ - 1].Is_Allocated = true;
            memcpy(vector->Elements_[vector->Size_ - 1].Data, pushed_Value->Data, pushed_Value->Rtti_.Size_Of);
        }
    }

    va_end(va_Args);
}

void TVector_Push(TVector* vector, TGeneric* value)
{
    TVector_Multi(vector, 1, value);
}

void TVector_Pop(TVector* vector)
{
    if (vector->Elements_[vector->Size_ - 1].Is_Allocated == true)
    {
        free(vector->Elements_[vector->Size_ - 1].Data);
        vector->Elements_[vector->Size_ - 1] = (TGeneric){ NULL };
    }

    vector->Size_--;
}

TGeneric TVector_Pop1(TVector* vector)
{
    TGeneric return_Gen = vector->Elements_[vector->Size_ - 1];
    vector->Elements_[vector->Size_ - 1] = (TGeneric){ NULL };
    vector->Size_--;
    return return_Gen;
}

void TVector_Free(TVector* vector)
{
    for (size_t i = 0; i < vector->Size_; i++)
    {
        if (vector->Elements_[i].Is_Allocated == true)
        {
            free(vector->Elements_[i].Data);
        }
    }

    free(vector->Elements_);
}

TGeneric TVector_Remove_At_Internal(TVector* vector, ssize_t index, bool free_Allocated)
{
    if (index > vector->Size_ - 1)
    {
        fprintf(stderr, "ERROR: vector index out of range. Index argument: %lld. Vector size: %zu.\n", index, vector->Size_);
        exit(EXIT_FAILURE);
    }

    size_t initial_Index = 0;
    int index_Change = 1;
    if (index < 0)
    {
        initial_Index = vector->Size_ - index;
        index_Change = -1;
    }

    for (size_t i = initial_Index; i < vector->Size_;)
    {
        if (i == index)
        {
            TGeneric return_Gen = vector->Elements_[i];

            if (vector->Elements_[i].Is_Allocated == true && free_Allocated == true)
            {
                free(vector->Elements_[i].Data);
            }

            vector->Size_--;
            vector->Elements_[i] = (TGeneric){ NULL };
            return return_Gen;
        }

        i += index_Change;
    }
}

void TVector_Remove_At(TVector* vector, ssize_t index)
{
    TVector_Remove_At_Internal(vector, index, true);
}

TGeneric TVector_Remove_At1(TVector* vector, ssize_t index)
{
    TVector_Remove_At_Internal(vector, index, false);
}