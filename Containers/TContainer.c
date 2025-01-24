#include "TContainer.h"
#include <stdio.h>
#include <stdlib.h>
#include "TIterator.h"

size_t TContainer_Grow(TContainer* container, size_t new_Capacity)
{ 
    if (new_Capacity < container->Size)
    {
        TIterator it_End = It_End(container);
        for (TIterator it = It_At(container, container->Size - new_Capacity); It_Cmp(container, &it, &it_End); It_Next(container, &it))
        {
            if (it.Is_Allocated == true)
            {
                free(it.Value.Data);
            }
            it.Value = (TGeneric){ NULL };
        }

        container->Size = new_Capacity;
    }
    
    container->Capacity = new_Capacity;
    Array_Of(TGeneric) temp_Array = realloc(vector->Elements, new_Capacity * sizeof(TGeneric)); // WIP: create a realloc function for TContainer: void* (*Realloc)(TContainer* container).
    if (temp_Array != NULL)
    {
        vector->Elements = temp_Array;

        if (new_Capacity > container->Size)
        {
            memset(vector->Elements + container->Size, NULL, new_Capacity - container->Size);
        }
    }else
    {
        perror("ERROR: TContainer->Elements could not be reallocated.");
        free(vector->Elements);
        free(temp_Array);
        exit(EXIT_FAILURE);
    }
}

size_t TContainer_Size(TContainer* container)
{
    return container->Size;
}

size_t TContainer_Capacity(TContainer* container)
{
    return container->Capacity;
}

size_t TContainer_Type_Count(TContainer* container)
{
    return container->Type_Count;
}

size_t TContainer_Type_Capacity(TContainer* container)
{
    return container->Type_Capacity;
}