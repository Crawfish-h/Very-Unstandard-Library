#pragma once
#include <stdint.h>
#include "TIterator.h"
#include "../TGeneric.h"

typedef struct TRtti TRtti;

typedef struct TArray
{
    void* Data;
    TRtti* Rtti;
    uint32_t Length;
    TIterator It;
    TGeneric* It_Return_Value; // Used for returning a value from It->Get_Info(...);
} TArray;

TArray* TArray_Init(TArray* tarray);
TGeneric* TArray_Get(TIterator* it, int64_t index);
void TArray_Set(TIterator* it, int64_t index, TGeneric* value);

// Creates a TArray.
#define TArrayc(type, value_List) \
    TArray_Init(&(TArray){ .Data = (type[]){ value_List }, .TRtti = Rtti(type), .Length = Array_Length((type[]){ value_List }), .It_Return_Value = TG(type, NULL) });

// Converts a normal array to a TArray.
#define To_TArray(type, array) \
({ \
    uint32_t vul_TArrayc_Macro_Length_0_ = Array_Length((Byte*[]){ value_List }); \
    for (uint32_t vul_TArrayc_Macro_Iterator_0_ = 0; vul_TArrayc_Macro_Iterator_0_ < vul_TArrayc_Macro_Length_0_; vul_TArrayc_Macro_Length_0_++)\
    { \
        \
    } \
    TArray_Init(&(TArray){ .Data = (Byte*[]){ value_List }, .TRtti = Rtti(type), .Length = vul_TArrayc_Macro_Length_0_ }); \
})
