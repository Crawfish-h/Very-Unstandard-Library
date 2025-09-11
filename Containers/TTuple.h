#pragma once
#include "../TGeneric.h"
#include "TContainer.h"

typedef struct TTuple
{
    TContainer Info_;
    Array_Of(TGeneric) Values_; 
} TTuple;

TTuple* TTuple_Init_Fn(TTuple* tuple, Array_Of(TRtti) types, uint32_t type_Count, Array_Of(TGeneric) values, uint32_t value_Count);

void* TTuple_Get(TTuple* tuple, int32_t index);
TGeneric* TTuple_Get_Info(TTuple* tuple, int32_t index);
TRtti* TTuple_Get_Type(TTuple* tuple, int32_t index);

#define TTuple_Init(types, values) \
    TTuple_Init_Fn \
    ( \
        &(TTuple){}, \
        (TRtti[]){Expand(types)}, \
        (sizeof((TRtti[]){Expand(types)}) / sizeof((TRtti[]){Expand(types)}[0])) - 1, \
        (TGeneric[]){Expand(values)}, \
        (sizeof((TGeneric[]){Expand(values)}) / sizeof((TGeneric[]){Expand(values)}[0])) - 1 \
    )
