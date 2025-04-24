#pragma once
#include <stddef.h>
#include "../TGeneric.h"
#include "../TString.h"
#include "../Utility.h"
#include "../Reflection.h"

 typedef struct TRtti TRtti;

typedef struct TArray
{
    TRtti Type;
    void* Array;
    size_t Size;
    bool Is_Allocated;
} TArray;

#define TArray_Init(type, size, array_Values) \
({ \
    type vul_TArray_Init_Array[size] = array_Values; \
    TArray* vul_TArray_Init_TArray_Result = &(TArray){ .Type = Rtti(type), .Array = vul_TArray_Init_Array, .Size = size }; \
    vul_TArray_Init_TArray_Result; \
})
    
#define TArray_Init1(size, array_Values) \
    TArray_Init(NULL, size, array_Values)
