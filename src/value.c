/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "s_exp.h"
#include "value.h"


/*
 * Creates a new value for an integer
 */
Value* create_int_value(const long int_value) {
    Value* value;

    value = malloc(sizeof(Value)); /* Allocate memory for the value structure */

    /* Check for allocation failure */
    if (!value) {
        return NULL;
    }

    value->type = VALUE_INT; /* Set the type to VALUE_INT */
    value->data.int_value = int_value; /* Set the integer value */

    return value;
}

/*
 * Creates a new value for a string
 */
Value* create_string_value(const char* string_value) {
    Value* value;
    char* text;
    size_t len;

    value = malloc(sizeof(Value)); /* Allocate memory for the value structure */

    /* Check for allocation failure */
    if (!value) {
        return NULL;
    }

    len = strlen(string_value) + 1; /* +1 for null terminator */
    text = malloc(len); /* Allocate memory for the string */

    /* Check for allocation failure */
    if (!text) {
        free(value); /* Free the value structure */
        return NULL;
    }
    strcpy(text, string_value); /* Copy the string value */

    value->type = VALUE_STRING; /* Set the type to VALUE_STRING */
    value->data.string_value = text; /* Set the string value */

    return value;
}

/*
 * Creates a new value for a symbol
 */
Value* create_symbol_value(const char* symbol_value) {
    Value* value;
    char* text;
    size_t len;

    value = malloc(sizeof(Value)); /* Allocate memory for the value structure */

    /* Check for allocation failure */
    if (!value) {
        return NULL;
    }

    len = strlen(symbol_value) + 1; /* +1 for null terminator */
    text = malloc(len); /* Allocate memory for the string */

    /* Check for allocation failure */
    if (!text) {
        free(value); /* Free the value structure */
        return NULL;
    }
    strcpy(text, symbol_value); /* Copy the symbol name */

    value->type = VALUE_SYMBOL; /* Set the type to VALUE_SYMBOL */
    value->data.string_value = text; /* Set the string name */

    return value;
}

/*
 * Creates a new value for a list
 */
Value* create_list_value(Node* node) {
    Value* value;

    value = malloc(sizeof(Value)); /* Allocate memory for the value structure */

    /* Check for allocation failure */
    if (!value) {
        return NULL;
    }

    value->type = VALUE_LIST; /* Set the type to VALUE_LIST */
    value->data.list_node = create_node_copy(node); /* Deep copy of a node */

    /* Check for allocation failure */
    if (!value->data.list_node) {
        free(value); /* Free the value structure */
        return NULL;
    }

    return value;
}

/*
 */
Value* create_break_value(void) {
    Value* value;

    value = malloc(sizeof(Value)); /* Allocate memory for the value structure */

    /* Check for allocation failure */
    if (!value) {
        return NULL;
    }
    value->type = VALUE_BREAK; /* Set the type to VALUE_BREAK */

    return value;
}

/**
 * Creates a T value - True
 */
Value* create_t_value(void) {
    Value* value;

    value = malloc(sizeof(Value)); /* Allocate memory for the value structure */

    /* Check for allocation failure */
    if (!value) {
        return NULL;
    }

    value->type = VALUE_T; /* Set the type to VALUE_T */

    return value;
}

/*
 * Creates a NIL value
 */
Value* create_nil_value(void) {
    Value* value;

    value = malloc(sizeof(Value)); /* Allocate memory for the value structure */

    /* Check for allocation failure */
    if (!value) {
        return NULL;
    }
    value->type = VALUE_NIL; /* Set the type to VALUE_NIL */

    return value;
}

/*
 * Creates a copy of value
 */
Value* create_value_copy(const Value* value) {
    Value* copy;

    /* Create a copy based on the type */
    switch (value->type) {
        case VALUE_INT:
            copy = create_int_value(value->data.int_value); /* Create int value */
            break;
        case VALUE_STRING:
            copy = create_string_value(value->data.string_value); /* Create string value */
            break;
        case VALUE_SYMBOL:
            copy = create_symbol_value(value->data.string_value);
            break;
        case VALUE_LIST:
            copy = create_list_value(value->data.list_node); /* Create list value */
            break;
        case VALUE_BREAK:
            copy = create_break_value();
        case VALUE_T:
            copy = create_t_value(); /* Create T value */
            break;
        case VALUE_NIL:
            copy = create_nil_value(); /* Create NIL value */
            break;
        default:
            return NULL; /* Unknown type */
    }
    return copy;
}

/*
 * Creates a Value from a Node
 */
Value* create_value_from_node(Node* node) {

    /* Check for NULL pointer */
    if (!node) {
        return create_nil_value();
    }

    switch (node->type) {
        case NODE_INT:
            return create_int_value(node->value.int_value); /* Create int value */
        case NODE_STRING:
            return create_string_value(node->value.text_value); /* Create string value */
        case NODE_SYMBOL:
            return create_symbol_value(node->value.text_value);
        case NODE_LIST:
            return create_list_value(node); /* Create list value */
        default:
            return create_nil_value(); /* Unsupported node type */
    }

}

/*
 * Frees allocated memory inside a Value structure
 */
void value_cleanup(Value* value) {
    /* Check for NULL pointer */
    if (!value) {
        return;
    }

    /* Free based on the type */
    switch (value->type) {
        case VALUE_INT:
        case VALUE_BREAK:
        case VALUE_T:
        case VALUE_NIL:
            return; /* No allocated memory to free */
        case VALUE_STRING:
        case VALUE_SYMBOL:
            /* Free the string value */
            free(value->data.string_value);
            value->data.string_value = NULL;
            break;
        case VALUE_LIST:
            /* Free the list node */
            if (value->data.list_node) {
                node_cleanup(value->data.list_node);
                value->data.list_node = NULL;
            }
            break;
        default:
            break;

    }

}
