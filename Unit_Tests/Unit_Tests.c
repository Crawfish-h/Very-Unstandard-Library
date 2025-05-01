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

void* TTree_Get_Stack_Test(TTree* tree, ssize_t index)
{
    return TTree_Get(tree, index);
}

void* TVector_Get_Stack_Test(TVector* vector, size_t index)
{
    return TVector_Get(vector, index);
}

void* TLinked_List_Get_Stack_Test(TLinked_List* list, ssize_t index)
{
    return TLinked_List_Get(list, index);
}

void* TDoubly_Linked_List_Get_Stack_Test(TDoubly_Linked_List* list, ssize_t index)
{
    return TDoubly_Linked_List_Get(list, index);
}

void* TMap_Get_Stack_Test(TMap* map, TString* key)
{
    return TMap_Get(map, key);
}

/*#define Make_Container_Test(add_0, add_1, add_2, add_3, add_4, add_5, init_Args, init_Fn, container_Type, multi_Fn, add_Fn, get_0, get_1, get_2, get_3, get_4, get_5) \
    container_Type* container = init_Fn \
    ( \
        init_Args \
        3, 3, \
        Rtti(float), \
        Rtti(TString), \
        Rtti(char*), \
        add_0, \
        add_1, \
        add_2, \
    ); \
     \
    multi_Fn \
    ( \
        container, 2, \
        add_3, \
        add_4 \
    ); \
     \
    add_Fn(container, add_5); \
    char* msg0 = "Actual node value does not equal expected value.\n"; \
    assert(strcmp((char*)get_0, "A string") == 0); \
    assert(*(int*)get_1 == 200); \
    assert(TString_Equal((TString*)get_2, NT_TString("www.google.com"))); \
    assert(*(int*)get_3 == 1066); \
    assert(strcmp((char*)get_4, "www3") == 0); \
    assert(TString_Equal((TString*)get_5, NT_TString("A23T"))); \
     \
    assert(strcmp((char*)get_0 ## Stack_Test, "A string") == 0); \
    assert(*(int*)get_1 ## Stack_Test == 200); \
    assert(TString_Equal((TString*)get_2 ## Stack_Test, NT_TString("www.google.com"))); \
    assert(*(int*)get_3 ## Stack_Test == 1066); \
    assert(strcmp((char*)get_4 ## Stack_Test, "www3") == 0); \
    assert(TString_Equal((TString*)get_5 ## Stack_Test, NT_TString("A23T")));*/

void Container_Type_Test(Array_Of(TRtti) expected_Types, Array_Of(TRtti) actual_Types, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        assert(Compare_Types(&expected_Types[i], &actual_Types[i]));
    }
}

void TVector_Test()
{
    TVector* vector = TVector_Init
    (
        3, 3,
        Rtti(float),
        Rtti(TString),
        Rtti(char*),
        TG(char*, "A new string"),
        TG(float, LV(400.5f)),
        TG(TString, NT_TString("www.bing.com"))
    );

    TVector_Multi
    (
        vector, 2,
        TG(float, LV(90007.7f)),
        TG(TString, NT_TString("abcdefg"))
    );

    TVector_Push(vector, TG(char*, "www4"));

    Container_Type_Test
    (
        (TRtti[])
        {
            Rtti(char*),
            Rtti(float),
            Rtti(TString),
            Rtti(float),
            Rtti(TString),
            Rtti(char*)
        },
        (TRtti[])
        {
            TVector_Get_Info(vector, 0)->Rtti_,
            TVector_Get_Info(vector, 1)->Rtti_,
            TVector_Get_Info(vector, 2)->Rtti_,
            TVector_Get_Info(vector, 3)->Rtti_,
            TVector_Get_Info(vector, 4)->Rtti_,
            TVector_Get_Info(vector, 5)->Rtti_,
        },
        6
    );

    assert(strcmp((char*)TVector_Get(vector, 0), "A new string") == 0);
    assert(*(float*)TVector_Get(vector, 1) == 400.5f);
    assert(TString_Equal((TString*)TVector_Get(vector, 2), NT_TString("www.bing.com")));
    assert(*(float*)TVector_Get(vector, 3) == 90007.7f);
    assert(TString_Equal((TString*)TVector_Get(vector, 4), NT_TString("abcdefg")));
    assert(strcmp((char*)TVector_Get(vector, 5), "www4") == 0);

    assert(strcmp((char*)TVector_Get_Stack_Test(vector, 0), "A new string") == 0);
    assert(*(float*)TVector_Get_Stack_Test(vector, 1) == 400.5f);
    assert(TString_Equal((TString*)TVector_Get_Stack_Test(vector, 2), NT_TString("www.bing.com")));
    assert(*(float*)TVector_Get_Stack_Test(vector, 3) == 90007.7f);
    assert(TString_Equal((TString*)TVector_Get_Stack_Test(vector, 4), NT_TString("abcdefg")));
    assert(strcmp((char*)TVector_Get_Stack_Test(vector, 5), "www4") == 0);

    TVector_Free(vector);
    printf("TVector_Test(): success!\n");
}

