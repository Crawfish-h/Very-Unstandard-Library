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

extern TVector Memory_Pool;


// Initializes a [TVector]. Can only initialize [TVectors] with types, no values.
// types of all [...] arguments should be a TString. 
void TVector_Init(TVector* vector, const size_t capcity, const size_t type_Count, ...);

// Pushes a value to a [TVector].
// Types of all [...] arguments should be a TGeneric. 
void TVector_Multi(TVector* vector, const size_t value_Count, ...);
void TVector_Push(TVector* vector, TGeneric value);
void Memory_Push();