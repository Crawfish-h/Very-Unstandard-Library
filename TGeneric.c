#include <stdlib.h>
#include <string.h>
#include "TGeneric.h"
#include "Reflection.h"

void TGeneric_Add_If_Pointer(TGeneric* value, TGeneric* new_Value)
{
    *value = *new_Value;
    if (Is_Pointer(&new_Value->Rtti_) == false)
    {
        value->Data = calloc(1, new_Value->Rtti_.Size_Of);
        Err_Alloc(value->Data);
        value->Is_Allocated = true;
        memmove(value->Data, new_Value->Data, new_Value->Rtti_.Size_Of);
    }
}