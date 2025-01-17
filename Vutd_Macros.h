#pragma once
#include <stdio.h>
// Credits: Stackoverflow user "Andre Kampling" at https://stackoverflow.com/questions/45585903/c-macros-how-to-map-another-macro-to-variadic-arguments.
// https://github.com/18sg/uSHET/blob/master/lib/cpp_magic.h.

#define FIRST_(a, ...) a
#define SECOND_(a, b, ...) b

#define FIRST(...) FIRST_(__VA_ARGS__,)
#define SECOND(...) SECOND_(__VA_ARGS__,)

#define EMPTY()

#define EVAL(...) EVAL1024(__VA_ARGS__)
#define EVAL1024(...) EVAL512(EVAL512(__VA_ARGS__))
#define EVAL512(...) EVAL256(EVAL256(__VA_ARGS__))
#define EVAL256(...) EVAL128(EVAL128(__VA_ARGS__))
#define EVAL128(...) EVAL64(EVAL64(__VA_ARGS__))
#define EVAL64(...) EVAL32(EVAL32(__VA_ARGS__))
#define EVAL32(...) EVAL16(EVAL16(__VA_ARGS__))
#define EVAL16(...) EVAL8(EVAL8(__VA_ARGS__))
#define EVAL8(...) EVAL4(EVAL4(__VA_ARGS__))
#define EVAL4(...) EVAL2(EVAL2(__VA_ARGS__))
#define EVAL2(...) EVAL1(EVAL1(__VA_ARGS__))
#define EVAL1(...) __VA_ARGS__

// An eval macro that is non-recursive
#define Eval_Nr(...) Eval_Nr2(__VA_ARGS__)
#define Eval_Nr2(...) __VA_ARGS__

#define TUPLE_AT_1(a, b, ...) b
#define CHECK(...) TUPLE_AT_1(__VA_ARGS__)
#define CAT_PROBE(...), CAT_END,

#define CAT_IND() CAT_
#define CAT_(x, a, ...) CHECK(CAT_PROBE a, CAT_NEXT)(x, a, __VA_ARGS__)
#define CAT_NEXT(x, a, ...) CAT_IND EMPTY()()(x ## _ ## a, __VA_ARGS__)
#define CAT_END(x, a, ...) x

#define CAT(a,b) a ## b
#define CAT_2(...) EVAL512(CAT_(, __VA_ARGS__, ()))

#define DEFER1(m) m EMPTY()
#define DEFER2(m) m EMPTY EMPTY()()

#define IS_PROBE(...) SECOND(__VA_ARGS__, 0)
#define PROBE() ~, 1

#define NOT(x) IS_PROBE(CAT(_NOT_, x))
#define _NOT_0 PROBE()

#define BOOL(x) NOT(NOT(x))

#define IF_ELSE(condition) _IF_ELSE(BOOL(condition))
#define _IF_ELSE(condition) CAT(_IF_, condition)

#define _IF_1(...) __VA_ARGS__ _IF_1_ELSE
#define _IF_0(...)             _IF_0_ELSE

#define _IF_1_ELSE(...)
#define _IF_0_ELSE(...) __VA_ARGS__

#define HAS_ARGS(...) BOOL(FIRST(_END_OF_ARGUMENTS_ __VA_ARGS__)())
#define _END_OF_ARGUMENTS_() 0

#define MAP(m, first, ...)           \
  m(first);                           \
  IF_ELSE(HAS_ARGS(__VA_ARGS__))(    \
    DEFER2(_MAP)()(m, __VA_ARGS__)   \
  )(                                 \
    /* Do nothing, just terminate */ \
  )
#define _MAP() MAP

#define MAP_Args(m, args, first, ...)           \
  m(args, first);                           \
  IF_ELSE(HAS_ARGS(__VA_ARGS__))(    \
    DEFER2(_MAP_Args)()(m, args, __VA_ARGS__)   \
  )(                                 \
    /* Do nothing, just terminate */ \
  )
#define _MAP_Args() MAP_Args

#define MAP_No_Colon(m, first, ...)           \
  m(first)                           \
  IF_ELSE(HAS_ARGS(__VA_ARGS__))(    \
    DEFER2(_MAP_No_Colon)()(m, __VA_ARGS__)   \
  )(                                 \
    /* Do nothing, just terminate */ \
  )
#define _MAP_No_Colon() MAP_No_Colon

#define MAP_Arg_Group(m, first, ...)           \
  m first;                           \
  IF_ELSE(HAS_ARGS(__VA_ARGS__))(    \
    DEFER2(_MAP_Arg_Group)()(m, __VA_ARGS__)   \
  )(                                 \
    /* Do nothing, just terminate */ \
  )
#define _MAP_Arg_Group() MAP

// Works for up to 1024 parameters in the __VA_ARGS__ argument.
#define Apply(fn, ...) EVAL(MAP(fn, __VA_ARGS__))

// Works for up to 1024 parameters in the __VA_ARGS__ argument.
// Use this when you want to operate on groups of arguments: function((int, bool), (char. "text")).
#define Apply_Arg_Group(fn, ...) EVAL(MAP_Arg_Group(fn, __VA_ARGS__))

// Works for up to 1024 parameters in the __VA_ARGS__ argument.
#define Apply_No_Colon(fn, ...) EVAL(MAP_No_Colon(fn, __VA_ARGS__))

// Works for up to 1024 parameters in the __VA_ARGS__ argument.
#define Apply_Args(fn, args, ...) EVAL(MAP_Args(fn, args, __VA_ARGS__))

// Credits: https://github.com/JacksonAllan/CC/blob/main/cc.h
// Do find CC_MAKE_LVAL_COPY( ty, xp ) on the page.
#define CFT_Lval_Copy(type, expression) *(type[1]){expression}