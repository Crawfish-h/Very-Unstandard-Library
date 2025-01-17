#include "Utility.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "TGeneric.h"
#include "TObject.h"

void* Valloc(TObject* object, size_t size)
{
    printf("metadata: %s\n", object->Metadata);
}

TRtti Type_Str_Check_Large_(void* check, TString type_String,
    Generic_Function ctor,
    Generic_Function dtor,
    Generic_Function move,
    Generic_Function copy,
    Generic_Function set,
    Generic_Function is_Equal,
    Generic_Function less,
    Generic_Function less_Equal,
    Generic_Function greater,
    Generic_Function greater_Equal,
    Generic_Function add,
    Generic_Function subtract,
    Generic_Function multiply,
    Generic_Function divide,
    Generic_Function to_String
)
{
    return (TRtti){ .Type = type_String };
}

TRtti TRtti_Init_(void* check, size_t size_Of_Type, TString type_String)
{
    return (TRtti){ .Type = type_String, .Size_Of = size_Of_Type };
}

void Type_Check(TString type_String, Array_Of(TRtti) types, size_t types_Count)
{
    if (types_Count > 0)
    {
        for (size_t i = 0; i < types_Count; i++)
        {
            // WIP: change this to a TString based function.
            if (strcmp(type_String.Str, types[i].Type.Str) == 0)
            {
                return;
            } 
        }
    }

    fprintf(stderr, "ERROR: type mismatch. Invalid added type: %s. Valid types: %s", type_String.Str, types[0].Type.Str);
    for (size_t i = 1; i < types_Count; i++) fprintf(stderr, ", %s", types[i].Type.Str);
    fprintf(stderr, ".\n");
    exit(EXIT_FAILURE);
}