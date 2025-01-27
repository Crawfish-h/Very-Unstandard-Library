#include "Utility.h"

void Err_Alloc(void* ptr)
{
    if (ptr == NULL)
    {
        Err_Verbose("ERROR: could not allocate on the heap");
    }
}