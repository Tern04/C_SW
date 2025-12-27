
#ifndef C_SW_UTILS_H
#define C_SW_UTILS_H

/* Forward declaration of structures */
struct Node; /* AST node */
struct Value; /* Value structure */

/**
 * Skips all whitespaces in the input
 * @param input Text to be handled
 */
void skip_whitespace(char** input);

/**
 * Sets the input text to uppercase
 * @param input Text to be set to the uppercase
 */
void string_to_uppercase(char* input);

/**
 * Checks whether the AST node represents a PRINT call
 * Used in the main.c for verbose and interactive mode
 * @param ast AST node to be checked
 * @return 1 if it is a PRINT call, 0 otherwise
 */
int is_print_call(const struct Node* ast);

/**
 * Print the AST node for printing function
 * @param node Node to be printed
 */
void print_node(struct Node* node);

/**
 * Prints the passed value for prints in all interpret modes
 * @param value value to be printed
 */
void print_value(const struct Value* value);

#endif /* C_SW_UTILS_H */
