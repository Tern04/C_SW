/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "utils.h"

#include <ctype.h>

void skip_whitespace(char** input) {
    while (**input != '\0' && isspace(**input)) {
        (*input)++;
    }
}

