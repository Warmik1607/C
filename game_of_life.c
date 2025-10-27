#include <ncurses.h>
#include <stdio.h>

#define H 25
#define W 80

void init_field(int current_field[H][W]);
void draw_field(int current_field[H][W], int delay);
void change_field(int current_field[H][W], int new_field[H][W]);
int count_neighbors(int current_field[H][W], int main_y, int main_x);
void copy_field(int current_field[H][W], int new_field[H][W]);
void handle_user_input(int *flag_exit, int *delay);
void initialize_game(void);
int get_user_choice(void);
void load_field_pattern(int choice, int current_field[H][W]);

int main(void) {
    int current_field[H][W], new_field[H][W];
    int delay = 1000, flag_exit = 0;
    int choice = get_user_choice();

    load_field_pattern(choice, current_field);
    initialize_game();

    while (flag_exit != 1) {
        draw_field(current_field, delay);
        handle_user_input(&flag_exit, &delay);
        change_field(current_field, new_field);
        copy_field(current_field, new_field);
        napms(delay);
    }

    endwin();
    return 0;
}

int get_user_choice(void) {
    int choice = 0, flag_choice = 0;
    while (!flag_choice) {
        printf(
            "Выберите шаблон для инициализации поля:\n1. Космический корабль\n2. Планер\n3. Ружье\n4. "
            "Улей\n5. Флот планеров\nВыбор >> ");
        choice = getchar();

        while (getchar() != '\n') {
        }

        if (choice >= '1' && choice <= '5') {
            flag_choice = 1;
        } else {
            printf("Неверный ввод. Выберите цифру от 1 до 5 >> ");
        }
    }
    return choice;
}

void initialize_game(void) {
    initscr();
    cbreak();
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(0);
}
void load_field_pattern(int choice, int current_field[H][W]) {
    const char *filename = NULL;

    switch (choice) {
        case '1':
            filename = "Космический корабль.txt";
            break;
        case '2':
            filename = "Планер.txt";
            break;
        case '3':
            filename = "Ружье.txt";
            break;
        case '4':
            filename = "Улей.txt";
            break;
        case '5':
            filename = "Флот планеров.txt";
            break;
        default:
            return;
    }

    if (filename != NULL) {
        FILE *file = fopen(filename, "r");
        if (file != NULL) {
            FILE *original_stdin = stdin;

            stdin = file;

            init_field(current_field);

            stdin = original_stdin;

            fclose(file);
        }
    }
}
void handle_user_input(int *flag_exit, int *delay) {
    int input_char = getch();
    if (input_char == ' ') {
        *flag_exit = 1;
    } else if ((input_char == 'a' || input_char == 'A') && *delay > 100) {
        *delay -= 100;
    } else if ((input_char == 'z' || input_char == 'Z') && *delay < 1500) {
        *delay += 100;
    }
    flushinp();
}

void draw_field(int current_field[H][W], int delay) {
    clear();
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            if (current_field[i][j] == 1) {
                mvaddch(i, j, '*');
            } else {
                mvaddch(i, j, '.');
            }
        }
    }
    float speed = delay / 1000.0;
    mvprintw(26, 0, "Skorost' = %.1fs", speed);
    refresh();
}

void init_field(int current_field[H][W]) {
    char buffer[W + 2];
    for (int i = 0; i < H; ++i) {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            for (int j = 0; j < W && buffer[j] != '\0' && buffer[j] != '\n'; ++j) {
                current_field[i][j] = buffer[j] - '0';
            }
        }
    }
}

void change_field(int current_field[H][W], int new_field[H][W]) {
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            int neighb_count = count_neighbors(current_field, i, j);
            if (current_field[i][j] == 1) {
                if (neighb_count == 2 || neighb_count == 3) {
                    new_field[i][j] = 1;
                } else {
                    new_field[i][j] = 0;
                }
            } else {
                if (neighb_count == 3) {
                    new_field[i][j] = 1;
                } else {
                    new_field[i][j] = 0;
                }
            }
        }
    }
}

int count_neighbors(int current_field[H][W], int main_y, int main_x) {
    int count = 0;
    int dy, dx;

    for (dy = -1; dy <= 1; dy++) {
        for (dx = -1; dx <= 1; dx++) {
            if (!(dy == 0 && dx == 0)) {
                int neighb_y = main_y + dy;
                int neighb_x = main_x + dx;

                if (neighb_y < 0) {
                    neighb_y = H - 1;
                } else if (neighb_y >= H) {
                    neighb_y = 0;
                }

                if (neighb_x < 0) {
                    neighb_x = W - 1;
                } else if (neighb_x >= W) {
                    neighb_x = 0;
                }

                count = count + current_field[neighb_y][neighb_x];
            }
        }
    }

    return count;
}

void copy_field(int main[H][W], int buff[H][W]) {
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            main[i][j] = buff[i][j];
        }
    }
}