/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_S_EXP_H
#define C_SW_S_EXP_H

struct Value; /* Forward declaration of Value structure */

/* Types of node */
typedef enum NodeType{
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

/**
 * Create a copy of a node for Environment
 * @param node node to be copied
 * @return Copy of a node
 */
Node* create_node_copy(Node* node);

/**
 * Creates a node from a value based on its type
 * @param value value to be converted
 * @return Created node
 */
Node* create_node_from_value(struct Value* value);

#endif /* C_SW_S_EXP_H */
