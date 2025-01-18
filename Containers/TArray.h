#pragma once
#include <stddef.h>
#include "../TGeneric.h"
#include "../TString.h"
#include "../Utility.h"
#include "../TObject.h"
#include "../Reflection.h"


typedef struct TArray
{
    TGeneric;
    size_t Size;
} TArray;

TArray TArray_Init(TRtti rtti, size_t size, void* array);
TGeneric TArray_Ctor(TArray args);