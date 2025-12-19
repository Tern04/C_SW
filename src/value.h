/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_VALUE_H
#define C_SW_VALUE_H
#include "s_exp.h"

/* Types of values */
typedef enum {
    VALUE_INT, /* Integer value */
    VALUE_STRING, /* String value */
    VALUE_LIST, /* Arrays */
    VALUE_NIL /* Nil value */
}ValueType;

/* Structure for value representation */
typedef struct {
    ValueType type; /* Type of value */
    union { /* Depends on the type */
        long int_value; /* Integer value */
        char* string_value; /* String value */
        Node* list_node;
    }data;
}Value;

/**
 * Frees allocated memory inside a Value structure
 * Only string values require a cleanup
 * @param value Value to be freed
 */
void value_cleanup(Value* value);

#endif /* C_SW_VALUE_H */
