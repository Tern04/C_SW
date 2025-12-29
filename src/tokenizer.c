
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "errors.h"
#include "tokenizer.h"

/*
 * Initialize the tokenizer and set him to the beginning of the input
 */
void tokenizer_init(Tokenizer* tokenizer, char* input) {
    /* Set initial values */
    tokenizer->input = input;
    tokenizer->index = 0;
    tokenizer->current_token.text = NULL;
}

/*
 * Checks the next token (without moving in the text)
 */
Token tokenizer_peek(Tokenizer* tokenizer) {
    size_t saved_index;
    Token saved_current;
    Token token;

    /* Save the current token and index */
    saved_index = tokenizer->index;
    saved_current = tokenizer->current_token;

    token = tokenizer_get_token(tokenizer); /* Look at the next token */

    /* Set the current token and index back */
    tokenizer->index = saved_index;
    tokenizer->current_token = saved_current;

    return token;
}

/*
 * Recognize the type of the current token
 */
TokenType tokenizer_process_token_type(const Tokenizer* tokenizer) {
    char c;
    char next;

    c = tokenizer->input[tokenizer->index]; /* Current character */

    /* Check for the end of input */
    if (c == '\0') {
        return TOKEN_END;
    }

    next = tokenizer->input[tokenizer->index + 1]; /* Next character */

    /* Determine the token type based on the current character */
    switch (c) {
        case '(':
            return TOKEN_LBRACKET;
        case ')':
            return TOKEN_RBRACKET;
        case '"':
            return TOKEN_STRING;
        case '\'':
            /* Apostrophe => quote symbol */
            return TOKEN_QUOTE;
        default:
            /* Check for positive or negative numbers*/
            if (isdigit(c) || (c == '-' && isdigit(next))) {
                return TOKEN_NUMBER;
            }
            /* Check for symbols */
            if (isalpha(c) || strchr("+-*/<>=", c)) {
                return TOKEN_SYMBOL;
            }
            return TOKEN_ERROR;
    }
}


/*
 * Process the current token by its type - handle its content
 */
void tokenizer_process_token_by_type(Tokenizer* tokenizer, Token* token) {
    const char* err_msg;
    size_t len;

    err_msg = "UNKNOWN_CHARACTER";
    /* Process based on the token type */
    switch (token->type) {
        case TOKEN_ERROR:
            /* Handle error token */
            len = strlen(err_msg) + 1; /* +1 for null terminator */
            token->text = malloc(len); /* Allocate memory for the error token text */

            /* Check for allocation failure */
            if (!token->text) {
                handle_error(ERR_OUT_OF_MEMORY, "Failed to allocate memory for error token text");
                return;
            }

            strcpy(token->text, err_msg); /* Copy the error message */
            tokenizer->index++; /* Move to the next character */
            break;
        case TOKEN_END:
            break;
        case TOKEN_LBRACKET:
        case TOKEN_RBRACKET:
            /* Single character tokens */
            tokenize_bracket(tokenizer, token);
            break;
        case TOKEN_STRING:
            /* Parse string literal */
            tokenize_string(tokenizer, token);
            break;
        case TOKEN_NUMBER:
            /* Parse number */
            tokenize_number(tokenizer, token);
            break;
        case TOKEN_SYMBOL:
            /* Parse symbol */
            tokenize_symbol(tokenizer, token);
            break;
        default:
            tokenizer->index++;
            break;
    }
}

/*
 * Gets the next token from the input (moves in the text)
 */
Token tokenizer_get_token(Tokenizer* tokenizer) {
    Token token;
    char* input_ptr;

    /* Initialize token */
    token.text = NULL;
    token.number_value = 0;

    /* Skip whitespace and comments */
    while (1) {
        /* Skip whitespace first */
        input_ptr = &tokenizer->input[tokenizer->index];
        skip_whitespace(&input_ptr);
        tokenizer->index = input_ptr - tokenizer->input; /* Update index */

        /* Check for comments starting with `;` */
        if (tokenizer->input[tokenizer->index] == ';') {

            /* Skip until the end of the line or end of the file */
            while (tokenizer->input[tokenizer->index] != '\0' &&
                   tokenizer->input[tokenizer->index] != '\n') {
                tokenizer->index++;
            }
            /* Continue loop to skip any whitespace after the comment */
            continue;
        }
        /* No more comments or whitespace, break out */
        break;
    }

    /* Determine the token type */
    token.type = tokenizer_process_token_type(tokenizer);

    /* Process the token based on its type */
    tokenizer_process_token_by_type(tokenizer, &token);

    return token;
}

