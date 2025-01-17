#include "TTuple.h"
#include <stdarg.h>
#include "TArray.h"


void TTuple_Ctor(TTuple* tuple, const size_t size, ...)
{
    va_list va_Args;
    va_start(va_Args, size);
    tuple->Data.Rtti_ = Rtti(TGeneric);
    tuple->Data.Size = 0;
    //void* void_Arr[size];
    //tuple->Data = ;
    for (int i = 0; i < size; i++)
    {
        
    }
}