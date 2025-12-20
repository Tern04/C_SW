/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "primitives.h"

#include <string.h>

PrimitivesType get_primitive_type(const char* name) {
    if (strcmp(name, "+") == 0) return PRIM_ADD;
    if (strcmp(name, "-") == 0) return PRIM_SUB;
    if (strcmp(name, "*") == 0) return PRIM_MUL;
    if (strcmp(name, "/") == 0) return PRIM_DIV;
    if (strcmp(name, "set") == 0) return PRIM_SET;
    if (strcmp(name, "if") == 0) return PRIM_IF;
    if (strcmp(name, "while") == 0) return PRIM_WHILE;
    return PRIM_UNKNOWN;
}
