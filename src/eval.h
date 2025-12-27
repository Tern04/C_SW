
#ifndef C_SW_EVAL_H
#define C_SW_EVAL_H

struct Node; /* Forward declaration of Node structure */
struct Value; /* Forward declaration of Value structure */
struct Env; /* Forward declaration of Env structure */

/**
 * Main evaluation function
 * @param env Environment with variables
 * @param node Node to evaluate
 * @return Value from the evaluation
 */
struct Value* eval(struct Env* env, const struct Node* node);

/**
 * Evaluates the list based on its first element - primitive function
 * @param env Environment with variables
 * @param node Node to evaluate
 * @return Value from the evaluation
 */
struct Value* eval_list(struct Env* env, const struct Node* node);

/**
 * Prepares array of evaluated argument values from node's children
 * @param env Environment with variables
 * @param node Node to handle
 * @param args_count Store number of arguments
 * @return Array of evaluated argument values
 */
struct Value** handle_arguments(struct Env* env, const struct Node* node, int* args_count);

/**
 * Handles the QUOTE - returns the argument without evaluation
 * @param node Node to handle
 * @return Quoted value
 */
struct Value* handle_quote(const struct Node* node);

/**
 * Handles the SET - Save or update variable in the environment
 * @param env Environment with variables
 * @param node Node to handle
 * @return Value of the set operation
 */
struct Value* handle_set(struct Env* env, const struct Node* node);

/**
 * Handles setting a value to a place
 * For instance (set (nth 0 arr) 10))
 * @param env Environment with variables
 * @param place_node Node representing the place
 * @param new_value New value to be set
 * @return Value of the set operation
 */
struct Value* handle_place_set(struct Env* env, const struct Node* place_node, struct Value* new_value);

/**
 * Handles the INC and DEC - Increment or decrement variable in the environment
 * Using one method for both operations to reduce code duplication
 * @param env Environment with variables
 * @param node Node to handle
 * @param flag 1 - handles the increment | -1 - handles the decrement
 * @return Value of the operation or NIL if error
 */
struct Value* handle_inc_dec(struct Env* env, const struct Node* node, int flag);

/**
 * Handles the IF - conditional evaluation
 * @param env Environment with variables
 * @param node Node to handle
 * @return Value of the IF operation
 */
struct Value* handle_if(struct Env* env, const struct Node* node);

/**
 * Handles the WHILE - conditional evaluation
 * @param env Environment with variables
 * @param node Node to handle
 * @return Value of the WHILE operation
 */
struct Value* handle_while(struct Env* env, const struct Node* node);

/**
 * Handles the BRK - break from the while loop
 * @return Break value
 */
struct Value* handle_brk(void);

/**
 * Exits the program
 */
void handle_quit(void);

/**
 * Frees allocated memory of arguments for primitive functions
 * @param args Arguments to be cleaned up
 * @param count Number of arguments
 */
void arguments_cleanup(struct Value** args, int count);

#endif /* C_SW_EVAL_H */
