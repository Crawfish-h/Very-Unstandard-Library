#pragma once
#include "TString.h"
#include <stdbool.h>
#include "Utility.h"

typedef struct TVector TVector;

typedef struct TRtti
{
    TString Type;
    uint32_t Size_Of;
} TRtti;

// Returns runtime type information. 
// [check] exists so that if a nonexistant variable is passed to it, it will generate an error.
TRtti TRtti_Init_(void* check, uint32_t size_Of_Type, TString* type_String);

// Returns runtime type information. 
// First argument is the type, second argument is optional and also a bool
// that, if supplied and true, runs a function that returns a large structure.
#define Rtti(type) &(TRtti){ .Type = *NT_TString(Stringify(type)), .Size_Of = sizeof(type) }

void Type_Check(TString* type_String, Array_Of(TRtti) types, uint32_t types_Count);
bool Compare_Types(TRtti* type_Info_0, TRtti* type_Info_1);

// Checks if a type or [TRtti] object is a pointer. Does not work with typedefed pointers (e.g., typedef int* int_Pointer).
bool Is_Pointer(TRtti* rtti);