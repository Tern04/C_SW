/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "env.h"
#include "value.h"

#include <stdlib.h>

Env* create_env(void) {
    Env* env;

    env = malloc(sizeof(Env));
    env->names = NULL;
    env->values = NULL;
    env->count = 0;
    return env;
}

void env_cleanup(Env* env) {
    int i;

    if (!env) {
        return;
    }

    for (i = 0; i < env->count; i++) {
        free(env->names[i]);
        value_cleanup(env->values[i]);
    }

    free(env->names);
    free(env->values);
    free(env);
}
