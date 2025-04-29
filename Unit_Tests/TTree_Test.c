#include "TTree_Test.h"
#include "../Containers/TTree.h"
#include "../Reflection.h"
#include "../TString.h"
#include "../Utility.h"
#include <assert.h>
#include <string.h>

void* Tree_Stack_Test(TTree* tree, ssize_t index)
{
    return TTree_Get(tree, index);
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

    char* msg0 = "Actual node value does not equal expected value.\n";
    assert(strcmp((char*)TTree_Get(tree, 0), "A string") == 0 && msg0);
    assert(*(int*)TTree_Get(tree, 200) == 200 && msg0);
    assert(TString_Equal((TString*)TTree_Get(tree, -6000), NT_TString("www.google.com")) && msg0);
    assert(*(int*)TTree_Get(tree, 100) == 1066 && msg0);
    assert(strcmp((char*)TTree_Get(tree, 33), "www3") == 0 && msg0);
    assert(TString_Equal((TString*)TTree_Get(tree, 1), NT_TString("A23T")) && msg0);

    assert(strcmp((char*)Tree_Stack_Test(tree, 0), "A string") == 0 && msg0);
    assert(*(int*)Tree_Stack_Test(tree, 200) == 200 && msg0);
    assert(TString_Equal((TString*)Tree_Stack_Test(tree, -6000), NT_TString("www.google.com")) && msg0);
    assert(*(int*)Tree_Stack_Test(tree, 100) == 1066 && msg0);
    assert(strcmp((char*)Tree_Stack_Test(tree, 33), "www3") == 0 && msg0);
    assert(TString_Equal((TString*)Tree_Stack_Test(tree, 1), NT_TString("A23T")) && msg0);

    printf("TTree_Test.c: success!\n");
}