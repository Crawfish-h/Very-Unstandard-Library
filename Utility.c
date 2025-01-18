#include "Utility.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "TGeneric.h"
#include "Containers/TVector.h"
#include "Reflection.h"

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

void* Valloc(size_t size)
{
    TVector latest_Pool = *Cast(TVector*, Memory_Pool.Elements_[Memory_Pool.Size_ - 1].Data);
    TVector_Push(&latest_Pool, TGeneric_Init(malloc(size), Rtti(void*), NULL));
    return latest_Pool.Elements_[latest_Pool.Size_ - 1].Data;
}