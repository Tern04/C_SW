/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "tokenizer.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


void tokenizer_init(Tokenizer* tokenizer, char* input) {
    tokenizer->input = input;
    tokenizer->index = 0;
    tokenizer->current_token.text = NULL;
}

Token tokenizer_peek(Tokenizer* tokenizer) {
    return tokenizer->current_token;
}

TokenType tokenizer_process_token_type(Tokenizer* tokenizer) {
    char c;
    char next;

    c = tokenizer->input[tokenizer->index];
    next = tokenizer->input[tokenizer->index + 1];

    if (c == '\0') {
        return TOKEN_END;
    }

    switch (c) {
        case '(':
            return TOKEN_LBRACKET;
        case ')':
            return TOKEN_RBRACKET;
        case '"':
            return TOKEN_STRING;
        default:
            /* Check for positive or negative numbers*/
            if (isdigit(c) || (c == '-' && isdigit(next))) {
                return TOKEN_NUMBER;
            }
            if (isalpha(c) || strchr("+-*/<>=", c)) {
                return TOKEN_SYMBOL;
            }
            return TOKEN_ERROR;
    }
}

void tokenizer_process_token_by_type(Tokenizer* tokenizer, Token* token) {
    switch (token->type) {
        case TOKEN_ERROR:
            exit(-1);
        case TOKEN_END:
            break;

    }
}



Token tokenizer_get_token(Tokenizer* tokenizer) {
    Token token;
    char* current;
    char t;
    token.text = NULL;
    token.number_value = 0;

    current = &tokenizer->input[tokenizer->index];
    skip_whitespace(&current);

    token.type = tokenizer_process_token_type(tokenizer);

    tokenizer_process_token_by_type(tokenizer, &token);

    return token;

}

void tokenizer_cleanup(Tokenizer* tokenizer) {
    if (tokenizer->current_token.text) {
        free(tokenizer->current_token.text);
        tokenizer->current_token.text = NULL;
    }

}
