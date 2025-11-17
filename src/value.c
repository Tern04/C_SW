/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#include "value.h"
#include <stdlib.h>

void value_cleanup(Value* value) {
    if (!value) {
        return;
    }

    /* Free string value - Int and Nil are not dynamically allocated */
    if (value->type == VALUE_STRING && value->data.string_value) {
        free(value->data.string_value);
        value->data.string_value = NULL;
    }
}
