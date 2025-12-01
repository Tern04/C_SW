/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_UTILS_H
#define C_SW_UTILS_H

#include "tokenizer.h"
#include "s_exp.h"

/*
 * Skips all whitespaces in the input
 * @param input Text to be handled
 */
void skip_whitespace(char** input);

/*
 * Print a token's type and value for debugging
 * @param token Token for printing
 */
void print_token(Token token);

/*
 * Print the AST node for debugging
 * @param node Node to be printed
 */
void print_node(Node* node);

/*
 * Frees the rest of the allocated memory
 * @param file_content Content of the input file
 */
void program_cleanup(char* file_content);

#endif /* C_SW_UTILS_H */
