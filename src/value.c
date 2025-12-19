/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "value.h"
#include <stdlib.h>

#include "parser.h"

/*
 * Frees allocated memory inside a Value structure
 * Only string values require a cleanup
 */
void value_cleanup(Value* value) {
    if (!value) {
        return;
    }

    switch (value->type) {
        case VALUE_INT:
        case VALUE_NIL:
            return;
        case VALUE_STRING:
            free(value->data.string_value);
            value->data.string_value = NULL;
            break;
        case VALUE_LIST:
            if (value->data.list_node) {
                node_cleanup(value->data.list_node);
                value->data.list_node = NULL;
            }
            break;
        default:
            break;

    }

}
