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

typedef enum {
    MODE_INTERACTIVE,
    MODE_BATCH,
    MODE_VERBOSE_BATCH,
    MODE_ERROR
} ProgramMode;

void run_interactive_mode(void) {
    char input_line[1024];
    Tokenizer tokenizer;
    Token token;
    Node* ast;

    printf("Interactive mode - enter expressions (Ctrl+C to exit):\n");

    while (1) {
        printf("lisp> ");
        fflush(stdout);

        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }

        /* Remove newline */
        input_line[strcspn(input_line, "\n")] = '\0';

        if (strlen(input_line) == 0) {
            continue;
        }

        /* First tokenize and print tokens */
        tokenizer_init(&tokenizer, input_line);
        printf("Tokens: \n");
        do {
            token = tokenizer_get_token(&tokenizer);
            print_token(token);
            token_cleanup(&token);
        } while (token.type != TOKEN_END && token.type != TOKEN_ERROR);
        tokenizer_cleanup(&tokenizer);

        /* Then parse and print AST */
        tokenizer_init(&tokenizer, input_line);
        ast = parse_expression(&tokenizer);

        if (ast) {
            printf("AST: ");
            print_node(ast);
            printf("\n\n");
            node_cleanup(ast);
        } else {
            printf("Parse error\n\n");
        }

        tokenizer_cleanup(&tokenizer);
    }
}

void run_batch_mode(char* file_content) {
    Tokenizer tokenizer;
    Token token;
    Node* ast;

    printf("Batch mode - processing file...\n");
    printf("File content: %s\n\n", file_content);

    /* First tokenize and print all tokens */
    tokenizer_init(&tokenizer, file_content);
    printf("Tokens:\n");
    do {
        token = tokenizer_get_token(&tokenizer);
        print_token(token);
        token_cleanup(&token);
    } while (token.type != TOKEN_END && token.type != TOKEN_ERROR);
    tokenizer_cleanup(&tokenizer);

    /* Then parse and print AST for each expression */
    tokenizer_init(&tokenizer, file_content);
    printf("\nParsed expressions:\n");

    while (1) {
        ast = parse_expression(&tokenizer);
        if (!ast) {
            break;
        }

        printf("Expression: ");
        print_node(ast);
        printf("\n");

        node_cleanup(ast);
    }

    tokenizer_cleanup(&tokenizer);
}

void run_verbose_batch_mode(char* file_content) {
    Tokenizer tokenizer;
    Token token;
    Node* ast;

    printf("Verbose batch mode - processing file with output...\n");
    printf("File content: %s\n\n", file_content);

    /* First tokenize and print all tokens */
    tokenizer_init(&tokenizer, file_content);
    printf("Tokens:\n");
    do {
        token = tokenizer_get_token(&tokenizer);
        print_token(token);
        token_cleanup(&token);
    } while (token.type != TOKEN_END && token.type != TOKEN_ERROR);
    tokenizer_cleanup(&tokenizer);

    /* Then parse and print AST for each expression */
    tokenizer_init(&tokenizer, file_content);
    printf("\nParsed expressions:\n");

    while (1) {
        ast = parse_expression(&tokenizer);
        if (!ast) {
            break;
        }

        printf("Expression: ");
        print_node(ast);
        printf("\n");

        node_cleanup(ast);
    }

    tokenizer_cleanup(&tokenizer);
}

ProgramMode setup(int argc, char* argv[], const char** input_file) {
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

int main(int argc, char* argv[]) {
    const char* input_file = NULL;
    char* file_content = NULL;
    ProgramMode mode;

    mode = setup(argc, argv, &input_file);

    switch (mode) {
        case MODE_INTERACTIVE:
            run_interactive_mode();
            break;

        case MODE_BATCH:
            file_content = load_input_file(input_file);
            if (file_content) {
                run_batch_mode(file_content);
                program_cleanup(file_content);
            } else {
                fprintf(stderr, "Error: Could not load file %s\n", input_file);
                return 1;
            }
            break;

        case MODE_VERBOSE_BATCH:
            file_content = load_input_file(input_file);
            if (file_content) {
                run_verbose_batch_mode(file_content);
                program_cleanup(file_content);
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
