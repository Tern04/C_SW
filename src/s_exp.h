/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_S_EXP_H
#define C_SW_S_EXP_H

/* Types of node */
typedef enum {
    NODE_INT, /* Integer value */
    NODE_STRING, /* String literal */
    NODE_LIST, /* List of nodes - expressions */
    NODE_SYMBOL /* Symbol - variable or function name */
}NodeType;

/* AST node structure */
typedef struct Node {
    NodeType type; /* Type of the node */
    union { /* Value of the node - depends on the type */
        long int_value; /* Integer value */
        char* text_value; /* String or symbol value */
        struct { /* List of nodes */
            struct Node** children; /* Array of child nodes */
            int count; /* Counter for children nodes */
        }list;
    }value;
}Node;

/**
 * Create a new node with an integer value
 * @param value Value of the node
 * @return Created node
 */
Node* create_int_node(long value);

/**
 * Creates a new node with a string literal
 * @param text Value of the node
 * @return Created node
 */
Node* create_string_node(const char* text);

/**
 * Creates a new node with a symbol value
 * @param text Value of the node
 * @return Created node
 */
Node* create_symbol_node(const char* text);

/**
 * Creates a new empty list node
 * @return Nothing
 */
Node* create_list_node(void);

#endif /* C_SW_S_EXP_H */
