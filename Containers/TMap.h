#pragma once
#include "../TPair.h"
#include <stddef.h>
#include "TIterator.h"

typedef struct TString TString;
typedef struct TGeneric TGeneric;
typedef struct TContainer TContainer;

typedef struct TMap
{
    TContainer Super;
    TIterator* Other_It;
    TIterator It;
} TMap;

// types of all [...] arguments should be a TRtti. 
TMap* TMap_Init(TIterator* it, uint32_t type_Count, uint32_t value_Count, ...);

// Returns true if [key] does not exist inside [map->Pairs].
// Does not add a key-value pair if the provided key already exists inside of the map.
// Copies an added value if it is not a pointer type.
bool TMap_Add(TMap* map, TPair* key_Value_Pair);

// Returns true if the added [key]s does not exist inside [map->Pairs].
// Does not add key-value pairs if the provided keys already exists inside of the map.
// Copies an added value if it is not a pointer type.
bool TMap_Multi(TMap* map, uint32_t value_Count, ...);

void* TMap_Get(void* map_Arg, TString* key);
TPair* TMap_Get_Info(TMap* map, TString* key);

void TMap_Remove(TMap* map, TString* key);
void TMap_Free(TMap* map);