void TTree_Test()
{
    TTree* tree = TTree_Init
    (
        3, 3,
        Rtti(int),
        Rtti(TString),
        Rtti(char*),
        TTree_Arg(0, 0, char*, "A string"),
        TTree_Arg(0, 200, int, LV(200)),
        TTree_Arg(200, -6000, TString, NT_TString("www.google.com"))
    );

    TTree_Multi
    (
        tree, 2, 
        TTree_Arg(200, 100, int, LV(1066)),
        TTree_Arg(100, 33, char*, "www3")
    );

    TTree_Add(tree, TTree_Arg(0, 1, TString, NT_TString("A23T")));

    Container_Type_Test
    (
        (TRtti[])
        {
            Rtti(char*),
            Rtti(int),
            Rtti(TString),
            Rtti(int),
            Rtti(char*),
            Rtti(TString)
        },
        (TRtti[])
        {
            TTree_Get_Info(tree, 0)->Rtti_,
            TTree_Get_Info(tree, 200)->Rtti_,
            TTree_Get_Info(tree, -6000)->Rtti_,
            TTree_Get_Info(tree, 100)->Rtti_,
            TTree_Get_Info(tree, 33)->Rtti_,
            TTree_Get_Info(tree, 1)->Rtti_
        },
        6
    );

    assert(strcmp((char*)TTree_Get(tree, 0), "A string") == 0);
    assert(*(int*)TTree_Get(tree, 200) == 200);
    assert(TString_Equal((TString*)TTree_Get(tree, -6000), NT_TString("www.google.com")));
    assert(*(int*)TTree_Get(tree, 100) == 1066);
    assert(strcmp((char*)TTree_Get(tree, 33), "www3") == 0);
    assert(TString_Equal((TString*)TTree_Get(tree, 1), NT_TString("A23T")));

    assert(strcmp((char*)TTree_Get_Stack_Test(tree, 0), "A string") == 0);
    assert(*(int*)TTree_Get_Stack_Test(tree, 200) == 200);
    assert(TString_Equal((TString*)TTree_Get_Stack_Test(tree, -6000), NT_TString("www.google.com")));
    assert(*(int*)TTree_Get_Stack_Test(tree, 100) == 1066);
    assert(strcmp((char*)TTree_Get_Stack_Test(tree, 33), "www3") == 0);
    assert(TString_Equal((TString*)TTree_Get_Stack_Test(tree, 1), NT_TString("A23T")));

    TTree_Free(tree);
    printf("TTree_Test(): success!\n");
}

