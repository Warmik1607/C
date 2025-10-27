#ifndef TOKEN_H
#define TOKEN_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 256

typedef enum {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_FUNCTION,
    TOKEN_VARIABLE,
    TOKEN_CONSTANT,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN
} TokenType;

typedef struct Token {
    TokenType type;
    char name[32];
    double value;
    int precedence;
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    int count;
} TokenList;

void init_token_list(TokenList* list);
int add_token(TokenList* list, Token token);
int tokenize_expression(const char* expr, TokenList* tokens);

#endif