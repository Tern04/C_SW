

#include <stdio.h>
#include <stdlib.h>

#include "s_exp.h"
#include "tokenizer.h"
#include "parser.h"


/*
 * Parse a single expression from the tokenizer
 */
Node* parse_expression(Tokenizer* tokenizer) {
    Token token;
    Node* node;
    Node* quote_list;
    Node* quoted_expr;

    node = NULL;
    token = tokenizer_get_token(tokenizer);

    switch (token.type) {
        case TOKEN_NUMBER:
        case TOKEN_STRING:
            node = parse_atom(&token);
            token_cleanup(&token);
            break;
        case TOKEN_QUOTE:
            quote_list = create_list_node();
            add_child_to_list(quote_list, create_symbol_node("QUOTE"));

            quoted_expr = parse_expression(tokenizer);
            if (quoted_expr) {
                add_child_to_list(quote_list, quoted_expr);
            }
            token_cleanup(&token);
            return quote_list;
        case TOKEN_SYMBOL:
            /* Normal symbol */
            node = parse_atom(&token);
            token_cleanup(&token);
            break;
        case TOKEN_LBRACKET:
            token_cleanup(&token);
            node = parse_list(tokenizer);
            break;
        case TOKEN_RBRACKET:
            token_cleanup(&token);
            return NULL;
        case TOKEN_ERROR:
            printf("Syntax error, invalid token\n");
            token_cleanup(&token);
            return NULL;
        case TOKEN_END:
            token_cleanup(&token);
            return NULL;
        default:
            printf("Unexpected token type\n");
            token_cleanup(&token);
            break;
    }

    if (node == NULL) {
        printf("Parse error, value of node is NULL\n");
    }

    return node;

}

/*
 * Convert a token to an atomic node
 */
Node* parse_atom(const Token* token) {
    Node* node;
    node = NULL;

    /* Check for NULL token */
    if (!token) {
        return NULL;
    }

    switch (token->type) {
        case TOKEN_NUMBER:
            node = create_int_node(token->number_value);
            break;

        case TOKEN_STRING:
            node = create_string_node(token->text);
            break;

        case TOKEN_SYMBOL:
            node = create_symbol_node(token->text);
            break;

        default:
            fprintf(stderr, "Syntax error: unexpected atom token\n");
            break;
    }

    return node;
}

/*
 * Parse a list - expression
 */
Node* parse_list(Tokenizer* tokenizer) {
    Node* child;
    Node* list;
    Token next;
    Token consumed;

    list = create_list_node();

    while (1) {
        next = tokenizer_peek(tokenizer);
        if (next.type == TOKEN_RBRACKET) {
            consumed = tokenizer_get_token(tokenizer);
            token_cleanup(&next);
            token_cleanup(&consumed);
            break;
        }
        if (next.type == TOKEN_END) {
            fprintf(stderr, "Syntax error: missing ')'\n");
            token_cleanup(&next);
            return NULL;
        }
        token_cleanup(&next);

        child = parse_expression(tokenizer);
        if (child) {
            add_child_to_list(list, child);
        }
    }

    return list;
}

/*
 * Add a child node to the list
 */
void add_child_to_list(Node* list, Node* child) {
    Node** temp;

    temp = realloc(
        list->value.list.children,
        sizeof(Node*) * (list->value.list.count + 1)
    );

    if (!temp) {
        fprintf(stderr, "Memory allocation failed in add_child_to_list\n");
        return;
    }

    list->value.list.children = temp;
    list->value.list.children[list->value.list.count] = child;
    list->value.list.count++;
}

/*
 * Frees the allocated memory of the node and its children
 */
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