void TLinked_List_Test()
{
    TLinked_List* list = TLinked_List_Init
    (
        3, 3,
        Rtti(float),
        Rtti(TString),
        Rtti(char*),
        TG(char*, "A new string"),
        TG(float, LV(400.5f)),
        TG(TString, NT_TString("www.bing.com"))
    );

    TLinked_List_Multi
    (
        list, -1, 2,
        TG(float, LV(90007.7f)),
        TG(TString, NT_TString("abcdefg"))
    );

    TLinked_List_Add(list, 4, TG(char*, "www4"));

    Container_Type_Test
    (
        (TRtti[])
        {
            Rtti(char*),
            Rtti(float),
            Rtti(TString),
            Rtti(float),
            Rtti(char*),
            Rtti(TString)
        },
        (TRtti[])
        {
            TLinked_List_Get_Info(list, 0)->Rtti_,
            TLinked_List_Get_Info(list, 1)->Rtti_,
            TLinked_List_Get_Info(list, 2)->Rtti_,
            TLinked_List_Get_Info(list, 3)->Rtti_,
            TLinked_List_Get_Info(list, 4)->Rtti_,
            TLinked_List_Get_Info(list, 5)->Rtti_
        },
        6
    );

    assert(strcmp((char*)TLinked_List_Get(list, 0), "A new string") == 0);
    assert(*(float*)TLinked_List_Get(list, 1) == 400.5f);
    assert(TString_Equal((TString*)TLinked_List_Get(list, 2), NT_TString("www.bing.com")));
    assert(*(float*)TLinked_List_Get(list, 3) == 90007.7f);
    assert(strcmp((char*)TLinked_List_Get(list, 4), "www4") == 0);
    assert(TString_Equal((TString*)TLinked_List_Get(list, 5), NT_TString("abcdefg")));

    assert(strcmp((char*)TLinked_List_Get_Stack_Test(list, 0), "A new string") == 0);
    assert(*(float*)TLinked_List_Get_Stack_Test(list, 1) == 400.5f);
    assert(TString_Equal((TString*)TLinked_List_Get_Stack_Test(list, 2), NT_TString("www.bing.com")));
    assert(*(float*)TLinked_List_Get_Stack_Test(list, 3) == 90007.7f);
    assert(strcmp((char*)TLinked_List_Get_Stack_Test(list, 4), "www4") == 0);
    assert(TString_Equal((TString*)TLinked_List_Get_Stack_Test(list, 5), NT_TString("abcdefg")));

    TLinked_List_Free(list);
    printf("TLinked_List_Test(): success!\n");
}

void TDoubly_Linked_List_Test()
{
    TDoubly_Linked_List* list = TDoubly_Linked_List_Init
    (
        3, 3,
        Rtti(float),
        Rtti(TString),
        Rtti(char*),
        TG(char*, "A new string"),
        TG(float, LV(400.5f)),
        TG(TString, NT_TString("www.bing.com"))
    );

    TDoubly_Linked_List_Multi
    (
        list, -1, 2,
        TG(float, LV(90007.7f)),
        TG(TString, NT_TString("abcdefg"))
    );

    TDoubly_Linked_List_Add(list, 4, TG(char*, "www4"));

    Container_Type_Test
    (
        (TRtti[])
        {
            Rtti(char*),
            Rtti(float),
            Rtti(TString),
            Rtti(float),
            Rtti(char*),
            Rtti(TString)
        },
        (TRtti[])
        {
            TDoubly_Linked_List_Get_Info(list, 0)->Rtti_,
            TDoubly_Linked_List_Get_Info(list, 1)->Rtti_,
            TDoubly_Linked_List_Get_Info(list, 2)->Rtti_,
            TDoubly_Linked_List_Get_Info(list, 3)->Rtti_,
            TDoubly_Linked_List_Get_Info(list, 4)->Rtti_,
            TDoubly_Linked_List_Get_Info(list, 5)->Rtti_
        },
        6
    );

    assert(strcmp((char*)TDoubly_Linked_List_Get(list, 0), "A new string") == 0);
    assert(*(float*)TDoubly_Linked_List_Get(list, 1) == 400.5f);
    assert(TString_Equal((TString*)TDoubly_Linked_List_Get(list, 2), NT_TString("www.bing.com")));
    assert(*(float*)TDoubly_Linked_List_Get(list, 3) == 90007.7f);
    assert(strcmp((char*)TDoubly_Linked_List_Get(list, 4), "www4") == 0);
    assert(TString_Equal((TString*)TDoubly_Linked_List_Get(list, 5), NT_TString("abcdefg")));

    assert(strcmp((char*)TDoubly_Linked_List_Get_Stack_Test(list, 0), "A new string") == 0);
    assert(*(float*)TDoubly_Linked_List_Get_Stack_Test(list, 1) == 400.5f);
    assert(TString_Equal((TString*)TDoubly_Linked_List_Get_Stack_Test(list, 2), NT_TString("www.bing.com")));
    assert(*(float*)TDoubly_Linked_List_Get_Stack_Test(list, 3) == 90007.7f);
    assert(strcmp((char*)TDoubly_Linked_List_Get_Stack_Test(list, 4), "www4") == 0);
    assert(TString_Equal((TString*)TDoubly_Linked_List_Get_Stack_Test(list, 5), NT_TString("abcdefg")));

    TDoubly_Linked_List_Free(list);
    printf("TDoubly_Linked_List_Test(): success!\n");
}

