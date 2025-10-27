#include <stdio.h>

#define FIELD_WIDTH 80
#define FIELD_HEIGHT 25
#define RACKET_LENGTH 3
#define WINNING_SCORE 21
#define BALL_SYMBOL 'O'
#define RACKET_SYMBOL '|'
#define BORDER_SYMBOL '='
#define DIVIDER_SYMBOL '|'

void clear_screen();
void render_game(int left_racket_y, int right_racket_y, int ball_x, int ball_y);
void print_info(int score_left, int score_right);

int update_left(int racket_y, char input);
int update_right(int racket_y, char input);
int update_ball_x(int ball_x, int ball_dir_x, int reset);
int update_ball_y(int ball_y, int ball_dir_y, int reset);
int update_dir_x(int ball_dir_x, int ball_x, int ball_y, int left_racket_y, int right_racket_y, int reset);
int update_dir_y(int ball_dir_y, int ball_x, int ball_y, int left_racket_y, int right_racket_y, int reset);

int check_score(int ball_x);
char get_user_input();
int is_game_over(int score_left, int score_right);
void display_game_over(int score_left, int score_right);

int main(void) {
    int left_racket_y = (FIELD_HEIGHT - RACKET_LENGTH) / 2;
    int right_racket_y = (FIELD_HEIGHT - RACKET_LENGTH) / 2;
    int ball_x = FIELD_WIDTH / 2;
    int ball_y = FIELD_HEIGHT / 2;
    int ball_dir_x = 1, ball_dir_y = 1;
    int score_left = 0, score_right = 0;
    int reset = 0;
    render_game(left_racket_y, right_racket_y, ball_x, ball_y);
    print_info(score_left, score_right);
    while (1) {
        char user_input = get_user_input();
        left_racket_y = update_left(left_racket_y, user_input);
        right_racket_y = update_right(right_racket_y, user_input);

        ball_x = update_ball_x(ball_x, ball_dir_x, reset);
        ball_y = update_ball_y(ball_y, ball_dir_y, reset);
        ball_dir_x = update_dir_x(ball_dir_x, ball_x, ball_y, left_racket_y, right_racket_y, reset);
        ball_dir_y = update_dir_y(ball_dir_y, ball_x, ball_y, left_racket_y, right_racket_y, reset);
        reset = check_score(ball_x);
        score_left += (reset == 1) ? 1 : 0;
        score_right += (reset == 2) ? 1 : 0;

        if (is_game_over(score_left, score_right)) {
            break;
        }
        render_game(left_racket_y, right_racket_y, ball_x, ball_y);
        print_info(score_left, score_right);
    }

    display_game_over(score_left, score_right);
    return 0;
}

void clear_screen(void) {
    int i = 0;
    while (i < 100) {
        printf("\n");
        i = i + 1;
    }
}

void render_game(int left_racket_y, int right_racket_y, int ball_x, int ball_y) {
    clear_screen();
    int i = 0;
    while (i < FIELD_WIDTH + 2) {
        printf("%c", BORDER_SYMBOL);
        i = i + 1;
    }
    printf("\n");

    int y = 0;
    while (y < FIELD_HEIGHT) {
        printf("%c", RACKET_SYMBOL);

        int x = 0;
        while (x < FIELD_WIDTH) {
            int is_left_racket = (x == 1) && (y >= left_racket_y) && (y < left_racket_y + RACKET_LENGTH);
            int is_right_racket =
                (x == FIELD_WIDTH - 2) && (y >= right_racket_y) && (y < right_racket_y + RACKET_LENGTH);
            int is_ball = (x == ball_x) && (y == ball_y);
            int is_divider = (x == FIELD_WIDTH / 2);

            if (is_left_racket || is_right_racket) {
                printf("%c", RACKET_SYMBOL);
            } else if (is_ball) {
                printf("%c", BALL_SYMBOL);
            } else if (is_divider) {
                printf("%c", DIVIDER_SYMBOL);
            } else {
                printf(" ");
            }

            x = x + 1;
        }

        printf("%c\n", RACKET_SYMBOL);
        y = y + 1;
    }

    i = 0;
    while (i < FIELD_WIDTH + 2) {
        printf("%c", BORDER_SYMBOL);
        i = i + 1;
    }
    printf("\n");
}

