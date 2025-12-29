
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "s_exp.h"
#include "value.h"
#include "env.h"
#include "buildins.h"
#include "errors.h"
#include "parser.h"
#include "eval.h"

/*
 * Main evaluation function
 */
Value* eval(Env* env, const Node* node) {
    Value* value;

    /* Evaluate based on the node type */
    switch (node->type) {
        case NODE_INT:
            /* Create int value */
            value = create_int_value(node->value.int_value);
            break;
        case NODE_STRING:
            /* Create string value */
            value = create_string_value(node->value.text_value);
            break;
        case NODE_SYMBOL:
            /* Lookup symbol in the environment */
            value = env_get_value(env, node->value.text_value);

            /* Check if the symbol is defined */
            if (!value) {
                handle_error(ERR_RUNTIME_ERROR, "Symbol is not defined");
                return create_nil_value();
            }
            return create_value_copy(value); /* Return a copy of the found value */
        case NODE_LIST:
            /* Evaluate the list */
            value = eval_list(env, node);
            break;
        default:
            return NULL;
    }
    return value;
}

/*
 * Evaluates the list based on its first element - primitive function
 */
Value* eval_list(Env* env, const Node* node) {
    BuildinType type;
    Node* first_elem;
    Value* result;
    Value** args;
    int args_count;

    /* Empty list  */
    if (node->value.list.count == 0) {
        return create_nil_value();
    }

    first_elem = node->value.list.children[0]; /* Get the first element of the list */

    /* The first element must be a symbol */
    if (first_elem->type != NODE_SYMBOL) {
        handle_error(ERR_SYNTAX_ERROR, "First element of a list must be a symbol");
        return create_nil_value();
    }

    /* Get a type of primitive function by the first element in the list */
    type = get_buildin_type(first_elem->value.text_value);

    /* Handle the built-in function based on its type */
    switch (type) {
        /* Section 1 - special forms will be handled in eval.c */
        case BI_QUOTE:
            return handle_quote(node);
        case BI_SET:
            return handle_set(env, node);
        case BI_INC:
            return handle_inc_dec(env, node, 1);
        case BI_DEC:
            return handle_inc_dec(env, node, -1);
        case BI_QUIT:
            return handle_quit();
        case BI_IF:
            return handle_if(env, node);
        case BI_WHILE:
            return handle_while(env, node);
        case BI_BRK:
            return handle_brk();

        /* Section 2 - standard primitive functions will be handled in buildins.c */
        case BI_ADD:  case BI_SUB:  case BI_MUL:
        case BI_DIV:  case BI_MAX:  case BI_MIN:
        case BI_EQ:   case BI_NEQ:  case BI_LT:
        case BI_GT:   case BI_LTE:  case BI_GTE:
        case BI_LIST: case BI_ATOM: case BI_CAR:
        case BI_CDR:  case BI_NTH:  case BI_LENGTH:
        case BI_PRINT:
            args = handle_arguments(env, node, &args_count); /* Get arguments for the primitive function */

            result = call_prim_function(type, args, args_count); /* Call the right primitive function */

            arguments_cleanup(args, args_count); /* Free the arguments */

            return result; /* Return the result from the primitive function */

        default:
            /* Unknown primitive function */
            handle_error(ERR_SYNTAX_ERROR, first_elem->value.text_value);
    }
    return create_nil_value(); /* Return NIL on error */
}

/*
 * Prepares array of evaluated argument values from node's children
 */
Value** handle_arguments(Env* env, const Node* node, int* args_count) {
    Value** args;
    int count;
    int i;

    count = node->value.list.count - 1; /* Elements - name of a primitive function */
    *args_count = count; /* Store number of arguments */

    args = malloc(sizeof(Value*) * count); /* Allocate memory for arguments array */

    /* Check for allocation failure */
    if (!args) {
        handle_error(ERR_RUNTIME_ERROR, "Memory allocation error for function arguments");
        return NULL;
    }
    /* Evaluate all arguments */
    for (i = 0; i < count; i++) {
        args[i] = eval(env, node->value.list.children[i + 1]);
    }

    return args;
}

/*
 * Handles the QUOTE - returns the argument without evaluation
 */
Value* handle_quote(const Node* node) {
    Node * argument;

    /* Check for at least one argument */
    if (node->value.list.count < 2) {
        handle_error(ERR_SYNTAX_ERROR, "QUOTE requires at least one argument");
        return create_nil_value();
    }

    argument = node->value.list.children[1]; /* Get the argument to be quoted */

    /* Return the argument as a value without evaluation */
    switch (argument->type) {
        case NODE_INT:
            return create_int_value(argument->value.int_value); /* Return the integer value */
        case NODE_STRING:
            return create_string_value(argument->value.text_value); /* Return the string value */
        case NODE_SYMBOL:
            return create_symbol_value(argument->value.text_value); /* Return the symbol value */
        case NODE_LIST:
            return create_list_value(argument); /* Return the quoted expression as a list value */
        default:
            /* Unknown argument type */
            handle_error(ERR_SYNTAX_ERROR, "QUOTE requires a list, integer or string as an argument");
            return create_nil_value();
    }

}

