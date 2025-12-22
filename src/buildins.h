/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_BUILDINS_H
#define C_SW_BUILDINS_H
#include "value.h"

/* Enum for types of build in functions */
typedef enum {
    /* ==========================================================
     * Section for special form operators (eval.c)
     * They work directly with unevaluated arguments from the AST.
     * ========================================================== */

    /* Special operators  */
    BI_QUOTE,   /* quote, ' */
    BI_SET,     /* set */
    BI_INC,     /* inc */
    BI_DEC,     /* dec */
    BI_QUIT,    /* quit */

    /* Control operators */
    BI_IF,      /* if */
    BI_WHILE,   /* while */
    BI_BRK,     /* brk */

    /* ==========================================================
     * Section for standard build-in functions (buildins.c)
     * They work with already evaluated arguments.
     * ========================================================== */

    /* Arithmetic operators */
    BI_ADD,     /* + */
    BI_SUB,     /* - */
    BI_MUL,     /* * */
    BI_DIV,     /* / */
    BI_MAX,     /* max */
    BI_MIN,     /* min */

    /* Relation operators */
    BI_EQ,      /* = */
    BI_NEQ,     /* /= */
    BI_LT,      /* < */
    BI_GT,      /* > */
    BI_LTE,     /* <= */
    BI_GTE,     /* >= */

    /* Operators for lists and atoms */
    BI_LIST,    /* list */
    BI_ATOM,    /* atom */
    BI_CAR,     /* car */
    BI_CDR,     /* cdr */
    BI_NTH,     /* nth */
    BI_LENGTH,  /* length */

    /* Interaction operators */
    BI_PRINT,   /* print */

    /* Unknown type of build-in function */
    BI_UNKNOWN
} BuildinType;

/**
 * Get type of primitive function by its name
 * @param name First element of a list - name of the primitive function
 * @return Type of primitive function
 */
BuildinType get_buildin_type(const char* name);

/**
 * Lisp addition primitive function (+)
 * @param args Array of argument values
 * @param args_count Number of arguments
 * @return Result value of the addition
 */
Value* prim_add(Value** args, int args_count);

/**
 * Lisp subtraction primitive function (-)
 * @param args Array of argument values
 * @param args_count Number of arguments
 * @return Result value of the subtraction
 */
Value* prim_sub(Value** args, int args_count);

/**
 * Lisp multiplication primitive function (*)
 * @param args Array of argument values
 * @param args_count Number of arguments
 * @return Result value of the multiplication
 */
Value* prim_mul(Value** args, int args_count);

/**
 * Lisp division primitive function (/)
 * Division is rounded only to whole numbers. - Could be improved in the future.
 * @param args Array of argument values
 * @param args_count Number of arguments
 * @return Result value of the division
 */
Value* prim_div(Value** args, int args_count);



#endif /* C_SW_BUILDINS_H */
