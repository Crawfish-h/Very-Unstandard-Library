#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "TString.h"
#include "Utility.h"
#include "Reflection.h"


typedef struct TGeneric
{    
    TGeneric (*Dtor)(TGeneric* data);
    bool Is_Allocated;
    TRtti Rtti_;
    void* Data;
} TGeneric;

// [TGeneric.Data] points to [data]. 
// Only valid with lvalues.
#define TG(type, data) \
    &(TGeneric){ .Data = data, .Rtti_ = *Rtti(type) }

void TGeneric_Add_If_Pointer(TGeneric* value, TGeneric* new_Value);

// returns [true] if [element] was removed.
bool TGeneric_Free(TGeneric* element);

