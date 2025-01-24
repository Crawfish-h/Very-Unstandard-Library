#pragma once
#include "../TGeneric.h"
#include "../Utility.h"
#include "../Reflection.h"

typedef struct TIterator
{
    TRtti Container_Type;

    // Holds a pointer to an element of an container. Doesn't need to be initialized.
    TGeneric Value; 
    
    // Should be true if [Value.Data] was allocated on the heap.
    bool Is_Allocated;
} TIterator;

typedef struct TContainer TContainer;

void TIterator_Next(TIterator* it);
TIterator It_Begin(TContainer* container);
TIterator It_At(TContainer* container, size_t index);
TIterator It_End(TContainer* container);
void It_Next(TContainer* container, TIterator* it);
bool It_Cmp(TContainer* container, TIterator* it_0, TIterator* it_1);