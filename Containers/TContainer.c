#include "TContainer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TIterator.h"
#include "../Utility.h"

void TContainer_Init
(
    TContainer* container, size_t capcity, size_t type_Count, 
    TGeneric* (*container_Get)(TContainer* container, ssize_t index), 
    void (*container_Add)(TContainer* container, ssize_t index, TGeneric* value),
    TC_Allocator allocator
)
{
    container->Allocator = allocator;
    container->Size = 0;
    container->Type_Count;
    container->Type_Capacity = type_Count * 2;
    container->Capacity = capcity;
    if (container->Type_Capacity == 0) container->Type_Capacity = 4;
    if (container->Capacity == 0) container->Capacity = 1;
    container->Types = container->Allocator.Calloc(container->Type_Capacity, container->Type_Capacity * sizeof(TRtti));
    Err_Alloc(container->Types);
    container->Get = container_Get;
    container->Add = container_Add;
}

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

void TContainer_Add_If_Pointer(TContainer* container, TGeneric* value, TGeneric* new_Value)
{
    *value = *new_Value;
    if (Is_Pointer(new_Value->Rtti_) == false)
    {
        value->Data = container->Allocator.Calloc(1, new_Value->Rtti_.Size_Of);
        value->Is_Allocated = true;
        container->Allocator.Memcpy(value->Data, new_Value->Data, new_Value->Rtti_.Size_Of);
    }
}

bool TContainer_Add_Type(TContainer* container, TRtti* new_Type)
{
    for (size_t i; i < container->Type_Count; i++)
    {
        if (Compare_Types(&container->Types[i], new_Type) == true)
        {
            return false;
        }
    }

    container->Type_Count++;
    if (container->Type_Count >= container->Type_Capacity)
    {
        container->Type_Capacity *= 2;
        Array_Of(TRtti) temp_Array = container->Allocator.Realloc(container->Types, container->Type_Capacity * sizeof(TRtti));
        if (temp_Array != NULL)
        {
            container->Types = temp_Array;
        }else
        {
            perror("ERROR: container->Types could not be reallocated.");
            free(container->Types);
            free(temp_Array);
            exit(EXIT_FAILURE);
        }
    }

    container->Types[container->Type_Count - 1] = *new_Type;
    return true; 
}

bool TContainer_Remove_Type(TContainer* container, TRtti* type)
{
    TRtti temp_Array[container->Type_Capacity];
    container->Allocator.Memcpy(temp_Array, container->Types, container->Type_Capacity);
    size_t j = 0;
    bool type_Found = false;
    for (size_t i = 0; i < container->Type_Count; i++)
    {
        if (Compare_Types(&container->Types[i], type) == true) 
        {
            type_Found = true;
            continue;
        }
        
        container->Types[j] = temp_Array[i];
        j++;
    }

    return type_Found;
}