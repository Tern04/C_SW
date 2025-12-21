/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_PRIMITIVES_H
#define C_SW_PRIMITIVES_H
#include "env.h"
#include "value.h"

/* Enum for types of primitive functions */
typedef enum {
    PRIM_ADD, /* + */
    PRIM_SUB, /* - */
    PRIM_MUL, /* * */
    PRIM_DIV, /* / */
    PRIM_SET, /* set */
    PRIM_IF, /* if */
    PRIM_WHILE, /* while */
    PRIM_UNKNOWN /* Unknown type */
}PrimitivesType;

/**
 * Get type of primitive function by its name
 * @param name First element of a list - name of the primitive function
 * @return Type of primitive function
 */
PrimitivesType get_primitive_type(const char* name);

/**
 * Lisp addition primitive function (+)
 * @param args Array of argument values
 * @param args_count Number of arguments
 * @return Result value of the addition
 */
Value* prim_add(Value** args, int args_count);

/**
 * Lisp subdivision primitive function (-)
 * @param args Array of argument values
 * @param args_count Number of arguments
 * @return Result value of the addition
 */
Value* prim_sub(Value** args, int args_count);



#endif /* C_SW_PRIMITIVES_H */
