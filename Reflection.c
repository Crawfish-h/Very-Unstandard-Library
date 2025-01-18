#include "Reflection.h"

TRtti TRtti_Init_(void* check, size_t size_Of_Type, TString type_String)
{
    return (TRtti){ .Type = type_String, .Size_Of = size_Of_Type };
}