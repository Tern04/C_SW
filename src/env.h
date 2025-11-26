/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_ENV_H
#define C_SW_ENV_H
#include "value.h"

/* Structure for the environment that holds created variables from the lisp code */
typedef struct {
    char** names; /* Array of variable names */
    Value** values; /* Array of variable values */
    int count; /* Total number of variables */
}Env;

/**
 * Create a new environment
 * @return Empty environment structure
 */
Env* create_env(void);

/**
 * Cleans up allocated memory of the environment structure
 * @param env Pointer to the environment structure to clean up
 */
void env_cleanup(Env* env);

#endif /* C_SW_ENV_H */
