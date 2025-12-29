
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "value.h"
#include "errors.h"
#include "env.h"

/*
 * Create a new environment
 */
Env* create_env(void) {
    Env* env;

    env = malloc(sizeof(Env)); /* Allocate memory for the environment structure */

    /* Check for allocation failure */
    if (!env) {
        return NULL;
    }

    /* Set initial values */
    env->names = NULL;
    env->values = NULL;
    env->count = 0;

    return env;
}

/*
 * Setup of global variables - T and NIL
 */
int setup_env(Env* env) {
    Value* t;
    Value* nil;
    int result;

    t = create_t_value(); /* Create T value */
    if (!t) {
        return 1; /* Allocation failure */
    }

    result = env_set_variable(env, "T", t); /* Set T variable in the environment */

    if (result != 0) {
        value_cleanup(t);
        free(t);
        return 1; /* Failed to set T */
    }

    nil = create_nil_value(); /* Create NIL value */

    if (!nil) {
        return 1; /* Allocation failure */
    }

    result = env_set_variable(env, "NIL", nil); /* Set NIL variable in the environment */

    if (result != 0) {
        value_cleanup(nil);
        free(nil);
        return 1; /* Failed to set NIL */
    }

    return 0; /* Success */
}

/*
 * Sets or rewrite variable in the environment
 */
int env_set_variable(Env* env, const char* name, Value* value) {
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
            return 0; /* Success */
        }
    }

    /* If it does not exist, create a new one */
    temp_names = realloc(env->names, sizeof(char*) * (env->count + 1)); /* Make names bigger by the new variable */

    /* Check for allocation failure */
    if (!temp_names) {
        handle_error(ERR_OUT_OF_MEMORY, "Failed to allocate memory for variable names");
        return 1; /* Allocation failure */
    }

    temp_values = realloc(env->values, sizeof(Value*) * (env->count + 1)); /* Make values bigger by the new variable */

    /* Check for allocation failure */
    if (!temp_values) {
        handle_error(ERR_OUT_OF_MEMORY, "Failed to allocate memory for variable values");
        return 1; /* Allocation failure */
    }

    /* Replace names and values for the new one */
    env->names = temp_names;
    env->values = temp_values;

    /* Allocate memory for variable name */
    temp_name = malloc(strlen(name) + 1);

    /* Check for allocation failure */
    if (temp_name == NULL) {
        handle_error(ERR_OUT_OF_MEMORY, "Failed to allocate memory for variable name");
        return 1; /* Allocation failure */
    }

    /* Add new variable to the structure */
    strcpy(temp_name, name);
    env->names[env->count] = temp_name;
    env->values[env->count] = value;
    env->count++;

    return 0; /* Success */
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
        free(env->values[i]);
    }

    free(env->names);
    free(env->values);
    free(env);
}
