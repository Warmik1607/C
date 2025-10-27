#include "calc.h"

#include <math.h>

#define CALC_STACK_SIZE 256

typedef struct {
    double data[CALC_STACK_SIZE];
    int top;
} CalcStack;

static void init_calc(CalcStack* s) { s->top = -1; }

static int push_calc(CalcStack* s, double v) {
    if (s->top >= CALC_STACK_SIZE - 1) return 0;
    s->data[++s->top] = v;
    return 1;
}

static double pop_calc(CalcStack* s) {
    if (s->top >= 0) return s->data[s->top--];
    return 0.0;
}

int calculate_rpn(TokenList* rpn, double x, double* result) {
    CalcStack stack;
    init_calc(&stack);
    int success = 1;

    for (int i = 0; i < rpn->count && success; i++) {
        Token t = rpn->tokens[i];

        if (t.type == TOKEN_NUMBER) {
            success = push_calc(&stack, t.value);
        } else if (t.type == TOKEN_VARIABLE) {
            success = push_calc(&stack, x);
        } else if (t.type == TOKEN_CONSTANT) {
            if (strcmp(t.name, "pi") == 0) {
                success = push_calc(&stack, 3.14159265358979323846);
            }
        } else if (t.type == TOKEN_OPERATOR) {
            if (strcmp(t.name, "~") == 0) {
                if (stack.top < 0) {
                    success = 0;
                } else {
                    double a = pop_calc(&stack);
                    success = push_calc(&stack, -a);
                }
            } else {
                if (stack.top < 1) {
                    success = 0;
                } else {
                    double b = pop_calc(&stack);
                    double a = pop_calc(&stack);

                    if (strcmp(t.name, "+") == 0) {
                        double r = a + b;
                        success = push_calc(&stack, r);
                    } else if (strcmp(t.name, "-") == 0) {
                        double r = a - b;
                        success = push_calc(&stack, r);
                    } else if (strcmp(t.name, "*") == 0) {
                        double r = a * b;
                        success = push_calc(&stack, r);
                    } else if (strcmp(t.name, "/") == 0) {
                        if (fabs(b) < 1e-12) {
                            success = 0;
                        } else {
                            double r = a / b;
                            success = push_calc(&stack, r);
                        }
                    }
                }
            }
        } else if (t.type == TOKEN_FUNCTION) {
            if (stack.top < 0) {
                success = 0;
            } else {
                double arg = pop_calc(&stack);

                if (strcmp(t.name, "log") == 0 && arg <= 0) {
                    success = 0;
                } else {
                    double r = 0.0;

                    if (strcmp(t.name, "sin") == 0)
                        r = sin(arg);
                    else if (strcmp(t.name, "cos") == 0)
                        r = cos(arg);
                    else if (strcmp(t.name, "tan") == 0)
                        r = tan(arg);
                    else if (strcmp(t.name, "log") == 0)
                        r = log(arg);

                    success = push_calc(&stack, r);
                }
            }
        }
    }

    if (success && stack.top == 0) {
        *result = pop_calc(&stack);
    } else {
        success = 0;
    }

    return success;
}