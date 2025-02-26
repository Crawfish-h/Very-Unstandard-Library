#pragma once
#include "TGeneric.h"

typedef struct TPair
{
    TGeneric First;
    TGeneric Second;
} TPair;

// Creates a [TPair] struct and gives the address of it.
#define TP(first_Type, first_Data, second_Type, second_Data) \
    &(TPair){ .First = TG(first_Type, (first_Data)), .Second = TG(second_Type, (second_Data)) }

