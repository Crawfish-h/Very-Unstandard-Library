#pragma once
#include "../TPair.h"
#include <stddef.h>

typedef struct TMap TMap;
typedef struct TString TString;
typedef struct TGeneric TGeneric;

// types of all [...] arguments should be a TRtti. 
TMap* TMap_Init(size_t capacity, size_t type_Count, ...);

// Returns true if [key] already exists inside [map->Pairs].
// Copies an added value if it is not a pointer type.
bool TMap_Add(TMap* map, TString* key, TGeneric* value);

TGeneric* TMap_Get(TMap* map, TString* key);

void TMap_Free(TMap* map);