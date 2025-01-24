#include "TMap.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../Reflection.h"

typedef struct TMap
{
    Array_Of(TPair) Pairs; 
    size_t Size;
    size_t Capacity;
    Array_Of(TRtti) Types;
    size_t Type_Count;
    size_t Type_Capacity;
} TMap;

TMap* TMap_Init(size_t capacity, size_t type_Count, ...)
{
    va_list va_Args;
    va_start(va_Args, type_Count);
    TMap* map = malloc(sizeof(TMap));
    map->Capacity = capacity;
    map->Pairs = calloc(map->Capacity, sizeof(TPair));
    map->Size = 0;
    map->Type_Count = type_Count;
    map->Type_Capacity = type_Count * 2;
    map->Types = malloc(type_Count * sizeof(TRtti));

    for (size_t i = 0; i < type_Count; i++)
    {
        map->Types[i] = va_arg(va_Args, TRtti);
    }

    va_end(va_Args);
    return map;
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// FNV-1 Hash function from https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1_hash
size_t Index_From_Hash(TMap* map, TString* key)
{
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key->Str; *p; p++)
    {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    
    return (size_t)(hash & (uint64_t)(map->Capacity - 1));
}

// If the provided key is in the map, the return value will be true. [out_Index] has the value of the found key, or the last index gotten in the while loop.
// If the provided key is not in the map, then [out_Index] will have a value of [0] and the return bool will be false. 
bool Find_Key(TMap* map, TString* key, size_t* out_Index)
{
    size_t index = Index_From_Hash(map, key);
    while (map->Pairs[index].First.Data != NULL)
    {
        if (TString_Equal(key, &*Cast(TString*, map->Pairs[index].First.Data)) == true)
        {
            *out_Index = index;
            return true;
        }

        index++;
        if (index >= map->Capacity) 
        {
            index = 0;
        }
    }
    
    *out_Index = index;
    return false;
}

bool TMap_Add(TMap* map, TString* key, TGeneric* value)
{
    Type_Check(&value->Rtti_.Type, map->Types, map->Type_Count);

    size_t index;
    if (Find_Key(map, key, &index) == true)
    {
        return false;
    }

    map->Size++;

    if (map->Size == map->Capacity)
    {
        map->Capacity *= 2;
        Array_Of(TPair) temp_Array = realloc(map->Pairs, map->Capacity * sizeof(TPair));
        if (temp_Array != NULL)
        {
            map->Pairs = temp_Array;
        }else
        {
            perror("ERROR: vector->Elements_ could not be reallocated.");
            free(map->Pairs);
            free(temp_Array);
            exit(EXIT_FAILURE);
        }

        for (size_t i = map->Size; i < map->Capacity; i++)
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
    map->Size--;
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
    for (size_t i = 0; i < map->Size; i++)
    {
        if (map->Pairs[i].Second.Is_Allocated == true)
        {
            free(map->Pairs[i].Second.Data);
        }
    }

    free(map->Pairs);
    free(map->Types);
    free(map);
    *map = (TMap){  };
}