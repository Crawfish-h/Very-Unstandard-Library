#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "TString.h"
#include "Utility.h"
#include "Reflection.h"

typedef struct TGeneric
{
    TObject* Super;
    TManip_Fns* Manip_Fns;
    bool Is_Allocated;
    TRtti Rtti_;
    void* Data;
} TGeneric;

TGeneric TGeneric_Init(void* data, TRtti rtti, TObject* super);
void TGeneric_Ctor();