#include "Unit_Tests.h"
#include "../Containers/TTree.h"
#include "../Containers/TVector.h"
#include "../Containers/TLinked_List.h"
#include "../Containers/TDoubly_Linked_List.h"
#include "../Containers/TMap.h"
#include "../Containers/TIterator.h"
#include "../Reflection.h"
#include "../TString.h"
#include "../Utility.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

void Container_Type_Test(Array_Of(TRtti*) expected_Types, TRtti* actual_Type, size_t count)
{
    bool found_Type = false;
    for (size_t i = 0; i < count; i++)
    {
        if (Compare_Types(expected_Types[i], actual_Type) == true)
        {
            found_Type = true;
        }
    }

    VUL_Assert(found_Type, "Actual type: %s\n", actual_Type->Type.Str);
}

void Test_Container(TIterator* it)
{
    for (size_t i = 0; i < *it->Size; i++)
    {
        TGeneric* tg = it->Get_Info(it, i);
        if (Compare_Types(&tg->Rtti_, Rtti(char*)))
        {
            VUL_Assert(strcmp((char*)it->Get(it, i), "A new string") == 0, "Actual type: %s\n", tg->Rtti_.Type.Str);
            return;
        }

        if (Compare_Types(&tg->Rtti_, Rtti(float)))
        {
            VUL_Assert(*(float*)it->Get(it, i) == 400.5f, "Actual type: %s\n", tg->Rtti_.Type.Str);
            return;
        }

        if (Compare_Types(&tg->Rtti_, Rtti(TString)))
        {
            VUL_Assert(TString_Equal((TString*)it->Get(it, i), NT_TString("www.bing.com")), "Actual type: %s\n", tg->Rtti_.Type.Str);
            return;
        }

        if (Compare_Types(&tg->Rtti_, Rtti(int)))
        {
            VUL_Assert(*(int*)it->Get(it, i) == 90007, "Actual type: %s\n", tg->Rtti_.Type.Str);
            return;
        }

        if (Compare_Types(&tg->Rtti_, Rtti(TRtti)))
        {
            VUL_Assert(Compare_Types((TRtti*)it->Get(it, i), Rtti(int)), "Actual type: %s\n", tg->Rtti_.Type.Str);
            return;
        }

        if (Compare_Types(&tg->Rtti_, Rtti(TVector*)))
        {
            VUL_Assert(TString_Equal((TString*)TVector_Get((TVector*)it->Get(it, i), 0), NT_TString("TString inside a TVector")), "Actual type: %s\n", tg->Rtti_.Type.Str);
            return;
        }
    }
}

void Container_Test(TIterator* it, char* success_Message)
{
    it->Add(it, 0, TG(char*, "A new string"));
    it->Add(it, 1, TG(float, LV(400.5f)));
    it->Add(it, 2, TG(TString, NT_TString("www.bing.com")));
    it->Add(it, 3, TG(int, LV(90007)));
    it->Add(it, 4, TG(TRtti, Rtti(int)));
    it->Add(it, 3, TG(TVector*, TVector_Init(1, 1, Rtti(TString), TG(TString, NT_TString("TString inside a TVector")))));

    for (size_t i = 0; i < 6; i++)
    {
        Container_Type_Test
        (
            (TRtti*[])
            {
                Rtti(char*),
                Rtti(float),
                Rtti(TString),
                Rtti(int),
                Rtti(TRtti),
                Rtti(TVector*)
            },
                &it->Get_Info(it, i)->Rtti_,
            6
        );
    }

    Test_Container(it);
    VUL_Assert(*it->Size == 6, "Actual size: %zu\n", *it->Size);

    printf("%s\n", success_Message);
}