#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct TIterator TIterator;
typedef struct TContainer TContainer;

TIterator It_Array_Begin(TContainer* container);

TIterator It_Array_At(TContainer* container, size_t index);

TIterator It_Array_End(TContainer* container);

void It_Array_Next(TContainer* container, TIterator* it);

bool It_Array_Cmp(TContainer* container, TIterator* it_0, TIterator* it_1);