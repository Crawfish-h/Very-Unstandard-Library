#pragma once
#include <stddef.h>
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

    // Some containers may want to pass different types of data to this iterator.
    TGeneric Metadata;

    // The index of the element inside of a [TContainer] that this [TIterator.Value.Data] is pointing to.
    size_t Current_Index;
} TIterator;

typedef struct TContainer TContainer;

TIterator It_Begin(TContainer* container);
TIterator It_At(TContainer* container, size_t index);
TIterator It_End(TContainer* container);
void It_Next(TContainer* container, TIterator* it);
bool It_Cmp(TContainer* container, TIterator* it_0, TIterator* it_1);