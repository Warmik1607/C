#include "per_rpn.h"

typedef struct {
    Token data[STACK_SIZE];
    int top;
} Stack;

static void init_stack(Stack* s) { s->top = -1; }

static int push(Stack* s, Token t) {
    if (s->top >= STACK_SIZE - 1) return 0;
    s->data[++s->top] = t;
    return 1;
}

static Token pop(Stack* s) {
    Token empty_token = {.type = TOKEN_NUMBER};
    if (s->top >= 0) return s->data[s->top--];
    return empty_token;
}

static Token peek(Stack* s) {
    Token empty_token = {.type = TOKEN_NUMBER};
    if (s->top >= 0) return s->data[s->top];
    return empty_token;
}

static int is_empty(Stack* s) { return s->top < 0; }

static int get_priority(Token op) {
    if (strcmp(op.name, "~") == 0) return 4;
    if (strcmp(op.name, "*") == 0 || strcmp(op.name, "/") == 0) return 3;
    if (strcmp(op.name, "+") == 0 || strcmp(op.name, "-") == 0) return 2;
    return 0;
}

int convert_to_rpn(TokenList* input, TokenList* output) {
    Stack stack;
    init_stack(&stack);
    int success = 1;

    init_token_list(output);

    for (int i = 0; i < input->count && success; i++) {
        Token t = input->tokens[i];

        if (t.type == TOKEN_NUMBER || t.type == TOKEN_VARIABLE || t.type == TOKEN_CONSTANT) {
            success = add_token(output, t);
        } else if (t.type == TOKEN_FUNCTION) {
            success = push(&stack, t);
        } else if (t.type == TOKEN_OPERATOR) {
            while (success && !is_empty(&stack)) {
                Token top = peek(&stack);
                if (top.type != TOKEN_OPERATOR) break;
                if (get_priority(t) <= get_priority(top)) {
                    success = add_token(output, pop(&stack));
                } else {
                    break;
                }
            }
            if (success) success = push(&stack, t);
        } else if (t.type == TOKEN_LEFT_PAREN) {
            success = push(&stack, t);
        } else if (t.type == TOKEN_RIGHT_PAREN) {
            int found = 0;
            while (success && !is_empty(&stack)) {
                Token top = pop(&stack);
                if (top.type == TOKEN_LEFT_PAREN) {
                    found = 1;
                    break;
                }
                success = add_token(output, top);
            }
            if (!found) success = 0;

            if (success && !is_empty(&stack) && peek(&stack).type == TOKEN_FUNCTION) {
                success = add_token(output, pop(&stack));
            }
        }
    }

    while (success && !is_empty(&stack)) {
        Token t = pop(&stack);
        if (t.type == TOKEN_LEFT_PAREN || t.type == TOKEN_RIGHT_PAREN) {
            success = 0;
        } else {
            success = add_token(output, t);
        }
    }

    return success;
}