/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "eval.h"

#include <stddef.h>

Value* eval(Env* env, Node* node) {
    switch (node->type) {
        case NODE_INT:
            /* Create int value */
            break;
        case NODE_STRING:
            /* Create string value */
            break;
        case NODE_SYMBOL:
            /* Lookup symbol in the environment */
            break;
        case NODE_LIST:
            /* Evaluate list - function application or special form */
            break;
        default:
            return NULL;
    }
}
