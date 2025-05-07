#include "Reflection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TString.h"

TRtti TRtti_Init_(void* check, size_t size_Of_Type, TString* type_String)
{
    return (TRtti){ .Type = *type_String, .Size_Of = size_Of_Type };
}

void Type_Check(TString* type_String, Array_Of(TRtti) types, size_t types_Count)
{
    if (types_Count > 0)
    {
        for (size_t i = 0; i < types_Count; i++)
        {
            // WIP: change this to a TString based function.
            if (strcmp(type_String->Str, types[i].Type.Str) == 0)
            {
                return;
            } 
        }
    }else
    {
        return;
    }

    fprintf(stderr, "ERROR: type mismatch. Invalid added type: %s. Valid types: %s", type_String->Str, types[0].Type.Str);
    for (size_t i = 1; i < types_Count; i++) fprintf(stderr, ", %s", types[i].Type.Str);
    fprintf(stderr, ".\n");
    exit(EXIT_FAILURE);
}

bool Compare_Types(TRtti* type_Info_0, TRtti* type_Info_1)
{
    return TString_Equal(&type_Info_0->Type, &type_Info_1->Type) == true && type_Info_0->Size_Of == type_Info_1->Size_Of;
}

bool Is_Pointer(TRtti* rtti)
{
    if (TString_Str_Str(&rtti->Type, NT_TString("*")).Str != NULL)
    {
        return true;
    }

    return false;
}