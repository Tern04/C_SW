
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "s_exp.h"
#include "tokenizer.h"
#include "value.h"
#include "utils.h"

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
 * Print the AST node for printing function
 */
void print_node(Node* node) {
    int i;

    /* Check for NULL node */
    if (!node) {
        printf("NIL");
        return;
    }

    switch (node->type) {
        case NODE_INT:
            /* Print integer value */
            printf("%ld", node->value.int_value);
            break;
        case NODE_STRING:
            /* Print string value */
            printf("\"%s\"", node->value.text_value);
            break;
        case NODE_SYMBOL:
            /* Print symbol value */
            printf("%s", node->value.text_value);
            break;
        case NODE_LIST:
            /* Print list of nodes */
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
 * Prints the passed value for prints in all modes
 */
void print_value(const Value* value) {
    /* Check for NULL value */
    if (!value) {
        printf("NIL");
        return;
    }

    /* Print based on the value type */
    switch (value->type) {
        case VALUE_NIL:
            /* Print NIL representation */
            printf("NIL");
            break;
        case VALUE_T:
            /* Print True representation */
            printf("T");
            break;
        case VALUE_BREAK:
            /* Print Break representation */
            printf("BREAK");
            break;
        case VALUE_INT:
            /* Print integer value */
            printf("%ld", value->data.int_value);
            break;
        case VALUE_STRING:
            /* Print string value with quotes */
            printf("\"%s\"", value->data.string_value);
            break;
        case VALUE_SYMBOL:
            /* Print symbol name without quotes */
            printf("%s", value->data.string_value);
            break;
        case VALUE_LIST:
            /* Print the list using the node printer */
            print_node(value->data.list_node);
            break;
        default:
            /* Fallback for unknown value types */
            printf("UNKNOWN_VALUE");
            break;
    }
}
