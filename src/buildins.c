
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "value.h"
#include "utils.h"
#include "buildins.h"
#include "s_exp.h"
#include "parser.h"

/* Structure for mapping built-in function names to their types and implementations */
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
    {"+",      BI_ADD, prim_arithmetics},
    {"-",      BI_SUB, prim_arithmetics},
    {"*",      BI_MUL, prim_arithmetics},
    {"/",      BI_DIV, prim_arithmetics},
    {"MAX",    BI_MAX, prim_arithmetics},
    {"MIN",    BI_MIN, prim_arithmetics},
    {"=",      BI_EQ, prim_compare},
    {"/=",     BI_NEQ, prim_compare},
    {"<",      BI_LT, prim_compare},
    {">",      BI_GT, prim_compare},
    {"<=",     BI_LTE, prim_compare},
    {">=",     BI_GTE, prim_compare},
    {"LIST",   BI_LIST, prim_list},
    {"ATOM",   BI_ATOM, prim_atom},
    {"CAR",    BI_CAR, prim_car},
    {"CDR",    BI_CDR, prim_cdr},
    {"NTH",    BI_NTH, prim_nth},
    {"LENGTH", BI_LENGTH, prim_length},
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

    return BI_UNKNOWN; /* Unknown built-in function */
}

/*
 * Call the primitive function by its type
 */
Value* call_prim_function(const BuildinType type, Value** args, const int args_count) {
    int i;

    /* Look for the function in the table */
    for (i = 0; i < BUILTIN_COUNT; i++) {
        if (type == BUILTIN_TABLE[i].type) {
            /* Call the function if it exists */
            if (BUILTIN_TABLE[i].function != NULL) {
                return BUILTIN_TABLE[i].function(type, args, args_count);
            }
            break;
        }
    }
    return create_nil_value(); /* Return NIL on error */
}

/**
 * Lisp arithmetic functions (+, -, *, /)
 */
Value* prim_arithmetics(const BuildinType type , Value** args, const int args_count) {
    Value* result;
    long res;
    long first_arg;
    int i;

    /* Check for at least one argument */
    if (args_count == 0) {
        handle_error(ERR_SYNTAX_ERROR, "Arithmetic functions expect at least one argument");
        return create_nil_value();
    }

    /* Check that all arguments are integers */
    for (i = 0; i < args_count; i++) {
        if (args[i]->type != VALUE_INT) {
            handle_error(ERR_SYNTAX_ERROR, "Arithmetic functions expect integer arguments");
            return create_nil_value();
        }
    }

    first_arg = args[0]->data.int_value; /* Store the first argument */

    /* Perform the arithmetic operation based on the type */
    switch (type) {
        case BI_ADD:
            res = 0; /* Initialize result to 0 */

            /* Sum all arguments */
            for (i = 0; i < args_count; i++) {
                res += args[i]->data.int_value;
            }
            break;
        case BI_SUB:
            /* Handle unary negation */
            if (args_count == 1) {
                res = -first_arg;
                break;
            }

            res = first_arg; /* Start with the first argument */

            /* Subtract all subsequent arguments */
            for (i = 1; i < args_count; i++) {
                res -= args[i]->data.int_value;
            }
            break;
        case BI_MUL:
            res = 1; /* Initialize result to 1 */

            /* Multiply all arguments */
            for (i = 0; i < args_count; i++) {
                res *= args[i]->data.int_value;
            }
            break;
        case BI_DIV:
            /* Check for at least two arguments */
            if (args_count < 2) {
                handle_error(ERR_RUNTIME_ERROR, "Division needs at least two arguments");
                return create_nil_value();
            }

            res = first_arg; /* Start with the first argument */

            /* Divide by all arguments */
            for (i = 1; i < args_count; i++) {

                /* Check for division by zero */
                if (args[i]->data.int_value == 0) {
                    handle_error(ERR_RUNTIME_ERROR, "Division by zero");
                    return create_nil_value();
                }
                res /= args[i]->data.int_value;
            }
            break;
        case BI_MAX:
            res = first_arg; /* Start with the first argument */

            /* Find the maximum value */
            for (i = 1; i < args_count; i++) {
                if (args[i]->data.int_value > res) {
                    res = args[i]->data.int_value;
                }
            }
            break;
        case BI_MIN:
            res = first_arg; /* Start with the first argument */

            /* Find the minimum value */
            for (i = 1; i < args_count; i++) {
                if (args[i]->data.int_value < res) {
                    res = args[i]->data.int_value;
                }
            }
            break;
        default:
            /* Unknown operator */
            handle_error(ERR_RUNTIME_ERROR, "Unknown arithmetic operator");
            return create_nil_value();
    }
    /* Create and return the result value */
    result = create_int_value(res);
    return result;
}

