/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include <stddef.h>

#include <stdio.h>
#include <stdlib.h>

#include "s_exp.h"
#include "value.h"
#include "env.h"
#include "buildins.h"
#include "eval.h"




/*
 * Main evaluation function
 */
Value* eval(Env* env, const Node* node) {
    Value* value;

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
            if (!value) {
                printf("Error: symbol '%s' is not defined\n", node->value.text_value);
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

    /* Check if the first element is valid */
    first_elem = node->value.list.children[0];
    if (first_elem->type != NODE_SYMBOL) {
        printf("Error: first element of a list must be a symbol\n");
        return create_nil_value();
    }

    /* Get a type of primitive function by the first element in the list */
    type = get_buildin_type(first_elem->value.text_value);

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
            handle_quit();
        case BI_IF:
            return handle_if(env, node);
        case BI_WHILE:
            return create_nil_value();
        case BI_BRK:
            /* Handling functions will be implemented - now just return NIL */
            return create_nil_value();

        /* Section 2 - standard primitive functions will be handled in buildins.c */
        case BI_ADD:
        case BI_SUB:
        case BI_MUL:
        case BI_DIV:
        case BI_MAX:
        case BI_MIN:
        case BI_EQ:
        case BI_NEQ:
        case BI_LT:
        case BI_GT:
        case BI_LTE:
        case BI_GTE:
        case BI_LIST:
        case BI_ATOM:
        case BI_CAR:
        case BI_CDR:
        case BI_NTH:
        case BI_LENGTH:
        case BI_PRINT:
            args = handle_arguments(env, node, &args_count); /* Get arguments for the primitive function */

            result = call_prim_function(type, args, args_count); /* Call the right primitive function */

            arguments_cleanup(args, args_count); /* Free the arguments */

            return result; /* Return the result from the primitive function */


        default:
            printf("Error: Unknown or unimplemented function\n");
            return create_nil_value();
    }
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
        return NULL;
    }

    for (i = 0; i < count; i++) {
        args[i] = eval(env, node->value.list.children[i + 1]); /* Evaluate each argument */
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
        printf("Error: quote requires at least one argument\n");
        return create_nil_value();
    }

    argument = node->value.list.children[1];

    switch (argument->type) {
        case NODE_INT:
            return create_int_value(argument->value.int_value);
        case NODE_STRING:
        case NODE_SYMBOL:
            return create_string_value(argument->value.text_value);
        case NODE_LIST:
            return create_list_value(argument); /* Return the quoted expression as a list value */
        default:
            printf("Error: quote requires a list, integer or string as an argument\n");
            return create_nil_value();
    }

}

/*
 * Handles the SET - Save or update variable in the environment
 */
Value* handle_set(Env* env, const Node* node) {
    Value* name_value;
    Value* value;
    char* name;

    /* Check for at least two arguments - name and value */
    if (node->value.list.count < 3) {
        printf("Error: set requires at least two arguments - name and value\n");
        return create_nil_value(); /* Return NIL on error */
    }

    name_value = eval(env, node->value.list.children[1]); /* Evaluate the name of the variable */

    /* Check if the name is found and if it is a string */
    if (!name_value || name_value->type != VALUE_STRING) {
        printf("Error: set requires a string as a variable name\n");

        /* Free the name value if it was allocated */
        if (name_value) {
            value_cleanup(name_value);
            free(name_value);
        }
        return create_nil_value(); /* Return NIL on error */
    }

    /* The first argument is a variable name */
    name = name_value->data.string_value;

    /* Evaluate the value to be set */
    value = eval(env, node->value.list.children[2]);

    /* If it does not exist, create a new variable */
    env_set_variable(env, name, value);

    /* Free the name value */
    value_cleanup(name_value);
    free(name_value);

    /* Return a copy of the set value */
    return create_value_copy(value);
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
        printf("Error: Inc/Dec requires exactly two arguments\n");
        return create_nil_value();
    }

    /* Check for the variable name - must be a symbol */
    name_value = eval(env, node->value.list.children[1]);
    if (!name_value || name_value->type != VALUE_STRING) {
        printf("Error: Variable name must be a symbol\n");

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
        printf("Error: Second argument must be an integer\n");

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
        printf("Error: Variable '%s' is not a defined integer\n", name);

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

    return_value = create_int_value(new_result);

    /* Free the name_value and argument */
    value_cleanup(name_value);
    free(name_value);
    value_cleanup(argument);
    free(argument);

    /* Return a copy of the updated value */
    return return_value;
}

/*
 * Handles the IF - conditional evaluation
 */
Value* handle_if(Env* env, const Node* node) {
    Value* condition;
    int is_true;

    /* Check for three or 4 arguments */
    if (node->value.list.count < 3 || node->value.list.count > 4) {
        printf("Error: IF requires at least two arguments\n");
        return create_nil_value();
    }

    condition = eval(env, node->value.list.children[1]);
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
 * Exits the program
 */
void handle_quit(void) {
    printf("Exiting the interpreter...\n");
    exit(0);
}

/*
 * Frees allocated memory of arguments for primitive functions
 */
void arguments_cleanup(Value** args, const int count) {
    int i;

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
