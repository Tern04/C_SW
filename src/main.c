
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
#include "s_exp.h"
#include "value.h"

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
 * @return 0 on success, error code on failure
 */
int run_interactive_mode(void) {
    char input_line[1024];
    Tokenizer tokenizer;
    Node* ast;
    Env* env;
    Value* result;
    int was_printed;
    int exit_code = 0;

    env = create_env(); /* Initialize env */

    /* Check for memory allocation failure */
    if (!env) {
        result = handle_error(ERR_OUT_OF_MEMORY, "Failed to allocate memory for the environment");
        if (result) {
            exit_code = (int)result->data.int_value;
            value_cleanup(result);
            free(result);
        }
        return exit_code;
    }

    if (setup_env(env) != 0) { /* Setup global variables */
        env_cleanup(env);
        return ERR_OUT_OF_MEMORY;
    }

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
            result = handle_error(ERR_SYNTAX_ERROR, "Invalid expression or unknown characters");
            if (result) {
                value_cleanup(result);
                free(result);
            }
            tokenizer_cleanup(&tokenizer); /* Cleanup tokenizer */
            continue;
        }

        was_printed = is_print_call(ast); /* Check if the AST is a PRINT call */

        result = eval(env, ast); /* Evaluate the AST */

        /* Print the result if it was not printed by PRINT */
        if (result) {

            /* Check for QUIT value to exit */
            if (result->type == VALUE_QUIT) {
                /* Free the result value and the AST*/
                value_cleanup(result);
                free(result);
                node_cleanup(ast);
                break;
            }

            /* Check for ERROR value */
            if (result->type == VALUE_ERROR) {
                exit_code = (int)result->data.int_value;
                value_cleanup(result);
                free(result);
                node_cleanup(ast);
                tokenizer_cleanup(&tokenizer);
                break;
            }

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

    return exit_code;
}

/*
 * Run the batch and verbose batch modes of the interpreter
 * Input is read from a file and processed
 * In verbose mode, results are printed after each evaluation
 * In batch mode, only PRINT outputs are shown
 * @return 0 on success, error code on failure
 */
int run_batch_modes(char* file_content, const ProgramMode mode) {
    Tokenizer tokenizer;
    Node* ast;
    Env* env;
    Value* result;
    Token token;
    int was_printed;
    int exit_code = 0;

    env = create_env(); /* Initialize env */

    /* Check for memory allocation failure */
    if (!env) {
        result = handle_error(ERR_OUT_OF_MEMORY, "Failed to allocate memory for the environment");
        if (result) {
            exit_code = (int)result->data.int_value;
            value_cleanup(result);
            free(result);
        }
        return exit_code;
    }

    if (setup_env(env) != 0) { /* Setup global variables */
        env_cleanup(env);
        return ERR_OUT_OF_MEMORY;
    }

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

        /* Check for parsing errors */
        if (!ast) {
            token = tokenizer_get_token(&tokenizer);

            /* If we reached the end of the file, exit the loop */
            if (token.type == TOKEN_END) {
                token_cleanup(&token);
                break;
            }

            /* If there was an error during parsing, handle it */
            token_cleanup(&token);
            result = handle_error(ERR_SYNTAX_ERROR, "Failed to parse input file");
            if (result) {
                exit_code = (int)result->data.int_value;
                value_cleanup(result);
                free(result);
            }
            break;
        }

        was_printed = is_print_call(ast); /* Check if the AST is a PRINT call */

        result = eval(env, ast); /* Evaluate the AST */

        if (result) {

            /* Check for QUIT value to exit */
            if (result->type == VALUE_QUIT) {
                /* Free the result value and the AST*/
                value_cleanup(result);
                free(result);
                node_cleanup(ast);
                break;
            }

            /* Check for ERROR value */
            if (result->type == VALUE_ERROR) {
                exit_code = (int)result->data.int_value;
                value_cleanup(result);
                free(result);
                node_cleanup(ast);
                break;
            }

            /* In verbose modes, print the value unless it was already printed by PRINT */
            if (mode == MODE_INTERACTIVE || mode == MODE_VERBOSE_BATCH) {

                /* Print the result if it was not printed by PRINT */
                if (!was_printed) {
                    print_value(result);
                    printf("\n");
                }
            }

            /* Free the evaluated value */
            value_cleanup(result);
            free(result);

        }

        node_cleanup(ast); /* Free the AST */
    }

    /* Free tokenizer and environment */
    tokenizer_cleanup(&tokenizer);
    env_cleanup(env);

    /* Print exit message */
    printf("--- FINISHED ---\n");

    return exit_code;
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
 * @return 0 on success, error code on failure
 */
int run(const char* program_name, const ProgramMode mode, const char* input_file) {
    char* file_content = NULL;
    int exit_code = 0;
    Value* error_result;

    /* Switch based on the selected mode */
    switch (mode) {
        case MODE_INTERACTIVE:
            exit_code = run_interactive_mode(); /* Run interactive mode */
            break;
        case MODE_BATCH:
        case MODE_VERBOSE_BATCH:

            /* Load content from the input file */
            file_content = load_content_from_file(input_file);

            /* Check for loading errors */
            if (!file_content) {
                error_result = handle_error(ERR_INVALID_INPUT_FILE, input_file);
                if (error_result) {
                    exit_code = (int)error_result->data.int_value;
                    value_cleanup(error_result);
                    free(error_result);
                }
                return exit_code;
            }
            /* Run batch or verbose batch mode */
            exit_code = run_batch_modes(file_content, mode);

            free(file_content); /* Free the loaded file content */
            break;
        default:
            /* Error case */
            fprintf(stderr, "Usage: %s [input_file] [-v]\n", program_name);
            return 1;
    }
    return exit_code;
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

    mode = setup(argc, argv, &input_file); /* Setup the program mode */

    /* Handle error mode */
    if (mode == MODE_ERROR) {
        fprintf(stderr, "Usage: %s [input_file] [-v]\n", argv[0]);
        return 1;
    }

    /* Run the interpreter in the selected mode */
    return run(argv[0], mode, input_file);
}
