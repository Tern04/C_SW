/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "primitives.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Get type of primitive function by its name
 */
PrimitivesType get_primitive_type(const char* name) {
    if (strcmp(name, "+") == 0) return PRIM_ADD;
    if (strcmp(name, "-") == 0) return PRIM_SUB;
    if (strcmp(name, "*") == 0) return PRIM_MUL;
    if (strcmp(name, "/") == 0) return PRIM_DIV;
    if (strcmp(name, "set") == 0) return PRIM_SET;
    if (strcmp(name, "if") == 0) return PRIM_IF;
    if (strcmp(name, "while") == 0) return PRIM_WHILE;
    return PRIM_UNKNOWN;
}

/*
 * Lisp addition primitive function (+)
 */
Value* prim_add(Value** args, const int args_count) {
    Value* result;
    long sum;
    int i;

    /* Check for at least one argument */
    if (args_count == 0) {
        printf("Error: + expects at least one argument\n");
        return create_nil_value();
    }

    sum = 0; /* Initialize sum to 0 */

    for (i = 0; i < args_count; i++) {

        /* Check for integer arguments */
        if (args[i]->type != VALUE_INT ) {
            printf("Error: + expects integer arguments\n");
            exit(1);
        }
        sum += args[i]->data.int_value; /* Sum the integer values */
    }

    result = create_int_value(sum); /* Create a new value for the result */
    return result;

}

/*
 * Lisp subdivision primitive function (-)
 */
Value* prim_sub(Value** args, int args_count) {
    Value* result;
    long diff;
    int i;

    /* Check for at least one argument */
    if (args_count == 0) {
        printf("Error: - expects at least one argument\n");
        exit(1);
    }

    /* Unary negation */
    if (args_count == 1) {
        return create_int_value(-args[0]->data.int_value);
    }

    diff = args[0]->data.int_value; /* Initialize difference with the first argument */

    for (i = 0; i < args_count - 1; i++) {

        /* Check for integer arguments */
        if (args[i + 1]->type != VALUE_INT) {
            printf("Error: - expects integer arguments\n");
            return create_nil_value();
        }
        diff -= args[i + 1]->data.int_value; /* Subtract the integer values */
    }

    result = create_int_value(diff); /* Create a new value for the result */
    return result;
}
