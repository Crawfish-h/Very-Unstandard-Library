#include "TString.h"
#include "TGeneric.h"
#include <stdarg.h>

bool TString_Equal(TString str_0, TString str_1)
{
    if (str_0.Size != str_1.Size) return false;
    
    for (size_t i = 0; i < str_0.Size; i++)
    {
        if (str_0.Str[i] != str_1.Str[i]) return false;
    }

    return true;
}

// Correct varargs and order: [TString], [TString].
// Returns [TGeneric.Type = bool];
TGeneric TString_Equal_Overload(int arg_Count, ...)
{
    va_list varargs;
    va_start(varargs, arg_Count);
    bool result = TString_Equal(va_arg(varargs, TString), va_arg(varargs, TString));
    return (TGeneric)
    { 
        .Data = &result,
        .Rtti_ = Rtti(bool),
        .Is_Allocated = false,
        .Super = NULL
    };
}