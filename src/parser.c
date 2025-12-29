
#include <stdio.h>
#include <stdlib.h>

#include "s_exp.h"
#include "tokenizer.h"
#include "errors.h"
#include "parser.h"
#include "value.h"

/*
 * Parse a single expression from the tokenizer
 */
Node* parse_expression(Tokenizer* tokenizer) {
    Token token;
    Node* node;
    Node* quote_list;
    Node* quoted_expr;
    Value* err;

    node = NULL; /* Initialize node to NULL */
    token = tokenizer_get_token(tokenizer); /* Get the next token */

    /* Parse based on the token type */
    switch (token.type) {
        case TOKEN_NUMBER:
        case TOKEN_STRING:
        case TOKEN_SYMBOL:
            /* Atomic value */
            node = parse_atom(&token); /* Parse atom */
            token_cleanup(&token); /* Clean up token */
            break;
        case TOKEN_QUOTE:
            quote_list = create_list_node(); /* Create a new list node for the quote */
            add_child_to_list(quote_list, create_symbol_node("QUOTE")); /* Add the QUOTE symbol */

            /* Parse the quoted expression */
            quoted_expr = parse_expression(tokenizer);

            /* Add the quoted expression to the quote list */
            if (quoted_expr) {
                add_child_to_list(quote_list, quoted_expr);
            }
            token_cleanup(&token); /* Clean up token */
            return quote_list;
        case TOKEN_LBRACKET:
            token_cleanup(&token); /* Clean up token */
            node = parse_list(tokenizer); /* Parse list */
            break;
        case TOKEN_RBRACKET:
            token_cleanup(&token); /* Clean up token */
            return NULL;
        case TOKEN_ERROR:
            /* Handle error token */
                err = handle_error(ERR_SYNTAX_ERROR, "Invalid token");
                if (err) {
                    value_cleanup(err);
                    free(err);
                }
            token_cleanup(&token);
            return NULL;
        case TOKEN_END:
            token_cleanup(&token); /* Clean up token */
            return NULL;
        default:
            /* Unexpected token type */
                err = handle_error(ERR_SYNTAX_ERROR, "Unexpected token type");
                if (err) {
                    value_cleanup(err);
                    free(err);
                }
            token_cleanup(&token);
            break;
    }
    return node;

}

/*
 * Convert a token to an atomic node
 */
Node* parse_atom(const Token* token) {
    Node* node;

    node = NULL; /* Initialize node to NULL */

    /* Check for NULL token */
    if (!token) {
        return NULL;
    }

    /* Parse based on the token type */
    switch (token->type) {
        case TOKEN_NUMBER:
            node = create_int_node(token->number_value); /* Create integer node */
            break;

        case TOKEN_STRING:
            node = create_string_node(token->text); /* Create string node */
            break;

        case TOKEN_SYMBOL:
            node = create_symbol_node(token->text); /* Create symbol node */
            break;
        default:
            /* Unexpected token type */
            {
                Value* err = handle_error(ERR_SYNTAX_ERROR, "Unexpected atom token");
                if (err) {
                    value_cleanup(err);
                    free(err);
                }
            }
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

    list = create_list_node(); /* Create an empty list node */

    /* Parse until closing bracket */
    while (1) {
        next = tokenizer_peek(tokenizer); /* Peek at the next token */

        /* Check for closing bracket */
        if (next.type == TOKEN_RBRACKET) {
            consumed = tokenizer_get_token(tokenizer); /* Consume the closing bracket */
            token_cleanup(&next); /* Clean up peeked token */
            token_cleanup(&consumed); /* Clean up consumed token */
            break;
        }

        /* Check for the end of input */
        if (next.type == TOKEN_END) {
            Value* err = handle_error(ERR_SYNTAX_ERROR, "Missing ')'"); /* Error: missing closing bracket */
            if (err) {
                value_cleanup(err);
                free(err);
            }
            token_cleanup(&next); /* Clean up peeked token */
            node_cleanup(list); /* Free the list node */
            return NULL;
        }

        token_cleanup(&next); /* Clean up peeked token */

        child = parse_expression(tokenizer); /* Parse the next expression */

        /* Check for parsing errors */
        if (child == NULL) {
            node_cleanup(list); /* Free the list node */
            return NULL;
        }

        if (add_child_to_list(list, child) != 0) {
            node_cleanup(child);
            node_cleanup(list);
            return NULL;
        }
    }
    return list;
}

/*
 * Add a child node to the list
 */
int add_child_to_list(Node* list, Node* child) {
    Node** temp;

    /* Reallocate memory for the new child */
    temp = realloc(
        list->value.list.children,
        sizeof(Node*) * (list->value.list.count + 1)
    );

    /* Check for allocation failure */
    if (!temp) {
        Value* err = handle_error(ERR_OUT_OF_MEMORY, "Memory allocation failed in add_child_to_list");
        if (err) {
            value_cleanup(err);
            free(err);
        }
        return 1; /* Allocation failure */
    }

    /* Add the new child to the list */
    list->value.list.children = temp;
    list->value.list.children[list->value.list.count] = child;
    list->value.list.count++;

    return 0; /* Success */
}

/*
 * Frees the allocated memory of the node and its children
 */
void node_cleanup(Node* node) {
    int i;

    /* Check for NULL pointer */
    if (!node) {
        return;
    }

    /* Free based on the node type */
    switch (node->type) {
        case NODE_SYMBOL:
        case NODE_STRING:
            /* Free the text value */
            if (node->value.text_value) {
                free(node->value.text_value);
            }
            break;
        case NODE_LIST:
            /* Free all child nodes */
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

