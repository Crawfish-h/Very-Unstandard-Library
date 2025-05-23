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

void TMap_Validate_Size(TMap* map)
{
    if (*map->It.Size < *map->Other_It->Size) *map->Other_It->Size = *map->It.Size;
}

void TMap_Define_Add(TIterator* it, int64_t index, TGeneric* value)
{
    TMap_Validate_Size((TMap*)it->Container.Data);
    TString* key = TString_Dyn("index ");

    char index_Str[2];
    sprintf(index_Str, "%lld", index);
    TString_Insert0(key, index_Str, -1);

    TPair* pair = &(TPair){ .First = *TG(TString*, key) };
    TGeneric_Add_If_Pointer(&pair->Second, value);
    pair->First.Is_Allocated = true;

    ((TMap*)it->Container.Data)->Other_It->Add
    (
        ((TMap*)it->Container.Data)->Other_It, 
        index, TG(TPair, pair)
    );

    ((TMap*)it->Container.Data)->Super.Size++;
}

TGeneric* TMap_Define_Get_Info(TIterator* it, int64_t index)
{
    
    TIterator* other_It = ((TMap*)it->Container.Data)->Other_It;
    TMap_Validate_Size((TMap*)it->Container.Data);
    void* value = other_It->Get(other_It, index);
    return &((TPair*)value)->Second;
}

TMap* TMap_Init(TIterator* it, uint32_t type_Count, uint32_t value_Count, ...)
{
    TMap* map = calloc(1, sizeof(TMap));
    Err_Alloc(map);
    va_list arg_List;
    value_Count += type_Count;
    va_start(arg_List, value_Count);
    value_Count -= type_Count;
    map->Other_It = it;
    TIterator_Init(&map->It, TG(TMap*, map), &map->Super.Size, TMap_Define_Get_Info, TMap_Define_Add);
    TContainer_Init(&map->Super, value_Count * 2, type_Count, TC_Allocator_Basic());
    
    for (uint32_t i = 0; i < type_Count; i++)
    {
        map->Super.Types[i] = *va_arg(arg_List, TRtti*);
    }

    for (uint32_t i = 0; i < value_Count; i++)
    {
        TPair* pair = va_arg(arg_List, TPair*);
        TMap_Multi(map, 1, pair);
    }

    va_end(arg_List);
    return map;
}

bool TMap_Add(TMap* map, TPair* key_Value_Pair)
{
    TMap_Multi(map, 1, key_Value_Pair);
}

TGeneric TMap_Element_Dtor(TGeneric* pair_Arg)
{
    TPair* pair = pair_Arg->Data;
    if (pair->First.Is_Allocated == true) TString_Free((TString*)pair->First.Data);
    TGeneric_Free(&pair->Second);
    TGeneric_Free(pair_Arg);
}

bool TMap_Multi(TMap* map, uint32_t value_Count, ...)
{
    va_list arg_List;
    va_start(arg_List, value_Count);
    TContainer* super = &map->Super;
    TMap_Validate_Size(map);

    for (uint32_t i = 0; i < value_Count; i++)
    {
        TPair* pair = va_arg(arg_List, TPair*);
        for (uint32_t j = 0; j < super->Size; j++)
        {
            TPair* pair_Cmp = map->Other_It->Get(map->Other_It, j);
            if (TString_Equal(pair_Cmp->First.Data, pair->First.Data) == true)
            {
                fprintf(stderr, "ERROR: in function [TMap_Multi(...)], tried adding a key that was already inside the TMap.");
            }
        }

        Type_Check(&pair->Second.Rtti_.Type, super->Types, super->Type_Count);
        TString* key = pair->First.Data;
        char* temp_Key_Str = key->Str;
        key->Str = super->Allocator.Calloc(1, key->Super.Size + 1);
        memcpy(key->Str, temp_Key_Str, key->Super.Size);
        TGeneric key_Gen = pair->First;
        TGeneric value_Gen = pair->Second;
        TContainer_Add_If_Pointer(super, &key_Gen, &pair->First);
        TContainer_Add_If_Pointer(super, &value_Gen, &pair->Second);
        pair->First = key_Gen;
        pair->Second = value_Gen;
        key = pair->First.Data;
        //printf("value type: %s.\n", pair->Second.Rtti_.Type.Str);
        uint32_t index = 0;
        if (super->Size > 0) index = super->Size - 1;
        map->Other_It->Add(map->Other_It, index, TG(TPair, pair));
        map->Other_It->Get_Info(map->Other_It, index)->Dtor = TMap_Element_Dtor;
        super->Size++;
    }

    va_end(arg_List);
}

TPair* TMap_Get_Info(TMap* map, TString* key)
{
    TMap_Validate_Size(map);
    for (uint32_t i = 0; i < map->Super.Size; i++)
    {
        TPair* pair = map->Other_It->Get(map->Other_It, i);
        if (TString_Equal(pair->First.Data, key) == true)
        {
            return pair;
        }
    }

    fprintf(stderr, "ERROR: in function [TMap_Get_Info(...)], could not find key '%s' in [TMap].\n", key->Str);
    exit(EXIT_FAILURE);
    return NULL;
}

void* TMap_Get(void* map_Arg, TString* key)
{
    TMap_Validate_Size(map_Arg);
    return TMap_Get_Info(map_Arg, key)->Second.Data;
}

void TMap_Remove(TMap* map, TString* key)
{
    TMap_Validate_Size(map);
    for (uint32_t i = 0; i < *map->It.Size; i++)
    {
        TPair* pair = map->Other_It->Get(map->Other_It, i);
        if (TString_Equal(pair->First.Data, key) == true)
        {
            TIterator_Remove(map->Other_It, i);
            *map->It.Size--;
            return;
        }
    }

    fprintf(stderr, "ERROR: in function [TMap_Remove(...)], could not find key '%s' in [TMap].\n", key->Str);
    exit(EXIT_FAILURE);
}

void TMap_Free(TMap* map)
{
    free(map->Super.Types);
    free(map);
}