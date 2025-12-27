/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_ERRORS_H
#define C_SW_ERRORS_H

/* Types of errors */
typedef enum ErrorType {
    ERR_NO_ERROR = 0, /* No error */
    ERR_INVALID_INPUT_FILE = 1, /* Invalid input file */
    ERR_SYNTAX_ERROR = 2, /* Syntax error in the source code */
    ERR_FILE_ACCESS_FAILURE = 3, /* Failure to access a file */
    ERR_OUT_OF_MEMORY = 4, /* Out of the memory error */
    ERR_RUNTIME_ERROR = 5 /* Runtime error during evaluation */
}ErrorType;

/**
 * Print an error message based on the error type and exits the program
 * @param error_type Type of the error
 * @param details Details about the error from the context
 */
void handle_error(ErrorType error_type, const char* details);

#endif /* C_SW_ERRORS_H */
