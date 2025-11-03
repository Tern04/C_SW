
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

Node* parse_expression(Tokenizer* tokenizer) {
    Token token;
    Node* node;

    node = NULL;

    token = tokenizer_get_token(tokenizer);

    switch (token.type) {
        case TOKEN_NUMBER:
            node = create_int_node(token.number_value);
            break;
        case TOKEN_STRING:
            node = create_string_node(token.text);
            break;
        case TOKEN_SYMBOL:
            node = create_symbol_node(token.text);
            break;
        case TOKEN_LBRACKET:
            node = parse_list(tokenizer);
            break;
        case TOKEN_RBRACKET:
            printf("Unexpected right bracket\n");
            return NULL;
        case TOKEN_ERROR:
            printf("Syntax error, invalid token\n");
            return NULL;
        default:
            printf("Unexpected token type\n");
            break;
    }

    if (node == NULL) {
        printf("Parse error, value of node is NULL");
    }

    return node;

}

void node_cleanup(Node* node) {
    int i;
    if (!node) {
        return;
    }

    switch (node->type) {
        case NODE_SYMBOL:
        case NODE_STRING:
            if (node->value.text_value) {
                free(node->value.text_value);
            }
            break;

        case NODE_LIST:
            if (node->value.list.children) {
                for (i = 0; i < node->value.list.count; i++) {
                    node_cleanup(node->value.list.children[i]);
                }
                free(node->value.list.children);
            }
            break;

        case NODE_INT:
            break;
    }

    free(node);
}

