#include "../Containers/TTree.h"
#include "../Containers/TVector.h"
#include "../Containers/TLinked_List.h"
#include "../Containers/TDoubly_Linked_List.h"
#include "../Containers/TMap.h"
#include "../Containers/TIterator.h"
#include "../Reflection.h"
#include "../TString.h"
#include "../Utility.h"
#include "../TVariant.h"


void TString_Example()
{

}

// TGeneric datatype. Used to hold data and type information in a generic way.
void TGeneric_Example()
{
    TRtti* rtti = Rtti(TString);
    printf("%d\n", rtti->Type.Str);

    TGeneric* tgen = TG(int, LV(100));

    printf("%d\n", *(int*)tgen->Data);
}

// Tree container. https://en.wikipedia.org/wiki/Tree_(abstract_data_type)#Representations
void TTree_Example()
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

    TTree_Add(tree, TTree_Arg(0, 1, TString, NT_TString("A23T")));

    TTree_Add(tree, TTree_Arg(100, 33, char*, "www3"));
    TTree_Multi
    (
        tree, 2, 
        TTree_Arg(200, 100, int, LV(12)),
        TTree_Arg(100, 33, char*, "Char Value")
    );
    
    printf("tree 0: %s\n", (char*)TTree_Get(tree, 0));
    printf("tree 1: %d\n", *(int*)TTree_Get(tree, 200));
    printf("tree 2: %s\n", ((TString*)TTree_Get(tree, -6000))->Str);
    printf("tree 3: %d\n", *(int*)TTree_Get(tree, 200));
    printf("tree 4: %s\n", (char*)TTree_Get(tree, 33));
    printf("tree 5: %s\n", ((TString*)TTree_Get(tree, 1))->Str);

    TTree_Free(tree);
}

