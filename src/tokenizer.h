/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_TOKENIZER_H
#define C_SW_TOKENIZER_H

typedef enum {
    TOKEN_SYMBOL,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_ERROR,
    TOKEN_END
}TokenType;

typedef struct {
    TokenType type;
    char* text;
    long number_value;
}Token;

typedef struct {
    const char* input;
    int index;
    Token current_token;
}Tokenizer;

void tokenizer_init(Tokenizer* tokenizer, const char* input);
Token tokenizer_peek(Tokenizer* tokenizer);
Token tokenizer_get_token(Tokenizer* tokenizer);
void tokenizer_cleanup(Tokenizer* tokenizer);


/*
* Myšlenka - Vstup -> tokenizer: rozdělí input -> parser postaví node stromu -> vyhodnocení ve value
*/
#endif /* C_SW_TOKENIZER_H */
