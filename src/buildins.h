/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_BUILDINS_H
#define C_SW_BUILDINS_H

struct Value;

/* Enum for types of build in functions */
typedef enum BuildinType{
    /* ==========================================================
     * Section for special form operators (eval.c)
     * They work directly with unevaluated arguments from the AST.
     * ========================================================== */

    /* Special operators  */
    BI_QUOTE,   /* quote, ' */
    BI_SET,     /* set */
    BI_INC,     /* inc */
    BI_DEC,     /* dec */
    BI_QUIT,    /* quit */

    /* Control operators */
    BI_IF,      /* if */
    BI_WHILE,   /* while */
    BI_BRK,     /* brk */

    /* ==========================================================
     * Section for standard build-in functions - primitives (buildins.c)
     * They work with already evaluated arguments.
     * ========================================================== */

    /* Arithmetic operators */
    BI_ADD,     /* + */
    BI_SUB,     /* - */
    BI_MUL,     /* * */
    BI_DIV,     /* / */
    BI_MAX,     /* max */
    BI_MIN,     /* min */

    /* Relation operators */
    BI_EQ,      /* = */
    BI_NEQ,     /* /= */
    BI_LT,      /* < */
    BI_GT,      /* > */
    BI_LTE,     /* <= */
    BI_GTE,     /* >= */

    /* Operators for lists and atoms */
    BI_LIST,    /* list */
    BI_ATOM,    /* atom */
    BI_CAR,     /* car */
    BI_CDR,     /* cdr */
    BI_NTH,     /* nth */
    BI_LENGTH,  /* length */

    /* Interaction operators */
    BI_PRINT,   /* print */

    /* Unknown type of build-in function */
    BI_UNKNOWN
} BuildinType;

/* Definition of a pointer to a primitive function */
typedef struct Value* (*PrimitiveFunction)(BuildinType type, struct Value** args, int args_count);

/* Structure for mapping between the function name and its type */
typedef struct BuildinMapping{
    const char* name; /* Name of the built-in function */
    BuildinType type; /* Type of the built-in function */
    PrimitiveFunction function; /* Pointer to the built-in function - NULL for special forms */
} BuildinMapping;

/**
 * Get type of primitive function by its name
 * @param name First element of a list - name of the primitive function
 * @return Type of primitive function
 */
BuildinType get_buildin_type(const char* name);

/**
 * Call the primitive function by its type
 * @param type Type of the primitive function
 * @param args Array of argument values
 * @param args_count Number of arguments
 * @return Result value of the primitive function
 */
struct Value* call_prim_function(BuildinType type, struct Value** args, int args_count);

/**
 * Lisp arithmetic functions (+, -, *, /, MIN, MAX)
 * @param type Type of arithmetic operation (+, -, *, /, MIN, MAX)
 * @param args Array of arguments
 * @param args_count Number of arguments
 * @return Result value of the arithmetic operation
 */
struct Value* prim_arithmetics(BuildinType type , struct Value** args, int args_count);

/**
 * Lisp print primitive function (print)
 * @param type Type of primitive function
 * @param args Array of argument values to be printed
 * @param args_count Number of arguments
 * @return Printed value
 */
struct Value* prim_print(BuildinType type, struct Value** args, int args_count);

/**
 * Lisp comparison primitive function (=, /=, <, >, <=, >=)
 * @param type Type of comparison (=, /=, <, >, <=, >=)
 * @param args Array of argument values
 * @param args_count Number of arguments
 * @return Result value of the comparison (T or NIL)
 */
struct Value* prim_compare(BuildinType type, struct Value** args, int args_count);

/**
 * Lisp list primitive function (list)
 * @param type Type of primitive function
 * @param args Array of argument values to be put into the list
 * @param args_count Number of arguments
 * @return Created list value
 */
struct Value* prim_list(BuildinType type, struct Value** args, int args_count);

/**
 * Lisp atom primitive function (atom)
 * @param type Type of primitive function
 * @param args Array of argument values
 * @param args_count Number of arguments
 * @return Result value (T or NIL)
 */
struct Value* prim_atom(BuildinType type, struct Value** args, int args_count);

/**
 * Lisp car primitive function (car)
 * returns the first element of the list
 * @param type Type of primitive function
 * @param args Array of argument values
 * @param args_count Number of arguments
 * @return Value of the first element of the list
 */
struct Value* prim_car(BuildinType type, struct Value** args, int args_count);


/**
 * Lisp cdr primitive function (cdr)
 * @param type Type of primitive function
 * @param args Array of argument values
 * @param args_count Number of arguments
 * @return Tail of the list - all elements except the first one
 */
struct Value* prim_cdr(BuildinType type, struct Value** args, int args_count);

/**
 * Lisp nth primitive function (nth)
 * returns the n-th element of the list
 * @param type Type of primitive function
 * @param args Array of argument values
 * @param args_count Number of arguments
 * @return Value of the n-th element of the list
 */
struct Value* prim_nth(BuildinType type, struct Value** args, int args_count);

/**
 * Lisp length primitive function (length)
 * @param type Type of primitive function
 * @param args Array of argument values
 * @param args_count Number of arguments
 * @return Value of the list
 */
struct Value* prim_length(BuildinType type, struct Value** args, int args_count);



#endif /* C_SW_BUILDINS_H */
