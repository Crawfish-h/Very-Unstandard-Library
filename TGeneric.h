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

// [TGeneric.Data] points to a copy of [data]. 
// Only valid with rvalues.
#define TGR(type, data) \
    &(TGeneric){ .Data = &(struct { type vul_Data_Value_; }){ data }.vul_Data_Value_, .Rtti_ = Rtti(type) }

// [TGeneric.Data] points to [data]. 
// Only valid with lvalues.
#define TGL(type, data) \
    &(TGeneric){ .Data = data, .Rtti_ = Rtti(type) }

/*&(struct { TGeneric Generic_Field; }) \
{ \
    ({ \
        type VUL_TG_data_Value_ = data; \
        (TGeneric){ .Data = &VUL_TG_data_Value_, .Rtti_ = Rtti(type) }; \
    }) \
}.Generic_Field*/


    /* &(TGeneric){ .Data = &(typeof(data)[]){ data }, .Rtti_ = Rtti(data) } */

TGeneric TGeneric_Init(void* data, TRtti rtti, TObject* super);
void TGeneric_Ctor();

