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

// Creates a dynamic string allocated on the heap that can be used with the TString_Insert functions.
TString* TString_Dyn(char* string);

// Inserts a other_String into string.
void TString_Insert0(TString* string, char* other_String, int64_t index);

// Inserts a other_String into string.
void TString_Insert1(TString* string, TString* other_String, int64_t index);

// Frees a TString.
void TString_Free(TString* string);

bool TString_Equal(TString* str_0, TString* str_1);
bool TString_Equal_N(TString* str_0, TString* str_1, uint32_t n);

// Returns true if [string.Str] contains [sub.Str].
TString TString_Str_Str(TString* string, TString* sub_String); 
