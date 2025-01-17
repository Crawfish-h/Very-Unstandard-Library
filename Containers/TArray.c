#include "TArray.h"
#include <stdlib.h>
#include <stdbool.h>
#include "../Reflection.h"

TArray TArray_Init(TRtti rtti, size_t size, void* array)
{
    TArray tarray = { .Rtti_ = rtti, .Size = size, .Data = array, .Is_Allocated = false };
    return tarray;
}