/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "s_exp.h"

#include <stdlib.h>
#include <string.h>

/*
 * Create a new node with an integer value
 */
Node* create_int_node(long value) {
    Node* node = malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }
    node->type = NODE_INT;
    node->value.int_value = value;
    return node;
}

/*
 * Creates a new node with a string literal
 */
Node* create_string_node(const char* text) {
    Node* node = malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }
    node->type = NODE_STRING;
    node->value.text_value = strdup(text);
    return node;
}

/*
 * Creates a new node with a symbol value
 */
Node* create_symbol_node(const char* text) {
    Node* node = malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }
    node->type = NODE_SYMBOL;
    node->value.text_value = strdup(text);
    return node;
}

/*
 * Creates a new empty list node
 */
Node* create_list_node(void) {
    Node* node = malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }
    node->type = NODE_LIST;
    node->value.list.children = NULL;
    node->value.list.count = 0;
    return node;
}


