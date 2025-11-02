/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_UTILS_H
#define C_SW_UTILS_H

#include "tokenizer.h"

void skip_whitespace(char** input);
void print_token(Token token);
void program_cleanup(char* file_content);

#endif /* C_SW_UTILS_H */
