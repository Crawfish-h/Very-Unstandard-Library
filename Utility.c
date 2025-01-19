#include "Utility.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "TGeneric.h"
#include "Containers/TVector.h"
#include "Reflection.h"

void* Valloc(size_t size)
{
    TVector latest_Pool = *Cast(TVector*, Memory_Pool.Elements_[Memory_Pool.Size_ - 1].Data);
    TGeneric gen = TGeneric_Init(malloc(size), Rtti(void*), NULL);
    TVector_Push(&latest_Pool, &gen);
    return latest_Pool.Elements_[latest_Pool.Size_ - 1].Data;
}