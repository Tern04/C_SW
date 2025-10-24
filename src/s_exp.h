/*
* Created by Tomáš Rybák on 19.10.2025.
*/

#ifndef C_SW_S_EXP_H
#define C_SW_S_EXP_H

typedef enum {
    NODE_INT,
    NODE_STRING,
    NODE_LIST,
    NODE_SYMBOL
}NodeType;

typedef struct Node {
    NodeType type;
    union {
        long int_value;
        char* text_value;
        struct {
            struct Node** children;
            int count;
        }list;
    }value;
}Node;

#endif /* C_SW_S_EXP_H */
