#include "Unit_Tests.h"
#include "../Containers/TTree.h"
#include "../Containers/TVector.h"
#include "../Containers/TLinked_List.h"
#include "../Containers/TDoubly_Linked_List.h"
#include "../Containers/TMap.h"
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

void Container_Test(TContainer* container, char* success_Message)
{
    container->Add(container, 0, TG(char*, "A new string"));
    container->Add(container, 1, TG(float, LV(400.5f)));
    container->Add(container, 2, TG(TString, NT_TString("www.bing.com")));
    container->Add(container, 3, TG(float, LV(90007.7f)));
    container->Add(container, 4, TG(TString, NT_TString("abcdefg")));
    container->Add(container, 3, TG(char*, "www4"));

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
            container->Get_Info(container, 0)->Rtti_,
            container->Get_Info(container, 1)->Rtti_,
            container->Get_Info(container, 2)->Rtti_,
            container->Get_Info(container, 3)->Rtti_,
            container->Get_Info(container, 4)->Rtti_,
            container->Get_Info(container, 5)->Rtti_,
        },
        6
    );

    assert(strcmp((char*)container->Get(container, 0), "A new string") == 0);
    assert(*(float*)container->Get(container, 1) == 400.5f);
    assert(TString_Equal((TString*)container->Get(container, 2), NT_TString("www.bing.com")));
    assert(*(float*)container->Get(container, 4) == 90007.7f);
    assert(strcmp((char*)container->Get(container, 3), "www4") == 0);
    assert(TString_Equal((TString*)container->Get(container, 5), NT_TString("abcdefg")));
    assert(container->Size == 6);

    printf("%s\n", success_Message);
}