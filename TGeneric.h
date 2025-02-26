#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "TString.h"
#include "Utility.h"
#include "Reflection.h"

typedef struct TGeneric
{    
    TGeneric (*Dtor)(TGeneric* data);
    TManip_Fns* Manip_Fns;
    bool Is_Allocated;
    TRtti Rtti_;
    void* Data;
} TGeneric;

/*
// Creates a [TGeneric] struct and gives the address of it.
inline TGeneric TG(TRtti* rtti, void* data);
inline TGeneric TG1(TRtti* rtti, void* data, bool is_Allocated);
inline TGeneric TG2(TRtti* rtti, void* data, bool is_Allocated, TGeneric (*dtor)(TGeneric* data));
*/

#define TG(type, data) \
({ \
    type* VUL_TG_data_Value_ = data; \
    TGeneric* VUL_TG_generic = &(TGeneric){ .Data = &VUL_TG_data_Value_, .Rtti_ = Rtti(type) }; \
    VUL_TG_generic; \
})

    /*&(TGeneric){ .Data = &data, .Rtti_ = Rtti(type) }*/

TGeneric TGeneric_Init(void* data, TRtti rtti, TObject* super);
void TGeneric_Ctor();

