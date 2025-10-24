/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_VALUE_H
#define C_SW_VALUE_H

typedef enum {
    VALUE_INT,
    VALUE_STRING,
    VALUE_NIL
}ValueType;

typedef struct {
    ValueType type;
    union {
        long int_value;
        char* string_value;
    }data;
}Value;

#endif /* C_SW_VALUE_H */
