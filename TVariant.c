#include "TVariant.h"
#include <stdlib.h>
#include <string.h>

TVariant* TVariant_Init_Function_Internal(TVariant* variant, Array_Of(TRtti) types, uint32_t type_Count, TGeneric* value)
{
    variant->Types_ = types;
    variant->Type_Count_ = type_Count;

    Type_Check(&value->Rtti_.Type, variant->Types_, variant->Type_Count_);
    TGeneric_Add_If_Pointer(&variant->Value_, value);

    return variant;
}

const TRtti* TVariant_Get_Type(TVariant* variant, uint32_t index)
{
    return (variant->Types_ + index);
}

void* TVariant_Get(TVariant* variant)
{
    return variant->Value_.Data;
}

void TVariant_Set(TVariant* variant, TGeneric* value)
{
    Type_Check(&value->Rtti_.Type, variant->Types_, variant->Type_Count_);
    TGeneric_Add_If_Pointer(&variant->Value_, value);
}

const TGeneric* TVariant_Get_Info(TVariant* variant)
{
    return &variant->Value_;
}

uint32_t TVariant_Get_Type_Count(TVariant* variant)
{
    return variant->Type_Count_;
}