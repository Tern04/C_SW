/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * Sets the input text to uppercase
 */
void string_to_uppercase(char* input) {
    char current_char;
    int ascii_offset;

    ascii_offset = 'a' - 'A'; /* Difference between lowercase and uppercase in ASCII */

    if (input == NULL) {
        return;
    }

    /* Iterate through each character in the string */
    while (*input != '\0') {
        current_char = *input;

        /* Check if the character is a lowercase letter */
        if (current_char >= 'a' && current_char <= 'z') {
            *input = (char)(current_char - ascii_offset); /* Convert to uppercase */
        }
        input++;
    }
}

/**
 * Checks whether the AST node represents a PRINT call
 * Used in the main.c for verbose and interactive mode
 */
int is_print_call(const Node* ast) {
    Node* first;
    int flag;

    flag = 0; /* Default to not a PRINT call */

    /* Check if the AST is a list and has at least one element */
    if (ast && ast->type == NODE_LIST && ast->value.list.count > 0) {
        first = ast->value.list.children[0]; /* Get the first element of the list */

        /* Check if the first element is a symbol with the value "PRINT" */
        flag = (first->type == NODE_SYMBOL && strcmp(first->value.text_value, "PRINT") == 0);
    }
    return flag;
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
        case TOKEN_QUOTE:
            printf("QUOTE");
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

/**
 * Prints a value for debugging
 */
void print_value(const Value* value) {
    if (value->type == VALUE_NIL) {
        printf("NIL");
    }
    else if (value->type == VALUE_T) {
        printf("T");
    }
    else if (value->type == VALUE_BREAK) {
        printf("BREAK");
    }
    else if (value->type == VALUE_INT) {
        printf("%ld", value->data.int_value);
    }
    else if (value->type == VALUE_LIST) {
        print_node(value->data.list_node);
    }
    else if (value->type == VALUE_SYMBOL) {
        printf("%s", value->data.string_value);
    }
    else {
        printf("\"%s\"", value->data.string_value);
    }
}

/**
 * Print variables in the environment
 */
void print_env(const Env* env) {
    int i;

    printf("Variables in the environment:");
    printf("\n");

    for (i = 0; i < env->count; i++) {
        printf("%s: ", env->names[i]);
        print_value(env->values[i]);
        printf("\n");
    }

}
