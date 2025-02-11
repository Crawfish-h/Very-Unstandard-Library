#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "TString.h"
#include "Utility.h"
#include "Reflection.h"

typedef struct TGeneric
{
    TObject* Super;
    
    TGeneric (*Dtor)(TGeneric* data);
    TManip_Fns* Manip_Fns;
    bool Is_Allocated;
    TRtti Rtti_;
    void* Data;
} TGeneric;

// Creates a [TGeneric] struct and gives the address of it.
#define TG(type, data) \
    &(TGeneric){ .Data = &data, .Rtti_ = Rtti(type) }

TGeneric TGeneric_Init(void* data, TRtti rtti, TObject* super);
void TGeneric_Ctor();