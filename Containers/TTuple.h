#pragma once
#include <stddef.h>
#include "../Utility.h"
#include "TArray.h"

typedef struct TTuple TTuple;
typedef struct TArray TArray;
typedef struct TGeneric TGeneric;
typedef struct TRtti TRtti;

// [init_Size]: The size of the [TTuple] at initialization. 
// [types]: The types the [TTuple] can hold. Should be an [TArray] of [TRtti].
// [values]: Initializes the [TTuple] with some values. Should be an [TArray] of [TMabye TGeneric].
TTuple TTuple_Init(size_t init_Size, TArray* types, TArray* values);

void TTuple_Set(TTuple* tuple, size_t index, TGeneric* new_Value);

// Removes the value at [index] and returns the value.
TGeneric TTuple_Remove(TTuple* tuple, size_t index);
