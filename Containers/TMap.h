#pragma once
#include "../TPair.h"
#include <stddef.h>

typedef struct TMap TMap;
typedef struct TString TString;
typedef struct TGeneric TGeneric;

// types of all [...] arguments should be a TRtti. 
TMap* TMap_Init(size_t capacity, size_t type_Count, ...);

// Returns true if [key] does not exist inside [map->Pairs].
// Does not add a key-value pair if the provided key already exists inside of the map.
// Copies an added value if it is not a pointer type.
bool TMap_Add(TMap* map, TString* key, TGeneric* value);
void TMap_Remove(TMap* map, TString* key);

TGeneric* TMap_Get(TMap* map, TString* key);

void TMap_Free(TMap* map);