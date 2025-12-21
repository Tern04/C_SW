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
Value* eval(const Env* env, const Node* node);

/**
 * Evaluates the list based on its first element - primitive function
 * @param env Environment with variables
 * @param node Node to evaluate
 * @return Value from the evaluation
 */
Value* eval_list(const Env* env, const Node* node);

/**
 * Prepares array of evaluated argument values from node's children
 * @param env Environment with variables
 * @param node Node to handle
 * @param args_count Store number of arguments
 * @return Array of evaluated argument values
 */
Value** handle_arguments(const Env* env, const Node* node, int* args_count);

/**
 * Frees allocated memory of arguments for primitive functions
 * @param args Arguments to be cleaned up
 * @param count Number of arguments
 */
void arguments_cleanup(Value** args, int count);

#endif /* C_SW_EVAL_H */
