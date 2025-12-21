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

#endif /* C_SW_EVAL_H */
