#pragma once
#include <stddef.h>
#include "../TGeneric.h"

typedef struct TIterator
{
    TGeneric Container;
    TGeneric* (*Get_Info)(ssize_t index);
    size_t* Size; // The amount of elements inside the container this iterator is bound to.
    void* (*Get)(TIterator* it, ssize_t index);
    void (*Add)(TIterator* it, ssize_t index, TGeneric* value);
} TIterator;

void TIterator_Init
(
    TIterator* it, 
    TGeneric* container,
    size_t* container_Size,
    TGeneric* (*get_Info)(TIterator* it,ssize_t index), 
    void (*add)(TIterator* it,ssize_t index, TGeneric* value)
);