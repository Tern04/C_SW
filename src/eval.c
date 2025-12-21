/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include <stddef.h>

#include "eval.h"

#include <stdio.h>

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




            value = NULL;
            break;
        default:
            return NULL;
    }
    return value;
}

Value* eval_list(const Env* env, const Node* node) {
    PrimitivesType type;
    Node* first_elem;

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

    /* Get type of a primitive function by the first element in the list */
    type = get_primitive_type(first_elem->value.text_value);

    switch (type) {
        /* To be implemented in primitives.c/h */
        case PRIM_ADD:
        case PRIM_SUB:
        case PRIM_MUL:
        case PRIM_DIV:
        case PRIM_SET:
        case PRIM_IF:
        case PRIM_WHILE:
        case PRIM_UNKNOWN:
            printf("Error: unknown primitive function\n");
        default:
            break;
    }


}
