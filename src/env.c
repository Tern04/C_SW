/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "env.h"
#include "value.h"

#include <stdlib.h>
#include <string.h>

/*
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

void env_set_variable(Env* env, const char* name, Value* value) {
    int i;

    /* Check if the variable exists to rewrite its value */
    for (i = 0; i < env->count; i++) {
        if (strcmp(env->names[i], name) == 0) {
            /* Variable already exists, update its value */
            value_cleanup(env->values[i]); /* Free the old value */
            env->values[i] = value; /* Set the new value */
            return;
        }
    }

    /* If it does not exist, create a new one */


}

/*
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
