#include "VMemory.h"
#include <stdlib.h>
#include "Utility.h"

TVector Memory_Pool;

void Memory_Push()
{
    TVector* vector = malloc(sizeof(TVector));
    TVector_Init(vector, 10, 0);
    TVector_Push(&Memory_Pool, &(TGeneric){ .Data = vector, .Rtti_ = Rtti(TVector*) });
}

void* Valloc(size_t size)
{
    TVector latest_Pool = *Cast(TVector*, Memory_Pool.Elements_[Memory_Pool.Size_ - 1].Data);
    TGeneric gen = TGeneric_Init(malloc(size), Rtti(void*), NULL);
    TVector_Push(&latest_Pool, &gen);
    return latest_Pool.Elements_[latest_Pool.Size_ - 1].Data;
}