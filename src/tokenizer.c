/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "tokenizer.h"

#include <stddef.h>
#include <stdlib.h>


void tokenizer_init(Tokenizer* tokenizer, const char* input) {
    tokenizer->input = input;
    tokenizer->index = 0;
    tokenizer->current_token.text = NULL;
}

Token tokenizer_peek(Tokenizer* tokenizer) {
    return tokenizer->current_token;
}

Token tokenizer_get_token(Tokenizer* tokenizer) {
    Token token;
    token.text = NULL;
    token.number_value = 0;


    return token;

}

void tokenizer_cleanup(Tokenizer* tokenizer) {
    if (tokenizer->current_token.text) {
        free(tokenizer->current_token.text);
        tokenizer->current_token.text = NULL;
    }

}
