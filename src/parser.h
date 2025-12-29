
#ifndef C_SW_PARSER_H
#define C_SW_PARSER_H

struct Tokenizer; /* Forward declaration of Tokenizer structure */
struct Token; /* Forward declaration of Token structure */
struct Node; /* Forward declaration of Node structure */

/**
 * Parse a single expression from the tokenizer
 * @param tokenizer Token reader
 * @return Parsed expression
 */
struct Node* parse_expression(struct Tokenizer* tokenizer);

/**
 * Convert a token to an atomic node
 * @param token Token to be converted
 * @return Parsed node
 */
struct Node* parse_atom(const struct Token* token);

/**
 * Parse a list - expression
 * @param tokenizer Token reader
 * @return Node of the parsed AST tree
 */
struct Node* parse_list(struct Tokenizer* tokenizer);

/**
 * Add a child node to the list
 * @param list list node to which the child will be added
 * @param child child node to be added to the parent node
 * @return 0 on success, 1 on failure
 */
int add_child_to_list(struct Node* list, struct Node* child);

/**
 * Frees the allocated memory of the node and its children
 * @param node Node to be freed
 */
void node_cleanup(struct Node* node);

#endif /* C_SW_PARSER_H */
