#include "TIterator.h"

void* TIterator_Get(TIterator* it, ssize_t index)
{
    return it->Get_Info(it, index)->Data;
}

void TIterator_Init
(
    TIterator* it, 
    TGeneric* container,
    size_t* container_Size,
    TGeneric* (*get_Info)(TIterator* it, ssize_t index), 
    void (*add)(TIterator* it, ssize_t index, TGeneric* value)
)
{
    it->Add = add;
    it->Container = *container;
    it->Size = container_Size;
    it->Get_Info = get_Info;
    it->Get = TIterator_Get;
}