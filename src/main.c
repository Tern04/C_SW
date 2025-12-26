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
#include "eval.h"

typedef enum {
    MODE_INTERACTIVE,
    MODE_BATCH,
    MODE_VERBOSE_BATCH,
    MODE_ERROR
} ProgramMode;

void run_interactive_mode(void) {
    char input_line[1024];
    Tokenizer tokenizer;
    Node* ast;
    Env* env;
    Value* result;

    /* Initialize and set up env */
    env = create_env();
    if (!env) return;
    setup_env(env);

    printf("--- LISP INTERPRETER INTERACTIVE MODE ---\n");

    /* Start an interactive loop */
    while (1) {
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
            printf("Error: Invalid expression or unknown characters\n");
            tokenizer_cleanup(&tokenizer); /* Cleanup tokenizer */
            continue;
        }

        result = eval(env, ast); /* Evaluate the AST */

        /* Print the result */
        if (result) {
            print_value(result);
            printf("\n");

            /* Free the result value */
            value_cleanup(result);
            free(result);
        }

        /* Free the AST */
        node_cleanup(ast);
        tokenizer_cleanup(&tokenizer);

    }
    env_cleanup(env); /* Cleanup environment */
    printf("--- EXITING INTERACTIVE MODE ---\n");
}

void run_batch_modes(char* file_content, ProgramMode mode) {
    Tokenizer tokenizer;
    Node* ast;
    Env* env;
    Value* value;

    env = create_env();
    if (!env) {
        return;
    }
    setup_env(env);

    if (mode == MODE_BATCH) {
        printf("--- LISP INTERPRETER BATCH MODE ---\n");
    } else {
        printf("--- LISP INTERPRETER VERBOSE BATCH MODE ---\n");
    }


    tokenizer_init(&tokenizer, file_content);

    while (1) {
        /* Parsing */
        ast = parse_expression(&tokenizer);

        /* Nothing to parse - break */
        if (!ast) {
            break;
        }

        value = eval(env, ast); /* Evaluate the AST */

        /* If there is a value - print it in verbose mode and free it in both modes*/
        if (value) {
            if (mode == MODE_VERBOSE_BATCH) {
                print_value(value);
                printf("\n");
            }

            value_cleanup(value);
            free(value);
        }
        node_cleanup(ast);
    }
    /* Free tokenizer and environment */
    tokenizer_cleanup(&tokenizer);
    env_cleanup(env);
    printf("--- FINISHED ---\n");
}

ProgramMode setup(const int argc, char* argv[], const char** input_file) {
    struct stat buffer;
    *input_file = NULL;

    if (argc == 1) {
        return MODE_INTERACTIVE;
    }

    if (argc == 2) {
        *input_file = argv[1];
        if (stat(*input_file, &buffer) != 0)
            return MODE_ERROR;
        return MODE_BATCH;
    }

    if (argc == 3 && strcmp(argv[2], "-v") == 0) {
        *input_file = argv[1];
        if (stat(*input_file, &buffer) != 0)
            return MODE_ERROR;
        return MODE_VERBOSE_BATCH;
    }

    return MODE_ERROR;
}

int main(const int argc, char* argv[]) {
    const char* input_file = NULL;
    char* file_content = NULL;
    ProgramMode mode;

    mode = setup(argc, argv, &input_file);

    switch (mode) {
        case MODE_INTERACTIVE:
            run_interactive_mode();
            break;

        case MODE_BATCH:
        case MODE_VERBOSE_BATCH:
            file_content = load_content_from_file(input_file);
            if (file_content) {
                run_batch_modes(file_content, mode);
                free(file_content);
            } else {
                fprintf(stderr, "Error: Could not load file %s\n", input_file);
                return 1;
            }
            break;

        default:
            fprintf(stderr, "Usage: %s [input_file] [-v]\n", argv[0]);
            return 1;
    }

    return 0;
}
