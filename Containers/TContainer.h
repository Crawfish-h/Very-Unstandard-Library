#pragma once
#include <stddef.h>
#include "../Utility.h"
#include "../TObject.h"
#include "../TGeneric.h"

typedef struct TContainer
{
    TObject;
    Array_Of(TGeneric) Data_; // [Data_.Data] should point to an array or container of some kind. 
    size_t Size_;
    size_t Capacity_;
    Array_Of(TString) Type_Data_;
    size_t Type_Count_;
    size_t Type_Capacity_;
} TContainer;
