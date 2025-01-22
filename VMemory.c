#include "VMemory.h"
#include <stdlib.h>
#include "Utility.h"

TVector Memory_Pool;

void* Valloc(size_t size)
{
    TVector* latest_Pool = Cast(TVector*, Memory_Pool.Elements_[Memory_Pool.Size_ - 1].Data);
    TGeneric gen = TGeneric_Init(malloc(size), Rtti(void*), NULL);
    gen.Is_Allocated = true;
    TVector_Push(latest_Pool, &gen);
    return latest_Pool->Elements_[latest_Pool->Size_ - 1].Data;
}

void Memory_Push()
{
    TVector* vector = malloc(sizeof(TVector));
    TVector_Init(vector, 10, 0);
    TVector_Push(&Memory_Pool, &(TGeneric){ .Data = vector, .Rtti_ = Rtti(TVector*) });
}

void Memory_Pop()
{
    TVector* latest_Pool = Cast(TVector*, Memory_Pool.Elements_[Memory_Pool.Size_ - 1].Data);
    TVector_Free(latest_Pool);
    TVector_Pop(&Memory_Pool);
}

void Memory_Free()
{
    for (size_t i = 0; i < Memory_Pool.Size_; i++)
    {
        TVector* vector = Cast(TVector*, Memory_Pool.Elements_[i].Data);
        TVector_Free(vector);
        free(vector);
    }

    TVector_Free(Memory_Pool.Elements_);
}