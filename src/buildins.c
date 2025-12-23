/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "buildins.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const BuildinMapping BUILTIN_TABLE[] = {
    /* Section for special form operators (eval.c) */
    {"QUOTE", BI_QUOTE, NULL},
    {"SET",   BI_SET, NULL},
    {"INC",   BI_INC, NULL},
    {"DEC",   BI_DEC, NULL},
    {"IF",    BI_IF, NULL},
    {"WHILE", BI_WHILE, NULL},
    {"QUIT",  BI_QUIT, NULL},
    {"BRK",   BI_BRK, NULL},

    /* Section for standard build-in functions (buildins.c) */
    {"+",      BI_ADD, prim_add},
    {"-",      BI_SUB, prim_sub},
    {"*",      BI_MUL, prim_mul},
    {"/",      BI_DIV, prim_div},
    {"MAX",    BI_MAX, NULL},
    {"MIN",    BI_MIN, NULL},
    {"=",      BI_EQ, NULL},
    {"/=",     BI_NEQ, NULL},
    {"<",      BI_LT, NULL},
    {">",      BI_GT, NULL},
    {"<=",     BI_LTE, NULL},
    {">=",     BI_GTE, NULL},
    {"LIST",   BI_LIST, NULL},
    {"ATOM",   BI_ATOM, NULL},
    {"CAR",    BI_CAR, NULL},
    {"CDR",    BI_CDR, NULL},
    {"NTH",    BI_NTH, NULL},
    {"LENGTH", BI_LENGTH, NULL},
    {"PRINT",  BI_PRINT, NULL}
};

/* Calculate the number of built-in functions in the table */
const int BUILTIN_COUNT = sizeof(BUILTIN_TABLE) / sizeof(BuildinMapping);


/*
 * Get type of built-in function by its name
 */
BuildinType get_buildin_type(const char* name) {
    int i;

    /* Look for the type in the table */
    for (i = 0; i < BUILTIN_COUNT; i++) {
        if (strcmp(name, BUILTIN_TABLE[i].name) == 0) {
            return BUILTIN_TABLE[i].type;
        }
    }

    return BI_UNKNOWN;
}

/*
 * Call the primitive function by its type
 */
Value* call_prim_function(const BuildinType type, Value** args, const int args_count) {
    int i;

    for (i = 0; i < BUILTIN_COUNT; i++) {
        if (type == BUILTIN_TABLE[i].type) {
            if (BUILTIN_TABLE[i].function != NULL) {
                return BUILTIN_TABLE[i].function(args, args_count);
            }
            break;
        }
    }
    return create_nil_value();
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
        if (!args[i] || args[i]->type != VALUE_INT ) {
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
Value* prim_sub(Value** args, const int args_count) {
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
        if (!args[i] || args[i + 1]->type != VALUE_INT) {
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
Value* prim_mul(Value** args, const int args_count) {
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
        if (!args[i] || args[i]->type != VALUE_INT) {
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
Value* prim_div(Value** args, const int args_count) {
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
        if (!args[i] || args[i]->type != VALUE_INT) {
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
