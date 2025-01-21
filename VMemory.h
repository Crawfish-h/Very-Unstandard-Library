#pragma once 
#include "Containers/TVector.h"

extern TVector Memory_Pool;
void Memory_Push();
void* Valloc(size_t size);