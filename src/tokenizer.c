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
    size_t saved_index;
    Token saved_current;
    Token token;

    saved_index= tokenizer->index;
    saved_current= tokenizer->current_token;
    token = tokenizer_get_token(tokenizer);


    tokenizer->index = saved_index;
    tokenizer->current_token = saved_current;

    return token;
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
        case '\'':
            /* Apostrophe => quote symbol */
            return TOKEN_SYMBOL;
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
    char* start;
    char* end;
    size_t length;

    switch (token->type) {
        case TOKEN_ERROR:
            exit(-1);
        case TOKEN_END:
            break;
        case TOKEN_LBRACKET:
        case TOKEN_RBRACKET:
            /* Single character tokens */
            token->text = malloc(2);
            token->text[0] = tokenizer->input[tokenizer->index];
            token->text[1] = '\0';
            tokenizer->index++;
            break;
        case TOKEN_STRING:
            /* Parse string literal */
            tokenizer->index++; /* Skip opening quote */
            start = &tokenizer->input[tokenizer->index];
            end = start;
            while (*end != '"' && *end != '\0') {
                end++;
            }
            if (*end == '\0') {
                token->type = TOKEN_ERROR;
                return;
            }
            length = end - start;
            token->text = malloc(length + 1);
            strncpy(token->text, start, length);
            token->text[length] = '\0';
            tokenizer->index = (end - tokenizer->input) + 1; /* Skip closing quote */
            break;
        case TOKEN_NUMBER:
            /* Parse number */
            start = &tokenizer->input[tokenizer->index];
            token->number_value = strtol(start, &end, 10);
            length = end - start;
            token->text = malloc(length + 1);
            strncpy(token->text, start, length);
            token->text[length] = '\0';
            tokenizer->index = end - tokenizer->input;
            break;
        case TOKEN_SYMBOL:
            /* Check if this is the apostrophe quote macro */
            if (tokenizer->input[tokenizer->index] == '\'') {
                /* Replace apostrophe with "quote" symbol */
                token->text = malloc(6); /* "quote" + null terminator */
                strcpy(token->text, "quote");
                tokenizer->index++;
            } else {
                /* Parse normal symbol/identifier */
                start = &tokenizer->input[tokenizer->index];
                end = start;
                while (*end != '\0' && !isspace(*end) && *end != '(' && *end != ')' && *end != '"') {
                    end++;
                }
                length = end - start;
                token->text = malloc(length + 1);
                strncpy(token->text, start, length);
                token->text[length] = '\0';
                tokenizer->index = end - tokenizer->input;
            }
            break;
        default:
            tokenizer->index++;
            break;
    }
}



Token tokenizer_get_token(Tokenizer* tokenizer) {
    Token token;
    char* input_ptr;

    token.text = NULL;
    token.number_value = 0;

    /* Skip whitespace first */
    input_ptr = &tokenizer->input[tokenizer->index];
    skip_whitespace(&input_ptr);
    tokenizer->index = input_ptr - tokenizer->input;

    token.type = tokenizer_process_token_type(tokenizer);

    tokenizer_process_token_by_type(tokenizer, &token);

    return token;

}

void token_cleanup(Token* token) {
    if (token && token->text) {
        free(token->text);
        token->text = NULL;
    }
}

void tokenizer_cleanup(Tokenizer* tokenizer) {
    if (tokenizer->current_token.text) {
        free(tokenizer->current_token.text);
        tokenizer->current_token.text = NULL;
    }

}
