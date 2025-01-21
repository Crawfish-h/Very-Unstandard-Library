#pragma once
#include <stddef.h>
#include "../Utility.h"
#include "../TGeneric.h"
#include "../TString.h"

typedef struct TVector
{
    TObject* Super;
    Array_Of(TGeneric) Elements_; 
    size_t Size_;
    size_t Capacity_;
    Array_Of(TRtti) Types_;
    size_t Type_Count_;
    size_t Type_Capacity_;
} TVector;

// Initializes a [TVector]. Can only initialize [TVectors] with types, no values.
// types of all [...] arguments should be a TString. 
void TVector_Init(TVector* vector, const size_t capcity, const size_t type_Count, ...);

// Pushes a value to a [TVector].
// Types of all [...] arguments should be a [TGeneric*]. 
void TVector_Multi(TVector* vector, const size_t value_Count, ...);
void TVector_Push(TVector* vector, TGeneric* value);
void TVector_Pop(TVector* vector); // Removes the last element in the vector and frees it if it was allocated.
TGeneric TVector_Pop1(TVector* vector); // Removes the last element in the vector and returns it.
void TVector_Free(TVector* vector);
void TVector_Remove_At(TVector* vector, ssize_t index); // Removes the element at [index] and frees it if it was allocated.
TGeneric TVector_Remove_At1(TVector* vector, ssize_t index); // Removes the element at [index] and returns it.