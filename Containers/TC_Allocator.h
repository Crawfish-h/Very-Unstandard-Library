#pragma once
#include <stddef.h>

typedef struct TContainer TContainer;
typedef struct TGeneric TGeneric;

typedef struct TC_Allocator
{
    void* (*Alloc)(size_t size);
    void* (*Realloc)(void* memory, size_t new_Size);
    void* (*Calloc)(size_t num_Elements, size_t size_Of_Elements);
    void* (*Memcpy)(void* destination, const void * __restrict__ source, size_t size_Of);
    void* (*Memmove)(void *_Dst, const void *_Src, size_t size_Of);
    TGeneric (*Free)(TContainer* container);
} TC_Allocator;

// Returns a TC_Allocator suited for basic manual memory management. [TC_Allocator.Free] is [NULL].
TC_Allocator TC_Allocator_Basic();

// Returns a TC_Allocator suited for arena-based manual memory management. [TC_Allocator.Free] is [NULL].
TC_Allocator TC_Allocator_Arena();