void TMap_Test(void* container)
{
    TMap* map = TMap_Init
    (
        (TContainer*)container, 
        4, 4,
        Rtti(int),
        Rtti(TString),
        Rtti(char*),
        Rtti(float*),
        TP(TString, NT_TString("First Key"), int, LV(32)),
        TP(TString, NT_TString("Second Key"), int, LV(2000)),
        TP(TString, NT_TString("Second to last"), TString, NT_TString("Value")),
        TP(TString, NT_TString("Final"), char*, "The last value.")
    );

    TMap_Add(map, TP(TString, NT_TString("TMap_Add Key"), float*, malloc(sizeof(float))));
    (*(float*)TMap_Get(map, NT_TString("TMap_Add Key"))) = 233.3f;
    TMap_Get_Info(map, NT_TString("TMap_Add Key"))->Second.Is_Allocated = true;

    TMap_Multi
    (
        map, 3, 
        TP(TString, NT_TString("Multi Key 0"), int, LV(110)),
        TP(TString, NT_TString("Multi Key 1"), char*, "an array of characters"),
        TP(TString, NT_TString("Multi Key 2"), TString, NT_TString("W23"))
    );

    Container_Type_Test
    (
        (TRtti[])
        {
            Rtti(int),
            Rtti(int),
            Rtti(TString),
            Rtti(char*),
            Rtti(float*),
            Rtti(int),
            Rtti(char*),
            Rtti(TString)
        },
        (TRtti[])
        {
            TMap_Get_Info(map, NT_TString("First Key"))->Second.Rtti_,
            TMap_Get_Info(map, NT_TString("Second Key"))->Second.Rtti_,
            TMap_Get_Info(map, NT_TString("Second to last"))->Second.Rtti_,
            TMap_Get_Info(map, NT_TString("Final"))->Second.Rtti_,
            TMap_Get_Info(map, NT_TString("TMap_Add Key"))->Second.Rtti_,
            TMap_Get_Info(map, NT_TString("Multi Key 0"))->Second.Rtti_,
            TMap_Get_Info(map, NT_TString("Multi Key 1"))->Second.Rtti_,
            TMap_Get_Info(map, NT_TString("Multi Key 2"))->Second.Rtti_
        },
        8
    );

    assert(*(int*)TMap_Get(map, NT_TString("First Key")) == 32);
    assert(*(int*)TMap_Get(map, NT_TString("Second Key")) == 2000);
    assert(TString_Equal((TString*)TMap_Get(map, NT_TString("Second to last")), NT_TString("Value")));
    assert(strcmp((char*)TMap_Get(map, NT_TString("Final")), "The last value.") == 0);
    assert((*(float*)TMap_Get(map, NT_TString("TMap_Add Key"))) == 233.3f);
    assert(*(int*)TMap_Get(map, NT_TString("Multi Key 0")) == 110);
    assert(strcmp((char*)TMap_Get(map, NT_TString("Multi Key 1")), "an array of characters") == 0);
    assert(TString_Equal((TString*)TMap_Get(map, NT_TString("Multi Key 2")), NT_TString("W23")));

    assert(*(int*)TMap_Get_Stack_Test(map, NT_TString("First Key")) == 32);
    assert(*(int*)TMap_Get_Stack_Test(map, NT_TString("Second Key")) == 2000);
    assert(TString_Equal((TString*)TMap_Get_Stack_Test(map, NT_TString("Second to last")), NT_TString("Value")));
    assert(strcmp((char*)TMap_Get_Stack_Test(map, NT_TString("Final")), "The last value.") == 0);
    assert((*(float*)TMap_Get_Stack_Test(map, NT_TString("TMap_Add Key"))) == 233.3f);
    assert(*(int*)TMap_Get_Stack_Test(map, NT_TString("Multi Key 0")) == 110);
    assert(strcmp((char*)TMap_Get_Stack_Test(map, NT_TString("Multi Key 1")), "an array of characters") == 0);
    assert(TString_Equal((TString*)TMap_Get_Stack_Test(map, NT_TString("Multi Key 2")), NT_TString("W23")));

    
    printf("TMap_Test(): success!\n");
}

void Container_Test(TContainer* container)
{
    container->Add(container, 0, TG())
}