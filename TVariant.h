#pragma once
#include "TGeneric.h"

typedef struct TVariant
{
    Array_Of(TRtti) Types_;
    uint32_t Type_Count_;
    TGeneric Value_;
} TVariant;

TVariant* TVariant_Init_Function_Internal(TVariant* variant, Array_Of(TRtti) types, uint32_t type_Count, TGeneric* value);

#define TVariant_Init(rtti_Args, value) \
    TVariant_Init_Function_Internal(&(TVariant){}, (TRtti[]){rtti_Args}, (sizeof((TRtti[]){rtti_Args}) / sizeof((TRtti[]){rtti_Args}[0])), value)
    //{ (TRtti[]){rtti_Args}, (sizeof((TRtti[]){rtti_Args}) / sizeof((TRtti[]){rtti_Args}[0])), value }; 

const TRtti* TVariant_Get_Type(TVariant* variant, uint32_t index);

void* TVariant_Get(TVariant* variant);
void TVariant_Set(TVariant* variant, TGeneric* value);

const TGeneric* TVariant_Get_Info(TVariant* variant);

uint32_t TVariant_Get_Type_Count(TVariant* variant);