#pragma once
#include "TGeneric.h"

typedef struct TPair
{
    TGeneric First;
    TGeneric Second;
} TPair;

// Creates a [TPair] struct and gives the address of it.
#define TP(first_Type, first_Data, second_Type, second_Data) \
    &(TPair){ .First = *TGL(first_Type, (first_Data)), .Second = *TGL(second_Type, (second_Data)) }

