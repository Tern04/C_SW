/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "buildins.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

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
    {"=",      BI_EQ, prim_compare},
    {"/=",     BI_NEQ, prim_compare},
    {"<",      BI_LT, prim_compare},
    {">",      BI_GT, prim_compare},
    {"<=",     BI_LTE, prim_compare},
    {">=",     BI_GTE, prim_compare},
    {"LIST",   BI_LIST, NULL},
    {"ATOM",   BI_ATOM, NULL},
    {"CAR",    BI_CAR, NULL},
    {"CDR",    BI_CDR, NULL},
    {"NTH",    BI_NTH, NULL},
    {"LENGTH", BI_LENGTH, NULL},
    {"PRINT",  BI_PRINT, prim_print}
};

/* Calculate the number of built-in functions in the table */
static const int BUILTIN_COUNT = sizeof(BUILTIN_TABLE) / sizeof(BuildinMapping);

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
                return BUILTIN_TABLE[i].function(type, args, args_count);
            }
            break;
        }
    }
    return create_nil_value();
}


/*
 * Lisp addition primitive function (+)
 */
Value* prim_add(const BuildinType type, Value** args, const int args_count) {
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
Value* prim_sub(const BuildinType type, Value** args, const int args_count) {
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
Value* prim_mul(const BuildinType type, Value** args, const int args_count) {
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
Value* prim_div(const BuildinType type, Value** args, const int args_count) {
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

Value* prim_print(const BuildinType type, Value** args, const int args_count) {
    (void) type; /* Type of function will not be used in this method */

    /* Check for exactly one argument */
    if (args_count != 1) {
        printf("Error: PRINT expects exactly 1 argument\n");
        return create_nil_value();
    }

    /* Print with a function from utils.c */
    if (args[0]) {
        print_value(args[0]);
        printf("\n"); /* Add newline after printing */
    }

    /* Return printed value */
    return create_value_copy(args[0]);
}

Value* prim_compare(const BuildinType type, Value** args, const int args_count) {
    long a;
    long b;
    int res;
    int i;
    int j;

    /* Check for at least two arguments */
    if (args_count == 0) {
        printf("Error: Comparison operators expect at least one argument\n");
        return create_nil_value();
    }

    /* A single argument is always true */
    if (args_count == 1) {
        return create_t_value(); /* A single argument is always true */
    }

    /* Check that all arguments are integers */
    for (i = 0; i < args_count; i++) {
        if (args[i]->type != VALUE_INT) {
            printf("Error: Comparison operators expect integer arguments\n");
            return create_nil_value();
        }
    }

    /* Handle BIQ comparison type */
    if (type == BI_NEQ) {
        for (i = 0; i < args_count - 1; i++) {
            for (j = i + 1; j < args_count; j++) {
                if (args[i]->data.int_value == args[j]->data.int_value) {
                    return create_nil_value();
                }
            }
        }
        return create_t_value();
    }

    for (i = 0; i < args_count - 1; i++) {
        a = args[i]->data.int_value;
        b = args[i + 1]->data.int_value;

        switch (type) {
            case BI_EQ:
                res = a == b;
                break;
            case BI_LT:
                res = a < b;
                break;
            case BI_LTE:
                res = a <= b;
                break;
            case BI_GT:
                res = a > b;
                break;
            case BI_GTE:
                res = a >= b;
                break;
            default:
                printf("Error: Unknown comparison operator\n");
                return create_nil_value();
        }
        if (!res) {
            return create_nil_value();
        }
    }
    return create_t_value();
}
