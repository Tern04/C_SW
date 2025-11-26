/*
 * Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_FILE_IO_H
#define C_SW_FILE_IO_H

/**
 * Reads the file and return it as a dynamically allocated string
 * @param filename Name of the input file
 * @return content of an input file in a dynamically allocated string or NULL on error
 */
char* load_content_from_file(const char* filename);

#endif /* C_SW_FILE_IO_H */