/*
 * Lisp print primitive function (print)
 */
Value* prim_print(const BuildinType type, Value** args, const int args_count) {
    (void) type; /* Type of function will not be used in this method */

    /* Check for exactly one argument */
    if (args_count != 1) {
        handle_error(ERR_SYNTAX_ERROR, "PRINT expects exactly 1 argument");
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

/*
 * Lisp comparison primitive function (=, /=, <, >, <=, >=)
 */
Value* prim_compare(const BuildinType type, Value** args, const int args_count) {
    long a;
    long b;
    int res;
    int i;
    int j;

    /* Check for at least two arguments */
    if (args_count == 0) {
        handle_error(ERR_SYNTAX_ERROR, "Comparison operators expect at least one argument");
        return create_nil_value();
    }

    /* A single argument is always true */
    if (args_count == 1) {
        return create_t_value(); /* A single argument is always true */
    }

    /* Check that all arguments are integers */
    for (i = 0; i < args_count; i++) {
        if (args[i]->type != VALUE_INT) {
            handle_error(ERR_SYNTAX_ERROR, "Comparison operators expect integer arguments");
            return create_nil_value();
        }
    }

    /* Handle BIQ comparison type */
    if (type == BI_NEQ) {

        /* Check for any equal arguments */
        for (i = 0; i < args_count - 1; i++) {
            for (j = i + 1; j < args_count; j++) {

                /* If any two arguments are equal, return NIL */
                if (args[i]->data.int_value == args[j]->data.int_value) {
                    return create_nil_value();
                }
            }
        }
        return create_t_value(); /* All arguments are different */
    }

    /* Perform the comparison for other types */
    for (i = 0; i < args_count - 1; i++) {
        a = args[i]->data.int_value; /* Get the first argument */
        b = args[i + 1]->data.int_value; /* Get the second argument */

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
                /* Unknown operator */
                handle_error(ERR_RUNTIME_ERROR, "Unknown comparison operator");
                return create_nil_value();
        }

        /* If any comparison fails, return NIL */
        if (!res) {
            return create_nil_value();
        }
    }
    return create_t_value();
}

/*
 * Lisp list primitive function (list)
 */
Value* prim_list(const BuildinType type, Value** args, const int args_count) {
    Value* result;
    Node* list;
    Node* child;
    int i;

    (void) type; /* Type of function will not be used in this method */

    list = create_list_node(); /* Create an empty list node */
    list->value.list.count = args_count; /* Set the number of children */
    list->value.list.children = malloc(sizeof(Node*) * args_count); /* Allocate memory for children */

    /* Check for allocation failure */
    if (!list->value.list.children) {
        handle_error(ERR_OUT_OF_MEMORY, "Memory allocation failed at list");
        return create_nil_value();
    }

    for (i = 0; i < args_count; i++) {
        child = create_node_from_value(args[i]);  /* Create a node from each argument value */
        list->value.list.children[i] = child; /* Add child to the list */
    }

    /* Create and return the list value */
    result = create_list_value(list);

    node_cleanup(list); /* Free the temporary list node */

    return result;
}

/*
 * Lisp atom primitive function (atom)
 */
Value* prim_atom(const BuildinType type, Value** args, const int args_count) {
    (void) type; /* Type of function will not be used in this method */

    /* Check for exactly one argument */
    if (args_count != 1) {
        handle_error(ERR_SYNTAX_ERROR, "ATOM expects exactly 1 argument");
        return create_nil_value();
    }

    /* Everything except a list is an atom */
    if (args[0]->type != VALUE_LIST) {
        return create_t_value(); /* It is an atom */
    }
    return create_nil_value(); /* It is a list, so not an atom */
}

/*
 * Lisp car primitive function (car)
 * Returns the first element of the list
 */
Value* prim_car(const BuildinType type, Value** args, const int args_count) {
    Node* list;

    (void) type; /* Type of function will not be used in this method */

    /* Check for exactly one argument which must be a list or NIL */
    if (args_count != 1 || (args[0]->type != VALUE_LIST && args[0]->type != VALUE_NIL)) {
        handle_error(ERR_SYNTAX_ERROR, "CAR expects exactly 1 argument - list");
        return create_nil_value();
    }

    /* Check for NIL which represents an empty list */
    if (args[0]->type == VALUE_NIL) {
        return create_nil_value();
    }

    list = args[0]->data.list_node; /* Get the list node */

    /* Check for the empty list */
    if (list->value.list.count == 0) {
        return create_nil_value();
    }

    return create_value_from_node(list->value.list.children[0]); /* Return first element as value */
}

/*
 * Lisp cdr primitive function (cdr)
 */
Value* prim_cdr(const BuildinType type, Value** args, const int args_count) {
    Node* list;
    Node* new_list;
    Value* result;
    int count;
    int i;

    (void) type; /* Type of function will not be used in this method */

    /* Check for exactly one argument which must be a list */
    if (args_count != 1 || (args[0]->type != VALUE_LIST && args[0]->type != VALUE_NIL)) {
        handle_error(ERR_SYNTAX_ERROR, "CDR expects exactly 1 argument - list");
        return create_nil_value();
    }

    /* Check for NIL which represents an empty list */
    if (args[0]->type == VALUE_NIL) {
        return create_nil_value();
    }

    /* Get the list node and original count */
    list = args[0]->data.list_node;
    count = list->value.list.count;

    /* If a list has less or one element - return NIL */
    if (count <= 1) {
        return create_nil_value();
    }

    new_list = create_list_node(); /* Create a new list node */
    new_list->value.list.count = count - 1; /* Set the count of the new list */
    new_list->value.list.children = malloc(sizeof(Node*) * (count - 1)); /* Allocate memory for new Node */

    /* Check for allocation failure */
    if (!new_list->value.list.children) {
        handle_error(ERR_OUT_OF_MEMORY, "Memory allocation failed at cdr");

        /* Free the created list */
        free(new_list);

        return create_nil_value();
    }

    /* Copy all elements except the first one as a deep copy */
    for (i = 1; i < count; i++) {
        new_list->value.list.children[i - 1] = create_node_copy(list->value.list.children[i]);
    }

    result = create_list_value(new_list);

    node_cleanup(new_list);

    return result;
}

/*
 * Lisp nth primitive function (nth)
 * returns the n-th element of the list
 */
Value* prim_nth(const BuildinType type, Value** args, const int args_count) {
    Node* list;
    int index;

    (void) type; /* Type of function will not be used in this method */

    /* Check for exactly two arguments: integer and list OR NIL */
    if (args_count != 2 || args[0]->type != VALUE_INT ||
        (args[1]->type != VALUE_LIST && args[1]->type != VALUE_NIL)) {
        handle_error(ERR_SYNTAX_ERROR, "NTH expects exactly 2 arguments - integer and list");
        return create_nil_value();
       }

    /* Check for NIL which represents an empty list */
    if (args[1]->type == VALUE_NIL) {
        return create_nil_value(); /* Any index is NIL */
    }

    list = args[1]->data.list_node; /* Get the list node */
    index = (int)args[0]->data.int_value; /* Get the index - convert long to int */

    /* Check for the valid index */
    if (index < 0 || index >= list->value.list.count) {
        handle_error(ERR_RUNTIME_ERROR, "NTH index out of bounds");
        return create_nil_value();
    }

    return create_value_from_node(list->value.list.children[index]); /* Return n-th element as a value */
}

/*
 * Lisp length primitive function (length)
 */
Value* prim_length(const BuildinType type, Value** args, const int args_count) {
    (void) type; /* Type of function will not be used in this method */

    /* Check for exactly one argument which must be a list or NIL */
    if (args_count != 1 || (args[0]->type != VALUE_LIST && args[0]->type != VALUE_NIL)) {
        handle_error(ERR_SYNTAX_ERROR, "LENGTH expects exactly 1 argument - list");
        return create_nil_value();
    }

    /* Check for NIL which represents an empty list */
    if (args[0]->type == VALUE_NIL) {
        return create_int_value(0); /* Return length 0 for NIL */
    }

    return create_int_value(args[0]->data.list_node->value.list.count); /* Return the length of the list */
}

