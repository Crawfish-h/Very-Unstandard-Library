#include "Utility.h"
#include <stdlib.h>

void Err_Alloc(void* ptr)
{
    if (ptr == NULL)
    {
        Err_Verbose("ERROR: could not allocate on the heap");
    }
}

void Index_Check(int64_t* index, uint32_t max_Index_Val)
{
    if (*index < 0) *index = max_Index_Val - *index;
    if (*index > max_Index_Val || *index < 0)
    {
        fprintf(stderr, "ERROR: index out if range in function getter function.");
        abort();
    }
}