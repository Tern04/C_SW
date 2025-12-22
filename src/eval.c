/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include <stddef.h>

#include "eval.h"

#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "primitives.h"
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
    PrimitivesType type;
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

    /* Get type of primitive function by the first element in the list */
    type = get_primitive_type(first_elem->value.text_value);

    switch (type) {
        /* To be implemented in primitives.c/h */
        case PRIM_ADD:
            args = handle_arguments(env, node, &args_count);
            result = prim_add(args, args_count);
            arguments_cleanup(args, args_count);
            return result;
        case PRIM_SUB:
            args = handle_arguments(env, node, &args_count);
            result = prim_sub(args, args_count);
            arguments_cleanup(args, args_count);
            return result;
        case PRIM_MUL:
            args = handle_arguments(env, node, &args_count);
            result = prim_mul(args, args_count);
            arguments_cleanup(args, args_count);
            return result;
        case PRIM_DIV:
            args = handle_arguments(env, node, &args_count);
            result = prim_div(args, args_count);
            arguments_cleanup(args, args_count);
            return result;
        case PRIM_SET:
        case PRIM_IF:
        case PRIM_WHILE:
        case PRIM_UNKNOWN:
            printf("Error: unknown primitive function\n");
            return create_nil_value();
        default:
            break;
    }
    return NULL;
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
