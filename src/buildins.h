/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_BUILDINS_H
#define C_SW_BUILDINS_H
#include "value.h"

/* Enum for types of build in functions */
typedef enum {
    BI_SET, /* set */
    BI_IF, /* if */
    BI_WHILE, /* while */
    BI_UNKNOWN, /* Unknown type */

    PRIM_ADD, /* + */
    PRIM_SUB, /* - */
    PRIM_MUL, /* * */
    PRIM_DIV /* / */
}BuildinType;

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
