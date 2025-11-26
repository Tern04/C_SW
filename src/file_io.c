/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "file_io.h"


#include <stdio.h>
#include <stdlib.h>


char* load_content_from_file(const char* filename) {
    FILE* file;
    long length;
    char* content;
    size_t size;

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error: Could not seek to end of file %s\n", filename);
        fclose(file);
        return NULL;
    }

    length = ftell(file);

    if (length < 0) {
        fprintf(stderr, "Error: Could not get length of file %s\n", filename);
        fclose(file);
        return NULL;
    }

    rewind(file);
    content = malloc((size_t)length + 1);

    if (content == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    size = fread(content, 1, (size_t)length, file);
    content[size] = '\0';

    fclose(file);
    return content;
}
