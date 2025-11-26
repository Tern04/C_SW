/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "env.h"
#include "value.h"

#include <stdlib.h>

/**
 * Create a new environment
 */
Env* create_env(void) {
    Env* env;

    env = malloc(sizeof(Env)); /* Allocate memory for the environment structure */

    /* Set initial values */
    env->names = NULL;
    env->values = NULL;
    env->count = 0;

    return env;
}

/**
 * Cleans up allocated memory of the environment structure
 */
void env_cleanup(Env* env) {
    int i;

    if (!env) {
        return; /* Nothing to clean up */
    }

    /* Free each variable name and value */
    for (i = 0; i < env->count; i++) {
        free(env->names[i]);
        value_cleanup(env->values[i]); /* Free the Value structure */
    }

    free(env->names);
    free(env->values);
    free(env);
}