/*
 * Helper function to tokenize brackets
 */
void tokenize_bracket(Tokenizer* tokenizer, Token* token) {
    token->text = malloc(2); /* Allocate memory for single character and null terminator */
    token->text[0] = tokenizer->input[tokenizer->index]; /* Set the bracket character */
    token->text[1] = '\0'; /* Null-terminate the string */
    tokenizer->index++; /* Move to the next character */
}

/*
 * Helper function to tokenize strings
 */
void tokenize_string(Tokenizer* tokenizer, Token* token) {
    char* start;
    char* end;
    size_t length;

    tokenizer->index++; /* Skip opening quote */
    start = &tokenizer->input[tokenizer->index]; /* Start of the string */
    end = start; /* Initialize end pointer */

    /* Loop until we find an unescaped quote or end of a string */
    while (*end != '\0') {
        if (*end == '"') {
            /* Check if this quote is escaped */
            if (end > start && *(end - 1) == '\\') {
                /* Escaped quote, continue */
                end++;
            } else {
                /* Unescaped quote, end of string */
                break;
            }
        } else {
            end++;
        }
    }

    /* Check for unterminated string */
    if (*end == '\0') {
        token->type = TOKEN_ERROR;
        return;
    }

    length = end - start; /* Calculate the length of the string */
    token->text = malloc(length + 1); /* Allocate memory for the string */
    strncpy(token->text, start, length); /* Copy the string content */
    token->text[length] = '\0'; /* Null-terminate the string */
    string_to_uppercase(token->text); /* Convert to uppercase */
    tokenizer->index = (end - tokenizer->input) + 1; /* Skip closing quote */
}

/*
 * Helper function to tokenize numbers
 */
void tokenize_number(Tokenizer* tokenizer, Token* token) {
    char* start;
    char* end;
    size_t length;

    start = &tokenizer->input[tokenizer->index]; /* Start of the number */
    token->number_value = strtol(start, &end, 10); /* Convert to long and find the end */
    length = end - start; /* Calculate the length of the number */
    token->text = malloc(length + 1);  /* Allocate memory for the number text */
    strncpy(token->text, start, length); /* Copy the number text */
    token->text[length] = '\0'; /* Null-terminate the string */
    tokenizer->index = end - tokenizer->input; /* Move index forward */
}

/*
 * Helper function to tokenize symbols
 */
void tokenize_symbol(Tokenizer* tokenizer, Token* token) {
    char* start;
    char* end;
    size_t length;

    /* Parse normal symbol/identifier */
    start = &tokenizer->input[tokenizer->index];
    end = start;
    /* Find the end of the symbol */
    while (*end != '\0' && !isspace(*end) && *end != '(' && *end != ')' && *end != '"') {
        end++;
    }

    length = end - start; /* Calculate the length of the symbol */
    token->text = malloc(length + 1); /* Allocate memory for the symbol text */
    strncpy(token->text, start, length); /* Copy the symbol text */
    token->text[length] = '\0'; /* Null-terminate the string */
    string_to_uppercase(token->text); /* Convert to uppercase */
    tokenizer->index = end - tokenizer->input; /* Move index forward */

}

/*
 * Frees the allocated memory of the token
 */
void token_cleanup(Token* token) {
    /* Check for NULL pointer */
    if (!token) {
        return;
    }

    /* Free the text if allocated */
    if (token->text) {
        free(token->text);
        token->text = NULL;
    }
}

/*
 * Frees the allocated memory of the tokenizer structure
 */
void tokenizer_cleanup(Tokenizer* tokenizer) {
    /* Check for NULL pointer */
    if (!tokenizer) {
        return;
    }
    /* Free the current token text if allocated */
    if (tokenizer->current_token.text) {
        free(tokenizer->current_token.text);
        tokenizer->current_token.text = NULL;
    }

}
