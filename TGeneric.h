#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "TString.h"
#include "Utility.h"

typedef struct TRtti TRtti;

typedef struct TGeneric
{
    TObject* Super;
    TManip_Fns* Manip_Fns;
    bool Is_Allocated;
    TRtti Rtti_;
    void* Data;
    size_t Size_Of;
} TGeneric;

TGeneric TGeneric_Init(void* data, TRtti rtti, TObject* super);
void TGeneric_Ctor();