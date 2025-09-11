#include "TTuple.h"
#include <stdlib.h>

TTuple* TTuple_Init_Fn(TTuple* tuple, Array_Of(TRtti) types, uint32_t type_Count, Array_Of(TGeneric) values, uint32_t value_Count)
{
    tuple->Info_.Types = types;
    tuple->Info_.Type_Count = type_Count;
    tuple->Info_.Type_Capacity = type_Count;
    tuple->Info_.Size = value_Count;
    tuple->Info_.Capacity = value_Count;
    tuple->Values_ = values;
    return tuple;
}

void* TTuple_Get(TTuple* tuple, int32_t index)
{
    if (index < 0) index = tuple->Info_.Size + 1 + index;
    return tuple->Values_[index].Data;
}

TGeneric* TTuple_Get_Info(TTuple* tuple, int32_t index)
{
    if (index < 0) index = tuple->Info_.Size + 1 + index;
    return &tuple->Values_[index];
}

TRtti* TTuple_Get_Type(TTuple* tuple, int32_t index)
{
    if (index < 0) index = tuple->Info_.Size + 1 + index;
    return &tuple->Info_.Types[index];
}