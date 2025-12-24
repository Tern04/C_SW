/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_EVAL_H
#define C_SW_EVAL_H
#include "env.h"
#include "value.h"

/**
 * Main evaluation function
 * @param env Environment with variables
 * @param node Node to evaluate
 * @return Value from the evaluation
 */
Value* eval(Env* env, const Node* node);

/**
 * Evaluates the list based on its first element - primitive function
 * @param env Environment with variables
 * @param node Node to evaluate
 * @return Value from the evaluation
 */
Value* eval_list(Env* env, const Node* node);

/**
 * Prepares array of evaluated argument values from node's children
 * @param env Environment with variables
 * @param node Node to handle
 * @param args_count Store number of arguments
 * @return Array of evaluated argument values
 */
Value** handle_arguments(Env* env, const Node* node, int* args_count);

/**
 * Handles the QUOTE - returns the argument without evaluation
 * @param node Node to handle
 * @return Quoted value
 */
Value* handle_quote(const Node* node);

/**
 * Handles the SET - Save or update variable in the environment
 * @param env Environment with variables
 * @param node Node to handle
 * @return Value of the set operation
 */
Value* handle_set(Env* env, const Node* node);

/**
 * Handles the INC and DEC - Increment or decrement variable in the environment
 * Using one method for both operations to reduce code duplication
 * @param env Environment with variables
 * @param node Node to handle
 * @param flag 1 - handles the increment | -1 - handles the decrement
 * @return Value of the operation or NIL if error
 */
Value* handle_inc_dec(Env* env, const Node* node, int flag);

/**
 * Exits the program
 */
void handle_quit(void);

/**
 * Frees allocated memory of arguments for primitive functions
 * @param args Arguments to be cleaned up
 * @param count Number of arguments
 */
void arguments_cleanup(Value** args, int count);

#endif /* C_SW_EVAL_H */
