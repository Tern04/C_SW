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
    char* input;
    int index;
    Token current_token;
}Tokenizer;

void tokenizer_init(Tokenizer* tokenizer, char* input);
Token tokenizer_peek(Tokenizer* tokenizer);
TokenType tokenizer_process_token_type(Tokenizer* tokenizer);
void tokenizer_process_token_by_type(Tokenizer* tokenizer, Token* token);
Token tokenizer_get_token(Tokenizer* tokenizer);
void token_cleanup(Token* token);
void tokenizer_cleanup(Tokenizer* tokenizer);


/*
* Myšlenka - Vstup -> tokenizer: rozdělí input -> parser postaví node stromu -> vyhodnocení ve value
*/
#endif /* C_SW_TOKENIZER_H */
