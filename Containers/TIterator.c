#include "TIterator.h"

void* TIterator_Get(TIterator* it, int64_t index)
{
    return it->Get_Info(it, index)->Data;
}

void TIterator_Init
(
    TIterator* it, 
    TGeneric* container,
    uint32_t* container_Size,
    TGeneric* (*get_Info)(TIterator* it, int64_t index), 
    void (*add)(TIterator* it, int64_t index, TGeneric* value)
)
{
    it->Add = add;
    it->Container = *container;
    it->Size = container_Size;
    it->Get_Info = get_Info;
    it->Get = TIterator_Get;
}

void TIterator_Remove(TIterator* it, uint index)
{
    Index_Check(&index, *it->Size);

    TGeneric_Free(it->Get_Info(it, index));
    for (uint32_t i = index; i < *it->Size; i++)
    {
        
    }
    
}