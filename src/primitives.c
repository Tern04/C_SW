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
        return create_nil_value();
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

/*
 * Lisp multiplication primitive function (*)
 */
Value* prim_mul(Value** args, int args_count) {
    Value* result;
    long product;
    int i;

    /* Check for at least one argument */
    if (args_count == 0) {
        printf("Error: * expects at least one argument\n");
        return create_nil_value();
    }

    product = 1; /* Initialize product to 1 */

    for (i = 0; i < args_count; i++) {

        /* Check for integer arguments */
        if (args[i]->type != VALUE_INT) {
            printf("Error: * expects integer arguments\n");
            return create_nil_value();
        }
        product *= args[i]->data.int_value; /* Multiply the integer values */
    }

    result = create_int_value(product); /* Create a new value for the result */
    return result;
}

/*
 * Lisp division primitive function (/)
 * Division is rounded only to whole numbers. - could be improved in the future.
 */
Value* prim_div(Value** args, int args_count) {
    Value* result;
    long quotient;
    int i;

    if (args_count < 2) {
        printf("Error: / expects at least two argument\n");
        return create_nil_value();
    }

    quotient = args[0]->data.int_value;

    for (i = 1; i < args_count; i++) {

        /* Check for integer arguments */
        if (args[i]->type != VALUE_INT) {
            printf("Error: / expects integer arguments\n");
            return create_nil_value();
        }

        /* Check for division by zero */
        if (args[i]->data.int_value == 0) {
            printf("Error: division by zero\n");
            return create_nil_value();
        }

        quotient /= args[i]->data.int_value; /* Divide the integer values - the result will be a whole number */
    }
    result = create_int_value(quotient);
    return result;

}
