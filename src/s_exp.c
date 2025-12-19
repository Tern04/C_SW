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

/*
 * Create a copy of a node for Environment
 */
Node* create_node_copy(Node* node) {
    int i;
    Node* node_copy;
    char* text;
    size_t text_len;

    node_copy = malloc(sizeof(Node)); /* Allocate memory for node*/

    /* Check for allocation failure */
    if (!node_copy) {
        return NULL;
    }
    node_copy->type = node->type; /* Copy the type of the node */

    /* Copy the value based on the type */
    switch (node->type) {
        case NODE_INT:
            node_copy->value.int_value = node->value.int_value; /* Copy just an integer value */
            break;
        case NODE_STRING:
        case NODE_SYMBOL:
            /* Allocate memory for text value */
            text_len = strlen(node->value.text_value) + 1;
            text = malloc(text_len);

            /* Check for allocation failure */
            if (!text) {
                /* Free everything allocated */
                free(node_copy);
                free(text);
                return NULL;
            }
            /* Copy the text value */
            strcpy(text, node->value.text_value);
            node_copy->value.text_value = text;
            break;
        case NODE_LIST:
            node_copy->value.list.count = node->value.list.count; /* Copy children count */

            /* Empty list */
            if (node_copy->value.list.count == 0) {
                node_copy->value.list.children = NULL; /* No children to copy - empty list */
                break;
            }

            /* Allocate memory for children nodes */
            node_copy->value.list.children = malloc(node->value.list.count * sizeof(Node*));

            /* Check for allocation failure */
            if (!node_copy->value.list.children) {
                free(node_copy);
                return NULL;
            }

            for (i = 0; i < node->value.list.count; i++) {
                node_copy->value.list.children[i] = create_node_copy(node->value.list.children[i]); /* Create copy of all children nodes */
            }
            break;
        default:
            break;
    }

    return node_copy;

}


