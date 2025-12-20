/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include <stddef.h>

#include "eval.h"

#include <stdio.h>

#include "env.h"
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
            if (node->value.list.count == 0) {
                /* Empty list  */
                return create_nil_value();
            }

            if (node->value.list.children[0]->type != NODE_SYMBOL) {
                printf("Error: first element of a list must be a symbol\n");
                return create_nil_value();
            }



            value = NULL;
            break;
        default:
            return NULL;
    }
    return value;
}
