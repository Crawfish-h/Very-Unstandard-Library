#pragma once
#include <stddef.h>
#include <stdbool.h>
#include "Containers/TContainer.h"

typedef struct TObject TObject;
typedef struct TGeneric TGeneric;

typedef struct TString
{
    TContainer Super;
    char* Str;
} TString;

// Creates a TString from a null terminated string.
#define NT_TString(n_String) \
    &(TString){ .Str = n_String, .Super.Size = (sizeof(n_String) / sizeof(n_String[0])) - 1, .Super.Capacity = ((sizeof(n_String) / sizeof(n_String[0])) - 1) * 2 }

TString* TString_Dyn(char* string);
void TString_Insert0(TString* string, char* other_String, ssize_t index);
void TString_Insert1(TString* string, TString* other_String, ssize_t index);
void TString_Free(TString* string);

bool TString_Equal(TString* str_0, TString* str_1);
bool TString_Equal_N(TString* str_0, TString* str_1, size_t n);

// Returns true if [string.Str] contains [sub.Str].
TString TString_Str_Str(TString* string, TString* sub_String); 


TGeneric TString_Equal_Overload(int arg_Count, ...);
