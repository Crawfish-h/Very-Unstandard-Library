#include "TMap.h"
#include <stdarg.h>
#include <stdlib.h>
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



bool TMap_Add(TMap* map, TString* key, TGeneric* value)
{
    size_t index = Index_From_Hash(map, key);

    while (map->Pairs[index].First.Data != NULL)
    {
        if (TString_Equal(key, &*Cast(TString*, map->Pairs[index].First.Data)) == true)
        {
            map->Pairs[index].Second = *value;
            if (Is_Pointer(value->Rtti_) == false)
            {
                map->Pairs[index].Second.Data = calloc(1, value->Rtti_.Size_Of);
                map->Pairs[index].Second.Is_Allocated = true;
                memcpy(map->Pairs[index].Second.Data, value->Data, value->Rtti_.Size_Of);
            }

            return true;
        }

        index++;
        if (index >= map->Capacity) 
        {
            index = 0;
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
    return false;
}

TGeneric* TMap_Get(TMap* map, TString* key)
{
    size_t index = Index_From_Hash(map, key);
    while (map->Pairs[index].First.Data != NULL)
    {
        
    }
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
}