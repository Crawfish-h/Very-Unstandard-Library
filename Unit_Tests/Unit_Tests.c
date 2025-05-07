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

void Container_Type_Test(Array_Of(TRtti) expected_Types, Array_Of(TRtti) actual_Types, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        assert(Compare_Types(&expected_Types[i], &actual_Types[i]));
    }
}

void Container_Test(TIterator* it, char* success_Message)
{
    it->Add(it, 0, TG(char*, "A new string"));
    it->Add(it, 1, TG(float, LV(400.5f)));
    it->Add(it, 2, TG(TString, NT_TString("www.bing.com")));
    it->Add(it, 3, TG(float, LV(90007.7f)));
    it->Add(it, 4, TG(TString, NT_TString("abcdefg")));
    it->Add(it, 3, TG(char*, "www4"));

    Container_Type_Test
    (
        (TRtti[])
        {
            Rtti(char*),
            Rtti(float),
            Rtti(TString),
            Rtti(char*),
            Rtti(float),
            Rtti(TString)
        },
        (TRtti[])
        {
            it->Get_Info(it, 0)->Rtti_,
            it->Get_Info(it, 1)->Rtti_,
            it->Get_Info(it, 2)->Rtti_,
            it->Get_Info(it, 3)->Rtti_,
            it->Get_Info(it, 4)->Rtti_,
            it->Get_Info(it, 5)->Rtti_,
        },
        6
    );

    assert(strcmp((char*)it->Get(it, 0), "A new string") == 0);
    assert(*(float*)it->Get(it, 1) == 400.5f);
    assert(TString_Equal((TString*)it->Get(it, 2), NT_TString("www.bing.com")));
    assert(*(float*)it->Get(it, 4) == 90007.7f);
    assert(strcmp((char*)it->Get(it, 3), "www4") == 0);
    assert(TString_Equal((TString*)it->Get(it, 5), NT_TString("abcdefg")));
    assert(*it->Size == 6);

    printf("%s\n", success_Message);
}