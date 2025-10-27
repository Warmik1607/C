#include <ncurses.h>
#include <stdlib.h>

#define FIELD_WIDTH 80
#define FIELD_HEIGHT 25
#define RACKET_LENGTH 3
#define WINNING_SCORE 21
#define BALL_SYMBOL 'O'
#define RACKET_SYMBOL '|'
#define BORDER_SYMBOL '='
#define DIVIDER_SYMBOL '|'

void render_game(int left_racket_y, int right_racket_y, int ball_x, int ball_y);
void print_info(int score_left, int score_right);

int update_left(int racket_y, char input);
int update_right(int racket_y, char input);
int update_ball_x(int ball_x, int ball_dir_x, int reset);
int update_ball_y(int ball_y, int ball_dir_y, int reset);
int update_dir_x(int ball_dir_x, int ball_x, int ball_y, int left_racket_y, int right_racket_y, int reset);
int update_dir_y(int ball_dir_y, int ball_x, int ball_y, int left_racket_y, int right_racket_y, int reset);

int check_score(int ball_x);
char get_user_input(void);
int is_game_over(int score_left, int score_right);
void display_game_over(int score_left, int score_right);

int main(void) {
    initscr();
    noecho();
    curs_set(0);
    timeout(0);
    int left_racket_y = (FIELD_HEIGHT - RACKET_LENGTH) / 2;
    int right_racket_y = (FIELD_HEIGHT - RACKET_LENGTH) / 2;
    int ball_x = FIELD_WIDTH / 2;
    int ball_y = FIELD_HEIGHT / 2;
    int ball_dir_x = 1, ball_dir_y = 1;
    int score_left = 0, score_right = 0;
    int reset = 0;
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
        napms(80);
    }
    display_game_over(score_left, score_right);
    timeout(-1);
    getch();
    endwin();

    return 0;
}

void render_game(int left_racket_y, int right_racket_y, int ball_x, int ball_y) {
    clear();
    for (int i = 0; i < FIELD_WIDTH + 2; i++) {
        mvaddch(0, i, BORDER_SYMBOL);
    }
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        mvaddch(y + 1, 0, RACKET_SYMBOL);

        for (int x = 0; x < FIELD_WIDTH; x++) {
            int is_left_racket = (x == 1) && (y >= left_racket_y) && (y < left_racket_y + RACKET_LENGTH);
            int is_right_racket =
                (x == FIELD_WIDTH - 2) && (y >= right_racket_y) && (y < right_racket_y + RACKET_LENGTH);
            int is_ball = (x == ball_x) && (y == ball_y);
            int is_divider = (x == FIELD_WIDTH / 2);

            if (is_left_racket || is_right_racket) {
                mvaddch(y + 1, x + 1, RACKET_SYMBOL);
            } else if (is_ball) {
                mvaddch(y + 1, x + 1, BALL_SYMBOL);
            } else if (is_divider) {
                mvaddch(y + 1, x + 1, DIVIDER_SYMBOL);
            } else {
                mvaddch(y + 1, x + 1, ' ');
            }
        }

        mvaddch(y + 1, FIELD_WIDTH + 1, RACKET_SYMBOL);
    }
    for (int i = 0; i < FIELD_WIDTH + 2; i++) {
        mvaddch(FIELD_HEIGHT + 1, i, BORDER_SYMBOL);
    }

    refresh();
}

void print_info(int score_left, int score_right) {
    mvprintw(FIELD_HEIGHT + 3, 0, "Player 1 Score: %d - Player 2 Score: %d", score_left, score_right);
    mvprintw(FIELD_HEIGHT + 4, 0, "Controls: A/Z - Left Racket | K/M - Right Racket");
    mvprintw(FIELD_HEIGHT + 5, 0, "First to %d points wins! Press 'q' to quit", WINNING_SCORE);
    refresh();
}

char get_user_input(void) {
    int ch = getch();
    if (ch == 'q' || ch == 'Q') {
        endwin();
        exit(0);
    }

    return (char)ch;
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
    int new_dir_x = ball_dir_x;

    if (reset != 0) {
        if (reset == 1) {
            new_dir_x = 1;
        } else if (reset == 2) {
            new_dir_x = -1;
        }
    } else if (ball_x <= 2) {
        if (ball_y >= left_racket_y && ball_y < left_racket_y + RACKET_LENGTH) {
            new_dir_x = 1;
        }
    } else if (ball_x >= FIELD_WIDTH - 3) {
        if (ball_y >= right_racket_y && ball_y < right_racket_y + RACKET_LENGTH) {
            new_dir_x = -1;
        }
    }

    return new_dir_x;
}

int update_dir_y(int ball_dir_y, int ball_x, int ball_y, int left_racket_y, int right_racket_y, int reset) {
    int new_dir_y = ball_dir_y;

    if (reset != 0) {
        new_dir_y = (reset % 2 == 0) ? 1 : -1;
    } else if (ball_y <= 0 || ball_y >= FIELD_HEIGHT - 1) {
        new_dir_y = -ball_dir_y;
    } else if (ball_x == 2) {
        if (ball_y == left_racket_y || ball_y == left_racket_y + RACKET_LENGTH - 1) {
            if (ball_y == left_racket_y) {
                new_dir_y = -1;
            } else {
                new_dir_y = 1;
            }
        }
    } else if (ball_x == FIELD_WIDTH - 3) {
        if (ball_y == right_racket_y || ball_y == right_racket_y + RACKET_LENGTH - 1) {
            if (ball_y == right_racket_y) {
                new_dir_y = -1;
            } else {
                new_dir_y = 1;
            }
        }
    }

    return new_dir_y;
}

int check_score(int ball_x) {
    int result = 0;

    if (ball_x >= FIELD_WIDTH) {
        result = 1;
    } else if (ball_x < 0) {
        result = 2;
    }

    return result;
}

int is_game_over(int score_left, int score_right) {
    return (score_left >= WINNING_SCORE) || (score_right >= WINNING_SCORE);
}

void display_game_over(int score_left, int score_right) {
    clear();

    mvprintw(5, 30, "=== GAME OVER ===");

    if (score_left >= WINNING_SCORE) {
        mvprintw(7, 25, "Player 1 WINS! Congratulations!");
    } else {
        mvprintw(7, 25, "Player 2 WINS! Congratulations!");
    }

    mvprintw(9, 35, "Final score: %d - %d", score_left, score_right);
    mvprintw(12, 30, "Press any key to exit");

    refresh();
}