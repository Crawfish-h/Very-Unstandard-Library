#pragma once
#include "Utility.h"

typedef struct TManip_Fns
{
    Generic_Function Ctor;
    void (*Dtor)(TGeneric* arg);
    void (*Move)(TGeneric* arg_0, TGeneric* arg_1);
    void (*Copy)(TGeneric* arg_0, TGeneric* arg_1);
    void (*Set_Equal)(TGeneric* arg_0, TGeneric* arg_1);
    void (*Is_Equal)(TGeneric* arg_0, TGeneric* arg_1);
    void (*Less)(TGeneric* arg_0, TGeneric* arg_1);
    void (*Less_Equal)(TGeneric* arg_0, TGeneric* arg_1);
    void (*Greater)(TGeneric* arg_0, TGeneric* arg_1);
    void (*Greater_Equal)(TGeneric* arg_0, TGeneric* arg_1);
    TGeneric (*Add)(TGeneric* arg_0, TGeneric* arg_1);
    TGeneric (*Subtract)(TGeneric* arg_0, TGeneric* arg_1);
    TGeneric (*Multiply)(TGeneric* arg_0, TGeneric* arg_1);
    TGeneric (*Divide)(TGeneric* arg_0, TGeneric* arg_1);
    TString (*To_String)(TGeneric* arg);
} TManip_Fns;

void Defualt_Dtor(TGeneric* arg);

void Defualt_Move(TGeneric* move_To, TGeneric* move_From);

// Use this when [copy_To] has not been initialized.
// [copy_To->Data] will be allocated if [copy_From->Is_Allocated] is true.
void Defualt_Copy(TGeneric* copy_To, TGeneric* copy_From);

// Use this when [old_Value] has been initialized.
// [copy_To->Data] will be allocated if [copy_From->Is_Allocated] is true.
void Defualt_Set_Equal(TGeneric* old_Value, TGeneric* new_Value);

TString Defualt_To_String(TGeneric* arg);

typedef struct TObject
{
    TGeneric* Metadata;
    TGeneric* Memory;
    TManip_Fns;
} TObject;