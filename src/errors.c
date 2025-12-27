/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "errors.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Print an error message based on the error type
 */
void handle_error(const ErrorType error_type, const char* details) {
    const char* msg = "Unknown error";

    switch (error_type) {
        case ERR_NO_ERROR:
            return;
        case ERR_INVALID_INPUT_FILE:
            msg = "Invalid input file or file not found";
            break;
        case ERR_SYNTAX_ERROR:
            msg = "Syntax error in Lisp source";
            break;
        case ERR_FILE_ACCESS_FAILURE:
            msg = "File access failure (file is inaccessible)";
            break;
        case ERR_OUT_OF_MEMORY:
            msg = "Out of memory (failed to allocate memory)";
            break;
        case ERR_RUNTIME_ERROR:
            msg = "Runtime error during evaluation";
            break;
    }

    fprintf(stderr, "ERROR %d\n", error_type);
    fprintf(stderr, "%s\n", msg);
    if (details) {
        fprintf(stderr, "Details: %s\n", details);
    }

    exit(error_type);

}
