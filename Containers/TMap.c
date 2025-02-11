#include "TMap.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../Reflection.h"
#include "TContainer.h"
#include "It_Array.h"

typedef struct TMap
{
    TContainer Super;
    Array_Of(TPair) Pairs; 
} TMap;

void* TMap_Container_Realloc(TContainer* container, size_t new_Capacity)
{
    TMap* map = (TMap*)container;
    map->Pairs = map->Super.Allocator.Realloc(map->Pairs, new_Capacity * sizeof(TPair));
    return map->Pairs;
}

TGeneric TMap_Index_Get(TContainer* container, TGeneric* index);

inline TGeneric TMap_Index_Get(TContainer* container, TGeneric* index)
{
    TMap* map = (TMap*)container;
    size_t index_Sz = *(size_t*)index->Data;

    TPair* pair = NULL;
    size_t pairs_Found = 0;
    for (size_t i = 0; i < container->Capacity; i++)
    {
        if (map->Pairs[i].First.Data != NULL)
        {
            pairs_Found++;
            if (pairs_Found >= index_Sz)
            {
                pair = &map->Pairs[i];
                break;
            }
        }
    }
    
    return (TGeneric){ .Data = pair, .Rtti_ = Rtti(TPair) };
}

TGeneric TMap_Allocator_Free(TContainer* container)
{
    TMap_Free((TMap*)container);
    return (TGeneric){ NULL };
}

TMap* TMap_Init(size_t capacity, size_t type_Count, ...)
{
    va_list va_Args;
    va_start(va_Args, type_Count);
    TMap* map = malloc(sizeof(TMap));
    Err_Alloc(map);
    TContainer* super = &map->Super;
    super->Allocator = TC_Allocator_Basic();
    super->Allocator.Free = TMap_Allocator_Free;
    super->Capacity = capacity;
    map->Pairs = calloc(super->Capacity, sizeof(TPair));
    super->Data_Ptr = (void**)&map->Pairs;
    Err_Alloc(map->Pairs);
    super->Size = 0;
    super->Type_Count = type_Count;
    super->Type_Capacity = type_Count * 2;
    super->Types = malloc(type_Count * sizeof(TRtti));
    Err_Alloc(super->Types);
    super->C_It_Begin = It_Array_Begin;
    super->C_It_At = It_Array_At;
    super->C_It_End = It_Array_End;
    super->C_It_Next = It_Array_Next;
    super->C_It_Cmp = It_Array_Cmp;
    super->Container_Realloc = TContainer_Array_Alloc_Again;
    super->Index_Get = TMap_Index_Get;
    super->Container_Type = Rtti(TMap);

    for (size_t i = 0; i < type_Count; i++)
    {
        super->Types[i] = va_arg(va_Args, TRtti);
    }

    va_end(va_Args);
    return map;
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// FNV-1 Hash function from https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1_hash
size_t Index_From_Hash(TMap* map, TString* key)
{
    TContainer* super = &map->Super;
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key->Str; *p; p++)
    {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    
    return (size_t)(hash & (uint64_t)(super->Capacity - 1));
}

bool Find_Key(TMap* map, TString* key, size_t* out_Index)
{
    TContainer* super = &map->Super;
    size_t index = Index_From_Hash(map, key);
    size_t elements_Searched = 0;
    while (map->Pairs[index].First.Data != NULL)
    {
        //printf("key: %s.\n", Cast(TString*, map->Pairs[index].First.Data)->Str);
        if (TString_Equal(key, Cast(TString*, map->Pairs[index].First.Data)) == true)
        {
            *out_Index = index;
            return true;
        }

        index++;
        elements_Searched++;
        if (index >= super->Capacity) 
        {
            index = 0;
        }

        if (elements_Searched == super->Capacity)
        {
            break;
        }
    }
    
    *out_Index = index;
    return false;
}

bool TMap_Add(TMap* map, TString* key, TGeneric* value)
{
    TContainer* super = &map->Super;
    Type_Check(&value->Rtti_.Type, super->Types, super->Type_Count);

    size_t index;
    if (Find_Key(map, key, &index) == true)
    {
        return false;
    }

    super->Size++;

    if (super->Size == super->Capacity)
    {
        size_t old_Capcity = super->Capacity;
        super->Capacity *= 2;
        Array_Of(TPair) temp_Array = realloc(map->Pairs, super->Capacity * sizeof(TPair));
        if (temp_Array != NULL)
        {
            map->Pairs = temp_Array;
        }else
        {
            perror("ERROR: TMap->Pairs could not be reallocated.");
            free(map->Pairs);
            free(temp_Array);
            exit(EXIT_FAILURE);
        }

        for (size_t i = old_Capcity; i < super->Capacity; i++)
        {
            map->Pairs[i] = (TPair){ NULL };
        }
    }

    map->Pairs[index].First = (TGeneric){ .Data = key, .Rtti_ = Rtti(TString) };
    map->Pairs[index].Second = *value;
    if (Is_Pointer(value->Rtti_) == false)
    {
        map->Pairs[index].Second.Data = calloc(1, value->Rtti_.Size_Of);
        map->Pairs[index].Second.Is_Allocated = true;
        memcpy(map->Pairs[index].Second.Data, value->Data, value->Rtti_.Size_Of);
    }

    return true;
}

void TMap_Remove(TMap* map, TString* key)
{
    TContainer* super = &map->Super;
    size_t index;
    if (Find_Key(map, key, &index) == false)
    {
        fprintf(stderr, "ERROR: invalid key argument for TMap_Remove(...): %s.\n", key->Str);
        exit(EXIT_FAILURE);
    }

    if (map->Pairs[index].Second.Is_Allocated == true)
    {
        free(map->Pairs[index].Second.Data);
    }

    map->Pairs[index] = (TPair){  };
    super->Size--;
}

TGeneric* TMap_Get(TMap* map, TString* key)
{
    size_t index;
    if (Find_Key(map, key, &index) == true)
    {
        return &map->Pairs[index].Second;
    }
    
    fprintf(stderr, "ERROR: invalid key argument for TMap_Get(...): %s.\n", key->Str);
    exit(EXIT_FAILURE);
    return NULL;
}

void TMap_Free(TMap* map)
{
    TContainer* super = &map->Super;
    for (size_t i = 0; i < super->Size; i++)
    {
        if (map->Pairs[i].Second.Is_Allocated == true)
        {
            free(map->Pairs[i].Second.Data);
        }
    }

    free(map->Pairs);
    free(super->Types);
    free(map);
    map = NULL;
}