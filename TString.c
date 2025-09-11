#include "TString.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Reflection.h"
#include "TGeneric.h"

TString* TString_Dyn(char* string)
{
    TString* tstring = calloc(1, sizeof(TString));
    tstring->Super.Size = strlen(string);
    tstring->Super.Capacity = tstring->Super.Size * 2;
    tstring->Str = calloc(tstring->Super.Capacity, sizeof(char));
    memcpy(tstring->Str, string, tstring->Super.Size);
    return tstring;
}

void TString_Insert0(TString* string, char* other_String, int64_t index)
{
    TContainer* super = &string->Super;
    uint32_t other_Length = strlen(other_String);
    if (index < 0)
    {
        index = super->Size + ++index;
    }
    
    if (index > super->Size || index < 0)
    {
        fprintf(stderr, "ERROR: in functon [TString_Insert], argument [index] is out of bounds.\n");
        exit(EXIT_FAILURE);
    }

    char* temp_String = calloc(super->Size, sizeof(char));
    memcpy(temp_String, string->Str, super->Size);

    if (other_Length + super->Size >= super->Capacity)
    {
        super->Capacity = (other_Length + super->Size) * 2;
    }

    string->Str = calloc(super->Capacity, sizeof(char));
    memcpy(string->Str, temp_String, index);
    memcpy(string->Str + index, other_String, other_Length);

    uint32_t j = index;
    for (uint32_t i = index + other_Length; i < other_Length + super->Size; i++)
    {
        string->Str[i] = temp_String[j];
        j++;
    }

    super->Size += other_Length;
    string->Str[super->Size] = '\0';
    free(temp_String);
}

void TString_Insert1(TString* string, TString* other_String, int64_t index)
{
    TContainer* super = &string->Super;
    if (index < 0)
    {
        index = super->Size + ++index;
    }
    
    if (index > super->Size || index < 0)
    {
        fprintf(stderr, "ERROR: in functon [TString_Insert], argument [index] is out of bounds.\n");
        exit(EXIT_FAILURE);
    }

    char* temp_String = string->Str;

    if (other_String->Super.Capacity >= super->Capacity)
    {
        super->Capacity = other_String->Super.Capacity * 2;
    }

    string->Str = calloc(super->Capacity, sizeof(char));
    memcpy(string->Str, temp_String, index);
    memcpy(string->Str + index, other_String->Str, other_String->Super.Size);

    uint32_t j = index;
    for (uint32_t i = index + other_String->Super.Size; i < other_String->Super.Size + super->Size; i++)
    {
        string->Str[i] = temp_String[j];
        j++;
    }

    super->Size += other_String->Super.Size;
    string->Str[super->Size] = '\0';
    free(temp_String);
}

void TString_Free(TString* string)
{
    free(string->Str);
    free(string);
}

bool TString_Equal(TString* str_0, TString* str_1)
{
    if (str_0->Super.Size != str_1->Super.Size) return false;
    
    for (uint32_t i = 0; i < str_0->Super.Size; i++)
    {
        if (str_0->Str[i] != str_1->Str[i]) return false;
    }

    return true;
}

bool TString_Equal_N(TString* str_0, TString* str_1, uint32_t n)
{    
    for (uint32_t i = 0; i < n; i++)
    {
        if (str_0->Str[i] != str_1->Str[i]) return false;
    }

    return true;
}

TString TString_Str_Str(TString* string, TString* sub_String)
{
    const char *p = string->Str;
    if (sub_String->Super.Size == 0) return *string;
    
    for (; (p = strchr(p, *sub_String->Str)) != 0; p++)
    {
        if (strncmp(p, sub_String->Str, sub_String->Super.Size) == 0)
            return (TString){ .Super.Size = sub_String->Super.Size, .Str = (char*)p, .Super = string->Super };
    }

    return (TString){ 0 };
}