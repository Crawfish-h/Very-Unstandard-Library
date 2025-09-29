# Very-Unstandard-Library
A wip library programmed in C that implements very basic RTTI, pool memory allocation, basic type aware generics, a custom string type with custom functions, various containers, and more.

## TString
TString is string datatype that stores its size as a number instead of using a null byte.
```C
    #include "TString.h"

    // Create TString pointer from the address of a TString created on the stack.
    TString* string_0 = NT_TString("Text");
    
    // Insert string_1 inside of string_0 at index 2.
    // string_0->Str will point to a heap allocated string after
    // TString_Insert1 or 0 is called.
    TString* string_1 = NT_TString("More Text");

    // string_0->Str now points to "TexMore Textt".
    TString_Insert1(string_0, string_1, 2);
```

## Rtti
This data type is used to store type information.
```C
    #include "../Reflection.h"

    TRtti* rtti = Rtti(TString);

    // Will print a value equal to sizeof(TString).
    printf("%zu\n", rtti->Size_Of); 

    // Will print TString.
    printf("%d\n", rtti->Type.Str);
```

## TGeneric
TGeneric datatype. Used to hold data and type information in a generic way.
```C
    #include "TGeneric.h"
    #include "Utility.h" // For LV.

    // LV converts Rvalues to Lvalues.
    // The TG macro only works with Lvalues
    TGeneric* tgen = TG(int, LV(100));

    // Will print 100.
    printf("%d\n", *(int*)tgen->Data); 
```

## TTree
An n-ary tree datatype.
```C
    #include "/Containers/TTree.h"

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
```

## TVector
A dynamic array-like container.
```C
    
```