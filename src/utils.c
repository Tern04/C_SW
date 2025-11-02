/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"

void skip_whitespace(char** input) {
    while (**input != '\0' && isspace(**input)) {
        (*input)++;
    }
}

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

void program_cleanup(char* file_content) {
    free(file_content);

}
