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
 * Creates a new value for an integer
 * @param int_value Integer value to be set
 * @return Created value
 */
Value* create_int_value(long int_value);

/**
 * Creates a new value for a string
 * @param string_value String value to be set
 * @return Created value
 */
Value* create_string_value(const char* string_value);

/**
 * Creates a new value for a list
 * @param node Node representing the list
 * @return Created value
 */
Value* create_list_value(Node* node);

/**
 * Creates a NIL value
 * @return NIL value
 */
Value* create_nil_value(void);

/**
 * Creates a copy of value
 * @param value Value to be copied
 * @return Created copy of a value
 */
Value* create_value_copy(Value* value);

/**
 * Frees allocated memory inside a Value structure
 * Only string values require a cleanup
 * @param value Value to be freed
 */
void value_cleanup(Value* value);

#endif /* C_SW_VALUE_H */
