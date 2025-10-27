#ifndef PER_RPN_H
#define PER_RPN_H

#include "token.h"

#define STACK_SIZE 256

int convert_to_rpn(TokenList* input, TokenList* output);

#endif