void print_info(int score_left, int score_right) {
    printf("Player 1 Score: %d - Player 2 Score: %d\n", score_left, score_right);
    printf("Controls: A/Z - Left Racket | K/M - Right Racket | Space - Skip\n");
    printf("First to %d points wins!\n", WINNING_SCORE);
}

int update_left(int racket_y, char input) {
    if (input == 'a' || input == 'A') {
        if (racket_y > 0) {
            return racket_y - 1;
        }
    } else if (input == 'z' || input == 'Z') {
        if (racket_y < FIELD_HEIGHT - RACKET_LENGTH) {
            return racket_y + 1;
        }
    }
    return racket_y;
}

int update_right(int racket_y, char input) {
    if (input == 'k' || input == 'K') {
        if (racket_y > 0) {
            return racket_y - 1;
        }
    } else if (input == 'm' || input == 'M') {
        if (racket_y < FIELD_HEIGHT - RACKET_LENGTH) {
            return racket_y + 1;
        }
    }
    return racket_y;
}

int update_ball_x(int ball_x, int ball_dir_x, int reset) {
    if (reset != 0) {
        return FIELD_WIDTH / 2;
    }
    return ball_x + ball_dir_x;
}

int update_ball_y(int ball_y, int ball_dir_y, int reset) {
    if (reset != 0) {
        return FIELD_HEIGHT / 2;
    }
    return ball_y + ball_dir_y;
}

int update_dir_x(int ball_dir_x, int ball_x, int ball_y, int left_racket_y, int right_racket_y, int reset) {
    if (reset != 0) {
        if (reset == 1) {
            return 1;
        } else if (reset == 2) {
            return -1;
        }
    }

    if (ball_x <= 2) {
        if (ball_y >= left_racket_y && ball_y < left_racket_y + RACKET_LENGTH) {
            return 1;
        }
    }

    if (ball_x >= FIELD_WIDTH - 3) {
        if (ball_y >= right_racket_y && ball_y < right_racket_y + RACKET_LENGTH) {
            return -1;
        }
    }

    return ball_dir_x;
}

int update_dir_y(int ball_dir_y, int ball_x, int ball_y, int left_racket_y, int right_racket_y, int reset) {
    if (reset != 0) {
        return (reset % 2 == 0) ? 1 : -1;
    }

    if (ball_y <= 0 || ball_y >= FIELD_HEIGHT - 1) {
        return -ball_dir_y;
    }

    if (ball_x == 2) {
        if (ball_y == left_racket_y || ball_y == left_racket_y + RACKET_LENGTH - 1) {
            if (ball_y == left_racket_y) {
                return -1;
            } else {
                return 1;
            }
        }
    }

    if (ball_x == FIELD_WIDTH - 3) {
        if (ball_y == right_racket_y || ball_y == right_racket_y + RACKET_LENGTH - 1) {
            if (ball_y == right_racket_y) {
                return -1;
            } else {
                return 1;
            }
        }
    }

    return ball_dir_y;
}

int check_score(int ball_x) {
    if (ball_x >= FIELD_WIDTH) {
        return 1;
    } else if (ball_x < 0) {
        return 2;
    }
    return 0;
}

char get_user_input(void) {
    char input;
    printf("Enter command: ");
    while (1) {
        if (scanf("%c", &input) != 1 || input != 'A' && input != 'a' && input != 'Z' && input != 'z' &&
                                            input != 'K' && input != 'k' && input != 'M' && input != 'm' &&
                                            input != ' ') {
            continue;
        } else {
            while (getchar() != '\n') {
            }
            return input;
        }
    }
}

int is_game_over(int score_left, int score_right) {
    return (score_left >= WINNING_SCORE) || (score_right >= WINNING_SCORE);
}

void display_game_over(int score_left, int score_right) {
    clear_screen();
    printf("=== GAME OVER ===\n");

    if (score_left >= WINNING_SCORE) {
        printf("Player 1 WINS! Congratulations!\n");
    } else {
        printf("Player 2 WINS! Congratulations!\n");
    }

    printf("Final score: %d - %d\n", score_left, score_right);
}
