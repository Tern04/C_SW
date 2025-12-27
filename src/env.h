
#ifndef C_SW_ENV_H
#define C_SW_ENV_H

struct Value; /* Forward declaration of Value structure */

/* Structure for the environment that holds created variables from the lisp code */
typedef struct Env{
    char** names; /* Array of variable names */
    struct Value** values; /* Array of variable values */
    int count; /* Total number of variables */
}Env;

/**
 * Create a new environment
 * @return Empty environment structure
 */
Env* create_env(void);

/**
 * Setup of global variables - T and NIL
 * @param env Environment structure to be set up
 */
void setup_env(Env* env);

/**
 * Sets or rewrite variable in the environment
 * @param env Pointer to an environment structure
 * @param name Name of the variable
 * @param value Value to be set
 */
void env_set_variable(Env* env, const char* name, struct Value* value);

/**
 *
 * @param env Pointer to an environment structure
 * @param name Name of the varible
 * @return Pointer to the found value or NULL
 */
struct Value* env_get_value(const Env* env, const char* name);

/**
 * Cleans up allocated memory of the environment structure
 * @param env Pointer to the environment structure to clean up
 */
void env_cleanup(Env* env);

#endif /* C_SW_ENV_H */
