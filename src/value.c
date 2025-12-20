/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include <stdlib.h>
#include "value.h"

#include <string.h>

#include "parser.h"

/*
 * Creates a new value for an integer
 */
Value* create_int_value(const long int_value) {
    Value* value;

    value = malloc(sizeof(Value));
    if (!value) {
        return NULL;
    }

    value->type = VALUE_INT;
    value->data.int_value = int_value;
    return value;
}

/*
 * Creates a new value for a string
 */
Value* create_string_value(const char* string_value) {
    Value* value;
    char* text;
    size_t len;

    value = malloc(sizeof(Value));
    if (!value) {
        return NULL;
    }

    len = strlen(string_value) + 1;
    text = malloc(len);
    if (!text) {
        free(value);
        return NULL;
    }
    strcpy(text, string_value);

    value->type = VALUE_STRING;
    value->data.string_value = text;
    return value;
}

/*
 * Creates a new value for a list
 */
Value* create_list_value(Node* node) {
    Value* value;

    value = malloc(sizeof(Value));
    if (!value) {
        return NULL;
    }

    value->type = VALUE_LIST;
    value->data.list_node = create_node_copy(node); /* Deep copy of a node */

    if (!value->data.list_node) {
        free(value);
        return NULL;
    }

    return value;
}

/*
 * Creates a NIL value
 */
Value* create_nil_value(void) {
    Value* value;

    value = malloc(sizeof(Value));
    if (!value) {
        return NULL;
    }

    value->type = VALUE_NIL;
    return value;
}

/*
 * Creates a copy of value
 */
Value* create_value_copy(const Value* value) {
    Value* copy;

    switch (value->type) {
        case VALUE_INT:
            copy = create_int_value(value->data.int_value);
            break;
        case VALUE_STRING:
            copy = create_string_value(value->data.string_value);
            break;
        case VALUE_LIST:
            copy = create_list_value(value->data.list_node);
            break;
        case VALUE_NIL:
            copy = create_nil_value();
            break;
        default:
            return NULL;
    }
    return copy;

}

/*
 * Frees allocated memory inside a Value structure
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
