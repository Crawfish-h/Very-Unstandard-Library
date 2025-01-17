#pragma once
#include <stddef.h>
#include "TString.h"


typedef struct TString TString;
typedef TString Type_Name;
typedef struct TArray TArray;
typedef struct TGeneric TGeneric;
typedef struct TObject TObject;
typedef struct TManip_Fns TManip_Fns;

typedef TGeneric (*Generic_Function)(int arg_Count, ...);

typedef struct TRtti
{
    TString Type;
    size_t Size_Of;
} TRtti;

/*
    Generic_Function Ctor;
    Generic_Function Dtor;
    Generic_Function Move;
    Generic_Function Copy;
    Generic_Function Set;
    Generic_Function Is_Equal;
    Generic_Function Less;
    Generic_Function Less_Equal;
    Generic_Function Greater;
    Generic_Function Greater_Equal;
    Generic_Function Add;
    Generic_Function Subtract;
    Generic_Function Multiply;
    Generic_Function Divide;
    Generic_Function To_String;
*/

// Returns runtime type information. 
// [check] exists so that if a nonexistant variable is passed to it, it will generate an error.
// Returns a large struct containing more type data.
TRtti Type_Str_Check_Large_(void* check, TString type_String, 
    Generic_Function ctor,
    Generic_Function dtor,
    Generic_Function move,
    Generic_Function copy,
    Generic_Function set,
    Generic_Function is_Equal,
    Generic_Function less,
    Generic_Function less_Equal,
    Generic_Function greater,
    Generic_Function greater_Equal,
    Generic_Function add,
    Generic_Function subtract,
    Generic_Function multiply,
    Generic_Function divide,
    Generic_Function to_String
);

// Returns runtime type information. 
// [check] exists so that if a nonexistant variable is passed to it, it will generate an error.
TRtti TRtti_Init_(void* check, size_t size_Of_Type, TString type_String);

#define Choose_Type_Str_Check_Fn_()

// Returns runtime type information. 
// First argument is the type, second argument is optional and also a bool
// that, if supplied and true, runs a function that returns a large structure.
#define Rtti(type) TRtti_Init_((type*)NULL, sizeof(type), NT_TString(Stringify(type)))
#define Rtti_L(type) Type_Str_Check_Large_((type*)NULL, NT_TString(Stringify(type)))
#define Stringify_2(x) #x
#define Stringify(x) Stringify_2(x)
#define Array_Of(type) type*
#define Cast(type, var) ((type)var)
#define Cast_2(type, var, var_2) Cast(type, ((type)var)->var_2)
#define Type_Id(type_Name) \
    type_Name; \
    static const Type_Name Type_ ## type_Name = { .Str = Stringify(type_Name), .Size = (sizeof(Stringify(type_Name)) / sizeof(Stringify(type_Name)[0]) - 1)  }; \
    static const Type_Name Type_ ## type_Name ## _p = { .Str = Stringify(type_Name*), .Size = (sizeof(Stringify(type_Name*)) / sizeof(Stringify(type_Name*)[0]) - 1)  }; \
    static const Type_Name Type_ ## type_Name ## _pp = { .Str = Stringify(type_Name**), .Size = (sizeof(Stringify(type_Name**)) / sizeof(Stringify(type_Name**)[0]) - 1)  }; \
    static const Type_Name Type_ ## type_Name ## _ppp = { .Str = Stringify(type_Name***), .Size = (sizeof(Stringify(type_Name***)) / sizeof(Stringify(type_Name***)[0]) - 1)  };

// This is here because my intellisense does not recognize stdbool.h.
#if defined false || defined true || defined bool
    #else
        #define bool	_Bool
        #define true	1
        #define false	0
    #endif

void* Valloc(TObject* object, size_t size);
void Type_Check(TString type_String, Array_Of(TRtti) types, size_t types_Count);