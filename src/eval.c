/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include <stddef.h>

#include "eval.h"

#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "buildins.h"
#include "value.h"

/*
 * Main evaluation function
 */
Value* eval(const Env* env, const Node* node) {
    Value* value;

    switch (node->type) {
        case NODE_INT:
            /* Create int value */
            value = create_int_value(node->value.int_value);
            break;
        case NODE_STRING:
            /* Create string value */
            value = create_string_value(node->value.text_value);
            break;
        case NODE_SYMBOL:
            /* Lookup symbol in the environment */
            value = env_get_value(env, node->value.text_value);
            if (!value) {
                printf("Error: symbol '%s' is not defined\n", node->value.text_value);
                return create_nil_value();
            }
            return create_value_copy(value); /* Return a copy of the found value */
        case NODE_LIST:
            /* Evaluate the list */
            value = eval_list(env, node);
            break;
        default:
            return NULL;
    }
    return value;
}

/*
 * Evaluates the list based on its first element - primitive function
 */
Value* eval_list(const Env* env, const Node* node) {
    BuildinType type;
    Node* first_elem;
    Value* result;
    Value** args;
    int args_count;

    /* Empty list  */
    if (node->value.list.count == 0) {
        return create_nil_value();
    }

    /* Check if the first element is valid */
    first_elem = node->value.list.children[0];
    if (first_elem->type != NODE_SYMBOL) {
        printf("Error: first element of a list must be a symbol\n");
        return create_nil_value();
    }

    /* Get a type of primitive function by the first element in the list */
    type = get_buildin_type(first_elem->value.text_value);

    switch (type) {
        /* Section 1 - special forms will be handled in eval.c */
        case BI_QUOTE:
        case BI_SET:
        case BI_INC:
        case BI_DEC:
        case BI_QUIT:
            handle_quit();
        case BI_IF:
        case BI_WHILE:
        case BI_BRK:
            /* Handling functions will be implemented - now just return NIL */
            return create_nil_value();

        /* Section 2 - standard primitive functions will be handled in buildins.c */
        case BI_ADD:
        case BI_SUB:
        case BI_MUL:
        case BI_DIV:
        case BI_MAX:
        case BI_MIN:
        case BI_EQ:
        case BI_NEQ:
        case BI_LT:
        case BI_GT:
        case BI_LTE:
        case BI_GTE:
        case BI_LIST:
        case BI_ATOM:
        case BI_CAR:
        case BI_CDR:
        case BI_NTH:
        case BI_LENGTH:
        case BI_PRINT:
            args = handle_arguments(env, node, &args_count); /* Get arguments for the primitive function */

            result = call_prim_function(type, args, args_count); /* Call the right primitive function */

            arguments_cleanup(args, args_count); /* Free the arguments */

            return result; /* Return the result from the primitive function */


        default:
            printf("Error: Unknown or unimplemented function\n");
            return create_nil_value();
    }
}

/*
 * Prepares array of evaluated argument values from node's children
 */
Value** handle_arguments(const Env* env, const Node* node, int* args_count) {
    Value** args;
    int count;
    int i;

    count = node->value.list.count - 1; /* Elements - name of a primitive function */
    *args_count = count; /* Store number of arguments */

    args = malloc(sizeof(Value*) * count); /* Allocate memory for arguments array */

    /* Check for allocation failure */
    if (!args) {
        return NULL;
    }

    for (i = 0; i < count; i++) {
        args[i] = eval(env, node->value.list.children[i + 1]); /* Evaluate each argument */
    }

    return args;
}

void handle_quit(void) {
    printf("Exiting the interpreter...\n");
    exit(0);
}

/*
 * Frees allocated memory of arguments for primitive functions
 */
void arguments_cleanup(Value** args, const int count) {
    int i;

    if (!args) {
        return;
    }

    /* Free each argument value */
    for (i = 0; i < count; i++) {
        if (args[i]) {
            value_cleanup(args[i]);
            free(args[i]);
        }
    }
    free(args);
}
