#include "TMap.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../Reflection.h"
#include "../TPair.h"
#include "TContainer.h"
#include "TVector.h"

typedef struct TMap
{
    TContainer* Container;
    void (*Add)(TContainer* container, ssize_t index, TGeneric* value);
    TGeneric* (*Get)(TContainer* container, ssize_t index);
} TMap;

void TMap_Define_Add(void* map_Arg, ssize_t index, TGeneric* value)
{
    TMap* map = map_Arg;
    TString* key = TString_Dyn("index ");

    char index_Str[2];
    sprintf(index_Str, "%lld", index);
    TString_Insert0(key, index_Str, -1);

    map->Container->Add
    (
        map->Container, index, 
        TG(TPair, &((TPair){ .First = *TG(TString*, key), .Second = *value }))
    );
}

TGeneric* TMap_Define_Get(void* map_Arg, ssize_t index)
{
    TMap* map = map_Arg;
    return map->Container->Get( map->Container, index);
}

TMap* TMap_Init(TContainer* container, size_t type_Count, size_t value_Count, ...)
{
    TMap* map = calloc(1, sizeof(TMap));
    va_list arg_List;
    value_Count += type_Count;
    va_start(arg_List, value_Count);
    value_Count -= type_Count;
    map->Container = container;
    map->Add = TMap_Define_Add;
    map->Get = TMap_Define_Get;
    
    for (size_t i = 0; i < type_Count; i++)
    {
        TRtti rtti = va_arg(arg_List, TRtti);
        TContainer_Add_Type(container, &rtti);
    }

    TRtti rtti = Rtti(TPair);
    TContainer_Add_Type(container, &rtti);

    for (size_t i = 0; i < value_Count; i++)
    {
        TPair* pair = va_arg(arg_List, TPair*);
        //TMap_Add(map, (TString*)pair->First.Data, &pair->Second);
        TMap_Multi(map, 1, pair);
    }

    va_end(arg_List);
    return map;
}

bool TMap_Add(TMap* map, TPair* key_Value_Pair)
{
    TMap_Multi(map, 1, key_Value_Pair);
}

bool TMap_Multi(TMap* map, size_t value_Count, ...)
{
    va_list arg_List;
    va_start(arg_List, value_Count);
    TContainer* cont = map->Container;

    for (size_t i = 0; i < value_Count; i++)
    {
        TPair* pair = va_arg(arg_List, TPair*);
        for (size_t j = 0; j < map->Container->Size; j++)
        {
            TPair* pair_Cmp = map->Container->Get(map->Container, j)->Data;
            if (TString_Equal(pair_Cmp->First.Data, pair->First.Data) == true)
            {
                fprintf(stderr, "ERROR tried adding a key that was already inside the TMap.");
            }
        }

        Type_Check(&pair->Second.Rtti_.Type, cont->Types, cont->Type_Count);
        TString* key = pair->First.Data;
        char* temp_Key_Str = key->Str;
        key->Str = cont->Allocator.Calloc(1, key->Super.Size + 1);
        memcpy(key->Str, temp_Key_Str, key->Super.Size);
        TGeneric key_Gen = pair->First;
        TGeneric value_Gen = pair->Second;
        TContainer_Add_If_Pointer(cont, &key_Gen, &pair->First);
        TContainer_Add_If_Pointer(cont, &value_Gen, &pair->Second);
        pair->First = key_Gen;
        pair->Second = value_Gen;
        key = pair->First.Data;
        //printf("value type: %s.\n", pair->Second.Rtti_.Type.Str);
        size_t index = 0;
        if (cont->Size > 0) index = cont->Size - 1;
        cont->Add(cont, index, TG(TPair, pair));
    }

    va_end(arg_List);
}

TPair* TMap_Get_Info(TMap* map, TString* key)
{
    for (size_t i = 0; i < map->Container->Size; i++)
    {
        TPair* pair = map->Container->Get(map->Container, i)->Data;
        if (TString_Equal(pair->First.Data, key) == true)
        {
            return pair;
        }
    }

    fprintf(stderr, "ERROR: could not find key '%s' in [TMap].\n", key->Str);
    exit(EXIT_FAILURE);
    return NULL;
}

void* TMap_Get(TMap* map, TString* key)
{
    return TMap_Get_Info(map, key)->Second.Data;
}