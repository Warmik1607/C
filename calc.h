#ifndef CALC_H
#define CALC_H

#include "token.h"

int calculate_rpn(TokenList* rpn, double x, double* result);

#endif