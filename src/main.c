/*
* Created by Tomáš Rybák on 19.10.2025.
*/
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "file_io.h"
#include "utils.h"
#include "tokenizer.h"
#include "parser.h"
#include "env.h"
#include "errors.h"
#include "eval.h"

/* Enum for program modes */
typedef enum {
    MODE_INTERACTIVE,
    MODE_BATCH,
    MODE_VERBOSE_BATCH,
    MODE_ERROR
} ProgramMode;

/*
 * Run the interactive mode of the interpreter
 * This mode allows users to input expressions line by line
 * After evaluation of each expression, the result is printed
 */
void run_interactive_mode(void) {
    char input_line[1024];
    Tokenizer tokenizer;
    Node* ast;
    Env* env;
    Value* result;
    int was_printed;

    /* Initialize env */
    env = create_env();

    /* Check for memory allocation failure */
    if (!env) {
        handle_error(ERR_OUT_OF_MEMORY, "Failed to allocate memory for the environment");
        return;
    }

    setup_env(env); /* Setup global variables */

    /* Print mode header */
    printf("--- LISP INTERPRETER INTERACTIVE MODE ---\n");

    /* Start an interactive loop */
    while (1) {
        /* Print prompt */
        printf("> ");
        fflush(stdout);

        /* Read the input line */
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }

        /* Remove trailing newline */
        input_line[strcspn(input_line, "\n")] = '\0';

        /* Skip empty lines */
        if (strlen(input_line) == 0) {
            continue;
        }

        /* Parsing */
        tokenizer_init(&tokenizer, input_line);
        ast = parse_expression(&tokenizer);

        /* Check for parsing errors */
        if (!ast) {
            handle_error(ERR_SYNTAX_ERROR, "Invalid expression or unknown characters");
            tokenizer_cleanup(&tokenizer); /* Cleanup tokenizer */
            continue;
        }

        was_printed = is_print_call(ast); /* Check if the AST is a PRINT call */

        result = eval(env, ast); /* Evaluate the AST */

        /* Print the result if it was not printed by PRINT */
        if (result) {

            /* In interactive mode, always print the result unless it was printed by PRINT */
            if (!was_printed) {
                print_value(result);
                printf("\n");
            }

            /* Free the result value */
            value_cleanup(result);
            free(result);
        }

        /* Free the AST */
        node_cleanup(ast);
        tokenizer_cleanup(&tokenizer);

    }
    env_cleanup(env); /* Cleanup environment */

    /* Print exit message */
    printf("--- EXITING INTERACTIVE MODE ---\n");
}

/*
 * Run the batch and verbose batch modes of the interpreter
 * Input is read from a file and processed
 * In verbose mode, results are printed after each evaluation
 * In batch mode, only PRINT outputs are shown
 */
void run_batch_modes(char* file_content, const ProgramMode mode) {
    Tokenizer tokenizer;
    Node* ast;
    Env* env;
    Value* value;
    Token token;
    int was_printed;

    /* Initialize env */
    env = create_env();

    /* Check for memory allocation failure */
    if (!env) {
        handle_error(ERR_OUT_OF_MEMORY, "Failed to allocate memory for the environment");
        return;
    }

    setup_env(env); /* Setup global variables */

    /* Print mode header */
    if (mode == MODE_BATCH) {
        printf("--- LISP INTERPRETER BATCH MODE ---\n");
    } else {
        printf("--- LISP INTERPRETER VERBOSE BATCH MODE ---\n");
    }

    tokenizer_init(&tokenizer, file_content); /* Initialize tokenizer */

    /* Main processing loop */
    while (1) {
        /* Parsing */
        ast = parse_expression(&tokenizer);

        /* Nothing to parse - break */
        if (!ast) {
            token = tokenizer_get_token(&tokenizer);

            /* If we reached the end of the file, exit the loop */
            if (token.type == TOKEN_END) {
                token_cleanup(&token);
                break;
            }
            /* If there was an error during parsing, handle it */
            token_cleanup(&token);
            handle_error(ERR_SYNTAX_ERROR, "Failed to parse input file");
        }

        was_printed = is_print_call(ast); /* Check if the AST is a PRINT call */

        value = eval(env, ast); /* Evaluate the AST */

        /* In verbose modes, print the value unless it was already printed by PRINT */
        if (mode == MODE_INTERACTIVE || mode == MODE_VERBOSE_BATCH) {

            /* Print the result if it was not printed by PRINT */
            if (!was_printed) {
                print_value(value);
                printf("\n");
            }
        }

        /* Free the evaluated value */
        value_cleanup(value);
        free(value);

        node_cleanup(ast); /* Free the AST */
    }

    /* Free tokenizer and environment */
    tokenizer_cleanup(&tokenizer);
    env_cleanup(env);

    /* Print exit message */
    printf("--- FINISHED ---\n");
}

/*
 * Set up the program mode based on command-line arguments
 * Based on the arguments, determine if the program should run in
 * interactive, batch, or verbose batch mode
 */
ProgramMode setup(const int argc, char* argv[], const char** input_file) {
    struct stat buffer;
    *input_file = NULL;

    /* Interactive mode */
    if (argc == 1) {
        return MODE_INTERACTIVE;
    }

    /* Batch mode with the input file */
    if (argc == 2) {
        *input_file = argv[1];

        /* Check if the file exists */
        if (stat(*input_file, &buffer) != 0) {
            return MODE_ERROR; /* File does not exist */
        }
        return MODE_BATCH;
    }

    /* Verbose batch mode with the input file */
    if (argc == 3 && strcmp(argv[2], "-v") == 0) {
        *input_file = argv[1];

        /* Check if the file exists */
        if (stat(*input_file, &buffer) != 0) {
            return MODE_ERROR; /* File does not exist */
        }
        return MODE_VERBOSE_BATCH;
    }
    return MODE_ERROR;
}
/*
 * Run the interpreter in the selected mode
 * Calls the appropriate function based on the mode
 */
int run(const char* program_name, const ProgramMode mode, const char* input_file) {
    char* file_content = NULL;

    /* Switch based on the selected mode */
    switch (mode) {
        case MODE_INTERACTIVE:
            run_interactive_mode(); /* Run interactive mode */
            break;
        case MODE_BATCH:
        case MODE_VERBOSE_BATCH:
            /* Load content from the input file */
            file_content = load_content_from_file(input_file);

            /* Check for loading errors */
            if (!file_content) {
                handle_error(ERR_INVALID_INPUT_FILE, input_file);
                return 1;
            }
            /* Run batch or verbose batch mode */
            run_batch_modes(file_content, mode);

            free(file_content); /* Free the loaded file content */
            break;
        default:
            /* Error case */
            fprintf(stderr, "Usage: %s [input_file] [-v]\n", program_name);
            return 1;
    }
    return 0;
}

/*
 * Main function - entry point of the program
 * Determines the mode and runs the appropriate function
 * Calls the setup function to parse command-line arguments
 * and then runs the interpreter in the selected mode
 */
int main(const int argc, char* argv[]) {
    const char* input_file = NULL;
    ProgramMode mode;

    mode = setup(argc, argv, &input_file);

    if (mode == MODE_ERROR) {
        fprintf(stderr, "Usage: %s [input_file] [-v]\n", argv[0]);
        return 1;
    }

    run(argv[0], mode, input_file);

    return 0;
}
