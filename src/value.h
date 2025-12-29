
#ifndef C_SW_VALUE_H
#define C_SW_VALUE_H

struct Node; /* Forward declaration of Node structure */

/* Types of values */
typedef enum ValueType{
    VALUE_INT, /* Integer value */
    VALUE_STRING, /* String value */
    VALUE_SYMBOL, /* Symbol value */
    VALUE_LIST, /* Arrays */
    VALUE_BREAK, /* Break value for while loops */
    VALUE_QUIT, /* Quit value for exiting the interpreter */
    VALUE_ERROR, /* Error value */
    VALUE_T, /* True value */
    VALUE_NIL /* Nil value */
}ValueType;

/* Structure for value representation */
typedef struct Value{
    ValueType type; /* Type of value */
    union { /* Depends on the type */
        long int_value; /* Integer value */
        char* string_value; /* String value */
        struct Node* list_node;
    }data;
}Value;

/**
 * Creates a new value for an integer
 * @param int_value Integer value to be set
 * @return Created integer value
 */
Value* create_int_value(long int_value);

/**
 * Creates a new value for a string
 * @param string_value String value to be set
 * @return Created string value
 */
Value* create_string_value(const char* string_value);

/**
 * Creates a new value for a symbol
 * @param symbol_value Symbol value to be set
 * @return Created symbol value
 */
Value* create_symbol_value(const char* symbol_value);

/**
 * Creates a new value for a list
 * @param node Node representing the list
 * @return Created list value
 */
Value* create_list_value(struct Node* node);

/**
 * Creates a break value - used in while loops
 * @return Break value
 */
Value* create_break_value(void);

/**
 * Creates a quit value - used to exit the interpreter
 * @return Quit value
 */
Value* create_quit_value(void);

/**
 * Creates an error value
 * @param error_code Error code to be set
 * @return Error value
 */
Value* create_error_value(long error_code);

/**
 * Creates a T value - True
 * @return T value
 */
Value* create_t_value(void);

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
Value* create_value_copy(const Value* value);

/**
 * Creates a Value from a Node
 * @param node Node to be converted to Value
 * @return Value created from the node
 */
Value* create_value_from_node(struct Node* node);

/**
 * Frees allocated memory inside a Value structure
 * Only string values require a cleanup
 * @param value Value to be freed
 */
void value_cleanup(Value* value);

#endif /* C_SW_VALUE_H */
