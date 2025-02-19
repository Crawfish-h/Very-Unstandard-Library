#include "TContainer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TIterator.h"

size_t TContainer_Grow(TContainer* container, size_t new_Capacity, TGeneric* arg)
{ 
    /*
    if (new_Capacity < container->Size)
    {
        TIterator it_End = It_End(container);
        for (TIterator it = It_At(container, container->Size - new_Capacity); It_Cmp(container, &it, &it_End); It_Next(container, &it))
        {
            if (it.Is_Allocated == true)
            {
                free(it.Value.Data);
            }
            void** d_Void_Ptr = &it.Value.Data;
            *d_Void_Ptr = NULL;
        }

        container->Size = new_Capacity;
    }
    
    container->Capacity = new_Capacity;
    //void* temp_Container_Data = realloc(container->Data_Ptr.Data, new_Capacity * sizeof(TGeneric)); // WIP: create a realloc function for TContainer: void* (*Expand)(TContainer* container).
    void* temp_Container_Data = container->Container_Realloc(container, new_Capacity, arg);
    if (temp_Container_Data != NULL)
    {
        if (new_Capacity > container->Size)
        {
            printf("New capcity\n");
            //memset(container->Data_Ptr.Data + container->Size, NULL, new_Capacity - container->Size);
            TIterator it_End = It_At(container, new_Capacity);
            for (TIterator it = It_At(container, container->Size); It_Cmp(container, &it, &it_End); It_Next(container, &it))
            {
                printf("value set\n");
                void** d_Void_Ptr = &it.Value.Data;
                *d_Void_Ptr = NULL;
            }
        }
    }else
    {
        perror("ERROR: container element data could not be reallocated.");
        container->Allocator.Free(container);
        free(temp_Container_Data);
        exit(EXIT_FAILURE);
    }
    */
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

void* TContainer_Array_Alloc_Again(TContainer* container, size_t new_Capacity, TGeneric* arg)
{
    void* container_Elements = container->Allocator.Calloc(new_Capacity, (size_t)arg->Data);
    memcpy(container_Elements, *container->Data_Ptr, container->Capacity - 1);
    free(*container->Data_Ptr);
    *container->Data_Ptr = container_Elements;
}