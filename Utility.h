#pragma once
#include <stddef.h>
#include <stdio.h>
#include "TString.h"


typedef struct TString TString;
typedef TString Type_Name;
typedef struct TArray TArray;
typedef struct TGeneric TGeneric;
typedef struct TObject TObject;
typedef struct TManip_Fns TManip_Fns;

typedef TGeneric (*Generic_Function)(int arg_Count, ...);

typedef void* TArg[];

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

#define Choose_Type_Str_Check_Fn_()

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
        #define true	(!0)
        #define false	0
    #endif


#define Err_Verbose(msg) \
    fprintf(stderr, "%s in file %s on line %d\n", msg, __FILE__, __LINE__);


void Err_Alloc(void* ptr);

typedef struct TMaybe
{
    void* Data;
    bool Is_Nothing;
} TMaybe;

#define Nothing (TMaybe){ .Data = NULL, .Is_Nothing = false }

#define Just(value) \
({ \
    typeof(value)* vul_Just_Value_ = &value; \
    TMaybe* vul_Just_Maybe_ = &(TMaybe){ .Data = vul_Just_Value_, .Is_Nothing = true }; \
    vul_Just_Maybe_; \
})

#define Num_Args(args) (sizeof((TGeneric[]){args})/sizeof(TGeneric))

// Converts an r value to a l value's address.
#define L_Val(r_Value) &(typeof(r_Value)[1]){ r_Value }

