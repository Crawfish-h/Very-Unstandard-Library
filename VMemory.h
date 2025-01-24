#pragma once 

typedef struct TVector TVector;
extern TVector* Memory_Pool;
void* Valloc(size_t size);
void Memory_Push();
void Memory_Pop();
void Memory_Free();