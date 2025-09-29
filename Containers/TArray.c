#include "TArray.h"
#include <string.h>

TArray* TArray_Init(TArray* tarray)
{
    TIterator_Init(&tarray->It, TG(TArray*, tarray), tarray->Length, TArray_Get, TArray_Set);

    return tarray;
}

TGeneric* TArray_Get(TIterator* it, int64_t index)
{
    TArray* tarray = (TArray*)it->Container.Data;
    Index_Check(&index, tarray->Length);

    tarray->It_Return_Value->Data = tarray->Data + index;
    return tarray->It_Return_Value;
}

void TArray_Set(TIterator* it, int64_t index, TGeneric* value)
{
    TArray* tarray = (TArray*)it->Container.Data;
    Index_Check(&index, tarray->Length);

    tarray->Data[index] = value->Data;
    //memcpy((tarray->Data + index), value->Data, sizeof(Byte*));
}