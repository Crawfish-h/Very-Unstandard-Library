#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct TObject TObject;
typedef struct TGeneric TGeneric;

typedef struct TString
{
    TObject* Super;
    char* Str;
    size_t Size;
} TString;

// Creates a TString from a null terminated string.
#define NT_TString(n_String) \
    (TString){ .Str = n_String, .Size = (sizeof(n_String) / sizeof(n_String[0])) - 1 }

bool TString_Equal(TString str_0, TString str_1);
TGeneric TString_Equal_Overload(int arg_Count, ...);

