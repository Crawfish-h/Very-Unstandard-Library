#include "TGeneric.h"

TGeneric TGeneric_Init(void* data,  TRtti rtti, TObject* super)
{
    TGeneric generic = { .Data = data, .Rtti_ = rtti, .Super = super };
    return generic;
}