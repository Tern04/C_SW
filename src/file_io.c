/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "file_io.h"


#include <stdio.h>


int load_input_file(const char* filename) {
    FILE* file;

    file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    char line[256];

    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
    return 0;
}
