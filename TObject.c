#include "TObject.h"
#include <stdlib.h>
#include <stdio.h>
#include "TGeneric.h"
#include "TString.h"

void Defualt_Dtor(TGeneric* arg)
{
    if (arg->Is_Allocated == true)
    {
        arg->Is_Allocated = false;
        free(arg->Data);
    }
}

void Defualt_Move(TGeneric* move_To, TGeneric* move_From)
{
    if (TString_Equal(&move_To->Rtti_.Type, &move_From->Rtti_.Type) == false)
    {
        fprintf(stderr, "ERROR: in [Defualt_Copy], [arg_0->Type] does not equal [arg_1->Type].");
        exit(EXIT_FAILURE);
    }

    if (move_From->Is_Allocated == false)
    {
        fprintf(stderr, "ERROR: in [Defualt_Move], [arg_1] is not allocated.");
        exit(EXIT_FAILURE);
    }

    if (move_To->Is_Allocated == true) free(move_To->Data);

    move_To->Data = move_From->Data;
    move_To->Data = NULL;
}

// Use this when [copy_To] has not been initialized.
// [copy_To->Data] will be allocated if [copy_From->Is_Allocated] is true.
void Defualt_Copy(TGeneric* copy_To, TGeneric* copy_From)
{
    if (TString_Equal(&copy_To->Rtti_.Type, &copy_From->Rtti_.Type) == false)
    {
        fprintf(stderr, "ERROR: in [Defualt_Copy], [arg_0->Type] does not equal [arg_1->Type].");
        exit(EXIT_FAILURE);
    }

    if (copy_From->Is_Allocated == true) 
    {
        // WIP: copy_To->Data = malloc(Rtti(copy_From->Type).Sizeof);
        // WIP: memcpy(copy_To->Data, copy_From->Data, Rtti(copy_From->Type).Sizeof);
    }else
    {
        copy_To->Data = copy_From->Data;
    }
}

// Use this when [old_Value] has been initialized.
// [copy_To->Data] will be allocated if [copy_From->Is_Allocated] is true.
void Defualt_Set_Equal(TGeneric* old_Value, TGeneric* new_Value)
{
    if (TString_Equal(&old_Value->Rtti_.Type, &new_Value->Rtti_.Type) == false)
    {
        fprintf(stderr, "ERROR: in [Defualt_Copy], [arg_0->Type] does not equal [arg_1->Type].");
        exit(EXIT_FAILURE);
    }

    if (old_Value->Is_Allocated == true) 
    {
        free(old_Value->Data);
        old_Value->Is_Allocated = false;
    }

    if (new_Value->Is_Allocated == true) 
    {
        // WIP: old_Value->Data = malloc(Rtti(new_Value->Type).Sizeof);
        // WIP: memcpy(old_Value->Data, new_Value->Data, Rtti(new_Value->Type).Sizeof);
    }else
    {
        old_Value->Data = new_Value->Data;
    }
}

TString Defualt_To_String(TGeneric* arg)
{
    return arg->Rtti_.Type;
}