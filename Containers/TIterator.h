#pragma once
#include <stddef.h>
#include "../TGeneric.h"

typedef struct TIterator
{
    TGeneric Container;
    TGeneric* (*Get_Info)(TIterator* it, int64_t index);
    uint32_t* Size; // The amount of elements inside the container this iterator is bound to.
    void* (*Get)(TIterator* it, int64_t index);
    void (*Add)(TIterator* it, int64_t index, TGeneric* value);
} TIterator;

void TIterator_Init
(
    TIterator* it, 
    TGeneric* container,
    uint32_t* container_Size,
    TGeneric* (*get_Info)(TIterator* it, int64_t index), 
    void (*add)(TIterator* it,int64_t index, TGeneric* value)
);

// returns [true] if [element] was removed.
bool TIterator_Remove(TIterator* it, int64_t index);