/*
 * Handles the SET - Save or update variable in the environment
 */
Value* handle_set(Env* env, const Node* node) {
    Value* name_value;
    Value* value;
    Value* place_result;
    Value* return_value;
    Node* place_node;
    char* name;

    /* Check for two arguments - name and value */
    if (node->value.list.count != 3) {
        handle_error(ERR_SYNTAX_ERROR, "SET expects exactly 2 arguments (name and value)");
        return create_nil_value();
    }

    /* Check if the first argument is a place like (nth 0 arr) */
    place_node = node->value.list.children[1]; /* Get the place node */

    value = eval(env, node->value.list.children[2]);  /* Evaluate the value to be set */

    place_result = handle_place_set(env, place_node, value); /* Try to handle a place set */

    /* If it was a place, return the result */
    if (place_result != NULL) {

        /* Free the evaluated value as it is no longer necessary */
        value_cleanup(value);
        free(value);
        return place_result;
    }

    name_value = eval(env, node->value.list.children[1]); /* Evaluate the name of the variable */

    /* Check if the name is found and if it is a string */
    if (!name_value || (name_value->type != VALUE_STRING && name_value->type != VALUE_SYMBOL)) {
        handle_error(ERR_SYNTAX_ERROR, "SET requires a symbol or a string as a variable name");

        /* Free the name value if it was allocated */
        if (name_value) {
            value_cleanup(name_value);
            free(name_value);
        }

        /* Free the evaluated value */
        value_cleanup(value);
        free(value);

        return create_nil_value(); /* Return NIL on error */
    }

    /* The first argument is a variable name */
    name = name_value->data.string_value;

    /* If it does not exist, create a new variable */
    env_set_variable(env, name, value);

    return_value = create_value_copy(value); /* Create a copy of the set value to return */

    /* Free the name value */
    value_cleanup(name_value);
    free(name_value);

    return return_value; /* Return a copy of the set value */
}

/*
 * Handles setting a value to a place
 * For instance (set (nth 0 arr) 10))
 */
Value* handle_place_set(Env* env, const Node* place_node, Value* new_value) {
    BuildinType type;
    Node* func_node;
    char* func_name;
    Node* list_expr;
    Value* val_idx;
    Value* env_list_val;
    Node* target_list_node;
    int index;

    /* Check if the place_node is a list with at least two elements */
    if (place_node->type != NODE_LIST || place_node->value.list.count < 2) {
        return NULL;
    }

    func_node = place_node->value.list.children[0]; /* Get the function node */

    /* Check if the function node is a symbol */
    if (func_node->type != NODE_SYMBOL) {
        return NULL;
    }

    func_name = func_node->value.text_value; /* Get the function name */

    type = get_buildin_type(func_name); /* Get the built-in function type */

    /* Handle based on the function type */
    switch (type) {
        case BI_NTH:
            /* Check for exactly three arguments */
            if (place_node->value.list.count != 3) {
                return NULL;
            }

            /* Evaluate the index */
            val_idx = eval(env, place_node->value.list.children[1]);

            /* Check if the index is valid */
            if (!val_idx || val_idx->type != VALUE_INT) {

                /* Free the index value if it was allocated */
                if (val_idx) {
                    value_cleanup(val_idx);
                    free(val_idx);
                }
                return NULL;
            }

            /* Check if the index is an integer */
            if (val_idx->type != VALUE_INT) {
                value_cleanup(val_idx);
                free(val_idx);
                return NULL;
            }

            index = (int)val_idx->data.int_value; /* Get the index as an integer */

            /* Free the index value */
            value_cleanup(val_idx);
            free(val_idx);

            list_expr = place_node->value.list.children[2]; /* Get the list expression */
            break;
        case BI_CAR:
            index = 0; /* CAR corresponds to index 0 */

            /* Check for exactly two arguments */
            if (place_node->value.list.count != 2) {
                return NULL;
            }

            list_expr = place_node->value.list.children[1]; /* Get the list expression */
            break;
        default:
            /* Not a place set */
            return NULL;
    }

    /* Check if the list expression is a symbol */
    if (list_expr->type != NODE_SYMBOL) {
        return create_nil_value();
    }

    /* Get the list value from the environment */
    env_list_val = env_get_value(env, list_expr->value.text_value);

    /* Check if the list value exists and is a list */
    if (!env_list_val || env_list_val->type != VALUE_LIST) {
        handle_error(ERR_RUNTIME_ERROR, "Variable is not a list or is undefined");
        return create_nil_value();
    }

    target_list_node = env_list_val->data.list_node; /* Get the target list node */

    /* Check for a valid index */
    if (index < 0 || index >= target_list_node->value.list.count) {
        handle_error(ERR_RUNTIME_ERROR, "SET - index out of bounds");
        return create_nil_value();
    }

    /* Free the old node at the index */
    node_cleanup(target_list_node->value.list.children[index]);

    /* Set the new value at the index */
    target_list_node->value.list.children[index] = create_node_from_value(new_value);

    return create_value_copy(new_value);
}

/*
 * Handles the INC and DEC - Increment or decrement variable in the environment
 * Using one method for both operations to reduce code duplication
 */
