/*
* Created by Tomáš Rybák on 03.11.2025.
*/

#ifndef C_SW_PARSER_H
#define C_SW_PARSER_H

#include "tokenizer.h"
#include "s_exp.h"


Node* parse_expression(Tokenizer* tokenizer);
void node_cleanup(Node* node);

#endif /* C_SW_PARSER_H */
