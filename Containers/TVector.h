#pragma once
#include <stddef.h>
#include "../Utility.h"
#include "../TGeneric.h"
#include "../TString.h"
#include "TIterator.h"

typedef struct TVector
{
    TContainer Super;
    Array_Of(TGeneric) Elements; 
    bool (*Sort)(TGeneric* value);
    TIterator It;
} TVector;

// Initializes a [TVector]. Can only initialize [TVectors] with types, no values.
// types of all [...] arguments should be a TRtti. 
TVector* TVector_Init(uint32_t type_Count, uint32_t value_Count, ...);

// Returns [true] if [new_Capacity] > [vector->Super.Capcity].
bool TVector_Grow(TVector* vector, uint32_t new_Capacity);

// Pushes a value to a [TVector].
// Types of all [...] arguments should be a [TGeneric*]. 
// Copies a pushed value if it is not a pointer type.
void TVector_Multi(TVector* vector, uint32_t value_Count, ...);

void TVector_Push(TVector* vector, TGeneric* value);

void TVector_Add_At(TVector* vector, int64_t index, TGeneric* value);

void TVector_Clear(TVector* vector);
void TVector_Free(TVector* vector);

// Removes the element at [index] and frees it if it was allocated.
void TVector_Remove_At(TVector* vector, int64_t index); 

// Removes the element at [index] and returns it.
void TVector_Remove_At1(TVector* vector, int64_t index); 

// Removes the last element in the vector and frees it if it was allocated.
void TVector_Pop(TVector* vector); 

// Removes the last element in the vector and returns it.
TGeneric TVector_Pop1(TVector* vector); 

TGeneric* TVector_Get_Info(TVector* vector, int64_t index);
void* TVector_Get(TVector* vector, int64_t index);
