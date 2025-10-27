#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calc.h"
#include "per_rpn.h"
#include "token.h"

#define WIDTH 80
#define HEIGHT 25
#define PI 3.14159265358979323846

static int is_letter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

static int is_digit(char c) { return (c >= '0' && c <= '9'); }

int check_balance(const char *expr) {
    int balance = 0;
    int i = 0;

    while (expr[i] != '\0') {
        if (expr[i] == '(')
            balance++;
        else if (expr[i] == ')')
            balance--;
        if (balance < 0) return 0;
        i++;
    }

    return balance == 0;
}

int valid_chars(const char *expr) {
    int i = 0;

    while (expr[i] != '\0') {
        char c = expr[i];
        if (!is_digit(c) && c != 'x' && c != '+' && c != '-' && c != '*' && c != '/' && c != '(' &&
            c != ')' && c != '.' && c != ' ' && !is_letter(c)) {
            return 0;
        }
        i++;
    }

    return 1;
}

int main() {
    char input[256];

    if (!fgets(input, sizeof(input), stdin)) {
        printf("n/a");
        return 1;
    }

    input[strcspn(input, "\n")] = 0;

    if (!valid_chars(input) || !check_balance(input)) {
        printf("n/a");
        return 1;
    }

    TokenList tokens;
    init_token_list(&tokens);
    if (!tokenize_expression(input, &tokens)) {
        printf("n/a");
        return 1;
    }

    TokenList rpn_tokens;
    init_token_list(&rpn_tokens);
    if (!convert_to_rpn(&tokens, &rpn_tokens)) {
        printf("n/a");
        return 1;
    }

    char field[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = '.';
        }
    }

    double x_min = 0.0;
    double x_max = 4 * PI;
    double y_min = -1.0;
    double y_max = 1.0;

    for (int screen_x = 0; screen_x < WIDTH; screen_x++) {
        double x = x_min + screen_x * (x_max - x_min) / (WIDTH - 1);
        double y;

        if (calculate_rpn(&rpn_tokens, x, &y)) {
            int screen_y = (int)round((y_max - y) / (y_max - y_min) * (HEIGHT - 1));
            if (screen_y >= 0 && screen_y < HEIGHT) {
                field[screen_y][screen_x] = '*';
            }
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", field[i][j]);
        }
        printf("\n");
    }

    return 0;
}