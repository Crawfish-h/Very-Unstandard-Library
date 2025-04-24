#include "TTuple.h"
#include <stdlib.h>
#include "TArray.h"
#include "TContainer.h"
#include "../Reflection.h"
#include "../TGeneric.h"

typedef struct TTuple
{
    TContainer Super;
    Array_Of(TMaybe) Array; 
} TTuple;

TTuple TTuple_Init(size_t init_Size, TArray* types, TArray* values)
{
    TTuple tuple = {  };
    TContainer* super = &tuple.Super;
    tuple.Array = (TMaybe*)values->Array;
    super->Types = (TRtti*)types->Array;
    super->Type_Count = types->Size;
    super->Type_Capacity = types->Size;
    super->Size = init_Size;
    super->Capacity = values->Size;

    for (size_t i = 0; i < super->Capacity; i++)
    {
        if (i > super->Size - 1)
        {
            tuple.Array[i] = Nothing;
        }
    }

    return tuple;
}

void TTuple_Set(TTuple* tuple, size_t index, TGeneric* new_Value)
{
    TContainer* super = &tuple->Super;
    if (index > super->Size - 1 || index < 0)
    {
        fprintf(stderr, "ERROR: Index is out of range! TTuple.Super.Size: %zu. Index argument: %zu.\n", super->Size, index);
    }

    if (TString_Equal(&new_Value->Rtti_.Type, NT_TString("NULL")))
    {
        if (tuple->Array[index].Is_Nothing == false)
            super->Size--;
        
    }else if (tuple->Array[index].Is_Nothing == true)
    {
        super->Size++;
    }
    
    Type_Check(&new_Value->Rtti_.Type, super->Types, super->Type_Count);
    *(TGeneric*)tuple->Array[index].Data = *new_Value;
}

TGeneric TTuple_Remove(TTuple* tuple, size_t index)
{
    TContainer* super = &tuple->Super;
    if (index > super->Size - 1 || index < 0)
    {
        fprintf(stderr, "ERROR: Index is out of range! TTuple.Super.Size: %zu. Index argument: %zu.\n", super->Size, index);
    }

    TGeneric value = *(TGeneric*)tuple->Array[index].Data;
    tuple->Array[index] = Nothing;
    super->Size--;
    return value;
}