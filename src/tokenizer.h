/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_TOKENIZER_H
#define C_SW_TOKENIZER_H

#include <stddef.h>

/* Token types that can be recognized */
typedef enum TokenType{
    TOKEN_SYMBOL, /* Variable or function name */
    TOKEN_NUMBER, /* Numbers */
    TOKEN_STRING, /* Text in  quotation marks*/
    TOKEN_LBRACKET, /* '(' */
    TOKEN_RBRACKET, /* ')' */
    TOKEN_QUOTE, /* Quote symbol ' */
    TOKEN_ERROR, /* Error at the tokenization */
    TOKEN_END /* End of input */
}TokenType;

/* Structure for token representation */
typedef struct Token{
    TokenType type; /* Type of token */
    char* text; /* Copy of token text representation */
    long number_value; /* Number if the token has it */
}Token;

/* Structure for handling tokenization */
typedef struct Tokenizer{
    char* input; /* Whole input string */
    size_t index; /* Current position in the input string */
    Token current_token; /* Last token that was handled */
}Tokenizer;

/**
 * Initialize the tokenizer and set him to the beginning of the input
 * @param tokenizer Token reader
 * @param input Input string with Lisp code (content of a file / content of a console)
 */
void tokenizer_init(Tokenizer* tokenizer, char* input);

/**
 * Checks the next token (without moving in the text)
 * @param tokenizer Token reader
 * @return The next token
 */
Token tokenizer_peek(Tokenizer* tokenizer);

/**
 * Recognize the type of the current token
 * @param tokenizer Token reader
 * @return Type of current token
 */
TokenType tokenizer_process_token_type(Tokenizer* tokenizer);

/**
 * Process the current token by its type - handle its content
 * @param tokenizer Token reader
 * @param token Pointer to current token
 */
void tokenizer_process_token_by_type(Tokenizer* tokenizer, Token* token);

/**
 * Gets the next token from the input (moves in the text)
 * @param tokenizer Token reader
 * @return The next token in the input
 */
Token tokenizer_get_token(Tokenizer* tokenizer);

/**
 * Helper function to tokenize brackets
 * @param tokenizer Token reader
 * @param token Token to be processed
 */
void tokenize_bracket(Tokenizer* tokenizer, Token* token);

/**
 * Helper function to tokenize strings
 * @param tokenizer Token reader
 * @param token Token to be processed
 */
void tokenize_string(Tokenizer* tokenizer, Token* token);

/**
 * Helper function to tokenize numbers
 * @param tokenizer Token reader
 * @param token Token to be processed
 */
void tokenize_number(Tokenizer* tokenizer, Token* token);

/**
 * Helper function to tokenize symbols
 * @param tokenizer Token reader
 * @param token Token to be processed
 */
void tokenize_symbol(Tokenizer* tokenizer, Token* token);

/**
 * Frees the allocated memory of the token
 * @param token Token to be freed
 */
void token_cleanup(Token* token);

/**
 * Frees the allocated memory of the tokenizer structure
 * @param tokenizer Token reader
 */
void tokenizer_cleanup(Tokenizer* tokenizer);


#endif /* C_SW_TOKENIZER_H */
