#include "token.h"

#include <stdlib.h>

static int is_digit(char c) { return (c >= '0' && c <= '9'); }

static int is_alpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

static int is_space(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }

static int is_operator_char(char c) { return c == '+' || c == '-' || c == '*' || c == '/' || c == '^'; }

static int is_function_name(const char* name) {
    return strcmp(name, "sin") == 0 || strcmp(name, "cos") == 0 || strcmp(name, "tan") == 0 ||
           strcmp(name, "log") == 0;
}

static int is_constant_name(const char* name) { return strcmp(name, "pi") == 0; }

static int is_variable_name(const char* name) { return strcmp(name, "x") == 0; }

static TokenType get_identifier_type(const char* name) {
    if (is_function_name(name)) return TOKEN_FUNCTION;
    if (is_constant_name(name)) return TOKEN_CONSTANT;
    if (is_variable_name(name)) return TOKEN_VARIABLE;
    return TOKEN_NUMBER;
}

Token create_token_from_char(char c) {
    Token token;
    strcpy(token.name, "");
    token.value = 0;
    token.precedence = 0;

    if (is_digit(c)) {
        token.type = TOKEN_NUMBER;
        token.value = c - '0';
    } else if (is_operator_char(c)) {
        token.type = TOKEN_OPERATOR;
        token.name[0] = c;
        token.name[1] = '\0';
        if (c == '^')
            token.precedence = 3;
        else if (c == '*' || c == '/')
            token.precedence = 2;
        else
            token.precedence = 1;
    } else if (c == 'x') {
        token.type = TOKEN_VARIABLE;
        strcpy(token.name, "x");
    } else if (c == '(') {
        token.type = TOKEN_LEFT_PAREN;
    } else if (c == ')') {
        token.type = TOKEN_RIGHT_PAREN;
    } else {
        token.type = TOKEN_FUNCTION;
    }
    return token;
}

void init_token_list(TokenList* list) { list->count = 0; }

int add_token(TokenList* list, Token token) {
    if (list->count >= MAX_TOKENS) return 0;
    list->tokens[list->count] = token;
    list->count++;
    return 1;
}

int tokenize_expression(const char* expr, TokenList* tokens) {
    int i = 0;
    int len = strlen(expr);
    TokenType prev_type = TOKEN_LEFT_PAREN;
    int success = 1;

    init_token_list(tokens);

    while (i < len && success) {
        if (is_space(expr[i])) {
            i++;
            continue;
        }

        if (is_digit(expr[i]) || (expr[i] == '.' && i + 1 < len && is_digit(expr[i + 1]))) {
            char* endptr;
            double value = strtod(expr + i, &endptr);
            if (endptr == expr + i) {
                success = 0;
            } else {
                Token token = {TOKEN_NUMBER, "", value, 0};
                if (add_token(tokens, token)) {
                    i = endptr - expr;
                    prev_type = TOKEN_NUMBER;
                } else {
                    success = 0;
                }
            }
            continue;
        }

        if (is_alpha(expr[i])) {
            int start = i;
            while (i < len && (is_alpha(expr[i]) || is_digit(expr[i]))) i++;

            int ident_len = i - start;
            char name[32];
            if (ident_len >= 32) ident_len = 31;
            strncpy(name, expr + start, ident_len);
            name[ident_len] = '\0';

            TokenType type = get_identifier_type(name);
            if (type == TOKEN_NUMBER && strcmp(name, "x") != 0 && strcmp(name, "pi") != 0 &&
                !is_function_name(name)) {
                success = 0;
            } else {
                Token token = {type, "", 0, 0};
                strcpy(token.name, name);

                if (type == TOKEN_CONSTANT && strcmp(name, "pi") == 0) {
                    token.value = 3.14159265358979323846;
                }

                if (add_token(tokens, token)) {
                    prev_type = type;
                } else {
                    success = 0;
                }
            }
            continue;
        }

        Token token = create_token_from_char(expr[i]);

        if (expr[i] == '-' &&
            (prev_type == TOKEN_LEFT_PAREN || prev_type == TOKEN_OPERATOR || tokens->count == 0)) {
            token.type = TOKEN_OPERATOR;
            strcpy(token.name, "~");
            token.precedence = 4;
        }

        if (add_token(tokens, token)) {
            prev_type = token.type;
            i++;
        } else {
            success = 0;
        }
    }

    return success;
}