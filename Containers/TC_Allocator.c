#include "TC_Allocator.h"
#include <stdlib.h>
#include <string.h>

TC_Allocator TC_Allocator_Basic()
{
    return (TC_Allocator){ .Alloc = malloc, .Realloc = realloc, .Calloc = calloc, .Memcpy = memcpy };
}

TC_Allocator TC_Allocator_Arena()
{
    return (TC_Allocator){ .Alloc = malloc, .Realloc = realloc, .Calloc = calloc, .Memcpy = memcpy };
}