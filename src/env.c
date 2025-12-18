/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "env.h"

#include <stdio.h>

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
    char** temp_names;
    Value** temp_values;
    char* temp_name;


    /* Check if the variable exists to rewrite its value */
    for (i = 0; i < env->count; i++) {
        if (strcmp(env->names[i], name) == 0) {
            /* Variable already exists, update its value */
            value_cleanup(env->values[i]); /* Free the old value */
            free(env->values[i]); /* Free the old value structure */
            env->values[i] = value; /* Set the new value */
            return;
        }
    }

    /* If it does not exist, create a new one */
    temp_names = realloc(env->names, sizeof(char*) * (env->count + 1)); /* Make names bigger by the new variable */

    /* Check for allocation failure */
    if (!temp_names) {
        fprintf(stderr, "Failed to allocate memory for names in the environment structure");
        return;
    }

    temp_values = realloc(env->values, sizeof(Value*) * (env->count + 1)); /* Make values bigger by the new variable */

    /* Check for allocation failure */
    if (!temp_values) {
        fprintf(stderr, "Failed to allocate memory for values in the environment structure");
        return;
    }

    /* Replace names and values for the new one */
    env->names = temp_names;
    env->values = temp_values;

    /* Allocate memory for variable name */
    temp_name = malloc(strlen(name) + 1);

    /* Check for allocation failure */
    if (temp_name == NULL) {
        fprintf(stderr, "Error: Memory allocation for variable name failed\n");
        return;
    }

    /* Add new variable to the structure */
    strcpy(temp_name, name);
    env->values[env->count] = value;
    env->count++;

}

Value* env_get_value(const Env* env, const char* name) {
    int i;

    /* Check every variable */
    for (i = 0; i < env->count; i++) {
        if (strcmp(env->names[i], name) == 0) {
            return env->values[i]; /* Return its value if it exists */
        }
    }
    return NULL; /* Return NULL if it was not found */
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
