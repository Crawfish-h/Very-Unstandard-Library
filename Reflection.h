#pragma once
#include "TString.h"
#include "Utility.h"

typedef struct TRtti
{
    TString Type;
    size_t Size_Of;
} TRtti;

// Returns runtime type information. 
// [check] exists so that if a nonexistant variable is passed to it, it will generate an error.
TRtti TRtti_Init_(void* check, size_t size_Of_Type, TString type_String);

// Returns runtime type information. 
// First argument is the type, second argument is optional and also a bool
// that, if supplied and true, runs a function that returns a large structure.
#define Rtti(type) TRtti_Init_((type*)NULL, sizeof(type), NT_TString(Stringify(type)))

void Type_Check(TString type_String, Array_Of(TRtti) types, size_t types_Count);