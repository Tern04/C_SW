/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "file_io.h"
#include "errors.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Reads the file and return it as a dynamically allocated string
 */
char* load_content_from_file(const char* filename) {
    FILE* file;
    long length;
    char* content;
    size_t size;


    file = fopen(filename, "r"); /* Try to open the file */

    /* Test if the file was opened successfully */
    if (file == NULL) {
        handle_error(ERR_FILE_ACCESS_FAILURE, "Could not open file");
        return NULL;
    }

    /* Try to et to the end of the file */
    if (fseek(file, 0, SEEK_END) != 0) {
        handle_error(ERR_FILE_ACCESS_FAILURE, "Could not seek to end of file");
        fclose(file);
        return NULL;
    }

    length = ftell(file); /* Get the length of the file */

    /* Test if length retrieval was successful */
    if (length < 0) {
        handle_error(ERR_FILE_ACCESS_FAILURE, "Could not get length of file");
        fclose(file);
        return NULL;
    }

    rewind(file); /* Go back to the beginning of the file */
    content = malloc((size_t)length + 1); /* Allocate the size of a file and the null character*/

    /* Test if the allocation was successful */
    if (content == NULL) {
        fclose(file);
        handle_error(ERR_OUT_OF_MEMORY, "Memory allocation failed");
        return NULL;
    }

    size = fread(content, 1, (size_t)length, file); /* Read the file content */
    content[size] = '\0'; /* Null-terminate the string */

    fclose(file); /* Close the file*/

    return content;
}
