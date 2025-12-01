/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"

/*
 * Skips all whitespaces in the input
 */
void skip_whitespace(char** input) {
    while (**input != '\0' && isspace(**input)) {
        (*input)++;
    }
}
/*
 * Print a token's type and value for debugging
 */
void print_token(Token token) {
    printf("Token type: ");
    switch(token.type) {
        case TOKEN_SYMBOL:
            printf("SYMBOL");
            break;
        case TOKEN_NUMBER:
            printf("NUMBER (value: %ld)", token.number_value);
            break;
        case TOKEN_STRING:
            printf("STRING");
            break;
        case TOKEN_LBRACKET:
            printf("LBRACKET");
            break;
        case TOKEN_RBRACKET:
            printf("RBRACKET");
            break;
        case TOKEN_ERROR:
            printf("ERROR");
            break;
        case TOKEN_END:
            printf("END");
            break;
    }
    printf(", text: \"%s\"\n", token.text ? token.text : "NULL");
}

/*
 * Frees the rest of the allocated memory
 */
void program_cleanup(char* file_content) {
    free(file_content);
}

/**
 * Print the AST node for debugging
 */
void print_node(Node* node) {
    int i;

    if (!node) {
        printf("NULL");
        return;
    }

    switch (node->type) {
        case NODE_INT:
            printf("%ld", node->value.int_value);
            break;

        case NODE_STRING:
            printf("\"%s\"", node->value.text_value);
            break;

        case NODE_SYMBOL:
            printf("%s", node->value.text_value);
            break;

        case NODE_LIST:
            printf("(");
            for (i = 0; i < node->value.list.count; i++) {
                if (i > 0) {
                    printf(" ");
                }
                print_node(node->value.list.children[i]);
            }
            printf(")");
            break;

        default:
            printf("UNKNOWN_NODE");
            break;
    }
}
