/*
* Created by Tomáš Rybák on 03.11.2025.
*/

#ifndef C_SW_PARSER_H
#define C_SW_PARSER_H

#include "tokenizer.h"
#include "s_exp.h"

/**
 * Parse a single expression from the tokenizer
 * @param tokenizer Token reader
 * @return Parsed expression
 */
Node* parse_expression(Tokenizer* tokenizer);

/**
 * Convert a token to an atomic node
 * @param token Token to be converted
 * @return Parsed node
 */
Node* parse_atom(Token token);

/**
 * Parse a list - expression
 * @param tokenizer Token reader
 * @return Node of the parsed AST tree
 */
Node* parse_list(Tokenizer* tokenizer);

/**
 * Add a child node to the list
 * @param list list nodo to which the child will be added
 * @param child child node to be added to the parent node
 */
void add_child_to_list(Node* list, Node* child);

/**
 * Frees the allocated memory of the node and its children
 * @param node Node to be freed
 */
void node_cleanup(Node* node);

#endif /* C_SW_PARSER_H */
