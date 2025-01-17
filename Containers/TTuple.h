#pragma once
#include <stddef.h>
#include "../Utility.h"
#include "TArray.h"

typedef struct TTuple
{
    TObject* Super;
    TArray Data; // Holds only [TGeneric]s which can store any value.
    TArray Types; // Holds only [TRtti]s
    size_t Size;
} TTuple;

void TTuple_Ctor(TTuple* tuple, const size_t size, ...);