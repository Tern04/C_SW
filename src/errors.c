
#include <stdio.h>
#include <stdlib.h>

#include "value.h"
#include "errors.h"

/*
 * Print an error message and return an error Value
 */
Value* handle_error(const ErrorType error_type, const char* details) {
    const char* msg;

    msg = "Unknown error"; /* Default message */

    /* Determine the error message based on the error type */
    switch (error_type) {
        case ERR_NO_ERROR:
            break;
        case ERR_INVALID_INPUT_FILE:
            msg = "Invalid input file or file not found. Usage";
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
    /* Print the error message to stderr */
    fprintf(stderr, "ERROR %d\n", error_type);
    fprintf(stderr, "%s\n", msg);

    /* Print additional details if provided */
    if (details) {
        fprintf(stderr, "Details: %s\n", details);
    }

    return create_error_value(error_type);

}
