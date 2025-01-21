#include "TString.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "Reflection.h"
#include "TGeneric.h"

bool TString_Equal(TString* str_0, TString* str_1)
{
    if (str_0->Size != str_1->Size) return false;
    
    for (size_t i = 0; i < str_0->Size; i++)
    {
        if (str_0->Str[i] != str_1->Str[i]) return false;
    }

    return true;
}

bool TString_Equal_N(TString* str_0, TString* str_1, size_t n)
{    
    for (size_t i = 0; i < n; i++)
    {
        if (str_0->Str[i] != str_1->Str[i]) return false;
    }

    return true;
}

TString TString_Str_Str(TString* string, TString* sub_String)
{
    const char *p = string->Str;
    if (sub_String->Size == 0) return *string;
    
    for (; (p = strchr(p, *sub_String->Str)) != 0; p++)
    {
        if (strncmp(p, sub_String->Str, sub_String->Size) == 0)
            return (TString){ .Size = sub_String->Size, .Str = p, .Super = string->Super };
    }

    return (TString){ 0 };
}

// Correct varargs and order: [TString*], [TString*].
// Returns [TGeneric.Type = bool];
TGeneric TString_Equal_Overload(int arg_Count, ...)
{
    va_list varargs;
    va_start(varargs, arg_Count);
    bool result = TString_Equal(va_arg(varargs, TString*), va_arg(varargs, TString*));
    return (TGeneric)
    { 
        .Data = &result,
        .Rtti_ = Rtti(bool),
        .Is_Allocated = false,
        .Super = NULL
    };
}