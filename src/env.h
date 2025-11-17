/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_ENV_H
#define C_SW_ENV_H
#include "value.h"


typedef struct {
    char** names; /* Array of variable names */
    Value** values; /* Array of variable values */
    int count; /* Total number of variables */
}Env;

Env* create_env(void);
void env_cleanup(Env* env);

#endif /* C_SW_ENV_H */
