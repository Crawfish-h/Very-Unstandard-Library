#include "TVector.h"
#include <stdarg.h>
#include <stdlib.h>
#include "../Reflection.h"

TVector Memory_Pool;

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
        TGeneric pushed_Value = va_arg(va_Args, TGeneric);
        Type_Check(pushed_Value.Rtti_.Type, vector->Types_, vector->Type_Count_);
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

        vector->Elements_[vector->Size_ - 1] = pushed_Value;
    }

    va_end(va_Args);
}

void TVector_Push(TVector* vector, TGeneric value)
{
    TVector_Multi(vector, 1, value);
}

void Memory_Push()
{
    
    TVector_Multi(&Memory_Pool, 1, );
}