/*
* Created by Tomáš Rybák on 19.10.2025.
*/
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "file_io.h"

int setup(const int argc, char* argv[], const char** input_file) {
    int i;
    struct stat buffer;

    *input_file = NULL;

    if (argc < 2) {
        return -1;
    }

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            /* Batch mode */
        }
        else if (i == 1) {
            *input_file = argv[i];
        }
    }

    /* Ensure an input file was provided */
    if (*input_file == NULL) {
        return -1;
    }

    /* Verify the existence of the input file */
    if (stat(*input_file, &buffer) != 0) {
        return -1;
    }

    load_input_file(*input_file);
    return 0;
}

int main(const int argc, char* argv[]) {
    const char* input_file;

    setup(argc, argv, &input_file);

    return 0;
}