Value* handle_inc_dec(Env* env, const Node* node, const int flag) {
    Value* name_value;
    Value* argument;
    Value* env_value;
    Value* return_value;
    char* name;
    long new_result;

    /* Check for exactly two arguments */
    if (node->value.list.count != 3) {
        handle_error(ERR_SYNTAX_ERROR, "INC/DEC requires exactly two arguments");
        return create_nil_value();
    }

    /* Evaluate the name of the variable */
    name_value = eval(env, node->value.list.children[1]);

    /* Check for the valid name */
    if (!name_value || (name_value->type != VALUE_STRING && name_value->type != VALUE_SYMBOL)) {
        handle_error(ERR_SYNTAX_ERROR, "Variable name must be a symbol or string");

        /* Cleanup */
        if (name_value) {
            value_cleanup(name_value);
            free(name_value);
        }
        return create_nil_value();
    }

    name = name_value->data.string_value; /* Get the variable name */

    argument = eval(env, node->value.list.children[2]); /* Evaluate the argument */

    /* Check for integer argument */
    if (!argument || argument->type != VALUE_INT) {
        handle_error(ERR_SYNTAX_ERROR, "Second argument must be an integer");

        /* Cleanup name_value*/
        value_cleanup(name_value);
        free(name_value);

        /* Cleanup argument */
        if (argument) {
            value_cleanup(argument); free(argument);
        }
        return create_nil_value();
    }

    env_value = env_get_value(env, name); /* Get the current value from the environment */

    /* Check if the variable exists and is an integer */
    if (!env_value || env_value->type != VALUE_INT) {
        handle_error(ERR_RUNTIME_ERROR, "Variable is not a defined integer");

        /* Cleanup */
        value_cleanup(name_value);
        free(name_value);
        value_cleanup(argument);
        free(argument);

        return create_nil_value();
    }

    /* Save the new result with increment or decrement */
    new_result = env_value->data.int_value + (argument->data.int_value * flag);

    /* Save the new value back to the environment */
    env_set_variable(env, name, create_int_value(new_result));

    /* Create the return value */
    return_value = create_int_value(new_result);

    /* Free the name_value and argument */
    value_cleanup(name_value);
    free(name_value);
    value_cleanup(argument);
    free(argument);

    return return_value; /* Return a copy of the updated value */
}

/*
 * Handles the IF - conditional evaluation
 */
Value* handle_if(Env* env, const Node* node) {
    Value* condition;
    int is_true;

    /* Check for three or 4 arguments */
    if (node->value.list.count < 3 || node->value.list.count > 4) {
        handle_error(ERR_SYNTAX_ERROR, "IF requires at least two arguments");
        return create_nil_value();
    }

    condition = eval(env, node->value.list.children[1]); /* Evaluate the condition */
    is_true = condition->type != VALUE_NIL; /* Condition is true if not NIL */

    /* Free the condition value */
    value_cleanup(condition);
    free(condition);

    if (is_true) {
        return eval(env, node->value.list.children[2]);
    }else if (node->value.list.count == 4) {
        return eval(env, node->value.list.children[3]);
    }

    return create_nil_value();
}

/*
 * Handles the WHILE - conditional evaluation
 */
Value* handle_while(Env* env, const Node* node) {
    Value* condition;
    Value* result;
    int is_true;
    int i;

    /* Check for at least three arguments */
    if (node->value.list.count < 3) {
        handle_error(ERR_SYNTAX_ERROR, "WHILE requires condition and body");
        return create_nil_value();
    }

    result = create_nil_value(); /* Initialize result as NIL */

    /* Loop while the condition is true */
    while (1) {
        condition = eval(env, node->value.list.children[1]); /* Evaluate the condition */
        is_true = condition->type != VALUE_NIL; /* Condition is true if not NIL */

        /* Free the condition value */
        value_cleanup(condition);
        free(condition);

        /* Exit the loop if the condition is false */
        if (!is_true) {
            break;
        }

        for (i = 2; i < node->value.list.count; i++) {

            /* Free previous result */
            value_cleanup(result);
            free(result);

            result = eval(env, node->value.list.children[i]); /* Evaluate each body expression */

            /* Exit the loop if break value */
            if (result->type == VALUE_BREAK) {

                /* Free the break value */
                value_cleanup(result);
                free(result);

                return create_nil_value(); /* Return NIL on break */
            }
        }
    }
    return result;
}

/*
 * Handles the BRK - break from the while loop
 */
Value* handle_brk(void) {
    return create_break_value(); /* Return the break value */
}

/*
 * Handles the QUIT - exit from the interpreter
 */
Value* handle_quit(void) {
    return create_quit_value(); /* Return the quit value */
}

/*
 * Frees allocated memory of arguments for primitive functions
 */
void arguments_cleanup(Value** args, const int count) {
    int i;

    /* Check for NULL pointer */
    if (!args) {
        return;
    }

    /* Free each argument value */
    for (i = 0; i < count; i++) {
        if (args[i]) {
            value_cleanup(args[i]);
            free(args[i]);
        }
    }
    free(args);
}
