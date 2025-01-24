#pragma once
#include <stddef.h>
#include "../Utility.h"
#include "../TGeneric.h"
#include "../TString.h"

typedef struct TVector TVector;

// Initializes a [TVector]. Can only initialize [TVectors] with types, no values.
// types of all [...] arguments should be a TRtti. 
TVector* TVector_Init(size_t capcity, size_t type_Count, ...);

// Pushes a value to a [TVector].
// Types of all [...] arguments should be a [TGeneric*]. 
// Copies a pushed value if it is not a pointer type.
void TVector_Multi(TVector* vector, size_t value_Count, ...);

void TVector_Push(TVector* vector, TGeneric* value);

void TVector_Free(TVector* vector);

// Removes the element at [index] and frees it if it was allocated.
void TVector_Remove_At(TVector* vector, ssize_t index); 

// Removes the last element in the vector and frees it if it was allocated.
void TVector_Pop(TVector* vector); 

// Removes the last element in the vector and returns it.
TGeneric TVector_Pop1(TVector* vector); 

// Removes the element at [index] and returns it.
TGeneric TVector_Remove_At1(TVector* vector, ssize_t index); 

TGeneric* TVector_Get(TVector* vector, size_t index);
