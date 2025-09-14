#include <tetromino.h>
#include <main.h>

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

static WINDOW *field_win;
static int field[FIELD_HEIGHT][FIELD_WIDTH];
static int curTetro[4][4];
static int curX, curY;
static int enter_key = 0;

void init_screen() {
    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr, true);
    nodelay(stdscr, true);

    field_win = newwin(FIELD_HEIGHT +2, FIELD_WIDTH *2 + 2, 1, 1);
    box(field_win, 0, 0);
    wrefresh(field_win);
}

void draw_field() {
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (field[y][x]) {
                wattron(field_win, A_REVERSE);
                mvwprintw(field_win, y + 1, x * 2 + 1, "  ");
                wattroff(field_win, A_REVERSE);
            } else {
                mvwprintw(field_win, y + 1, x * 2 + 1, "  ");
            }
        }
    }
}

void draw_info() {
    int y = 2;
    int x = 24;
    mvprintw(y++, x, "level: ");
    mvprintw(y++, x, "score: ");
    mvprintw(y++, x, "goal: ");
    y++;
    mvprintw(y++, x, "*smth else*");
    y++;
    mvprintw(y++, x, "next:");
}

void draw_piece() {
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (curTetro[py][px]) {
                int fx = curX + px;
                int fy = curY + py;
                if (fy >= 0 && fy < FIELD_HEIGHT &&
                    fx >= 0 && fx < FIELD_WIDTH) {
                    wattron(field_win, A_REVERSE);
                    mvwprintw(field_win, fy + 1, fx * 2 + 1, "  ");
                    wattroff(field_win, A_REVERSE);
                }
            }
        }
    }
}

void draw() {
    box(field_win, 0, 0);
    draw_field();
    draw_piece();
    wrefresh(field_win);
    
    draw_info();
    refresh();
}

void spawnPiece() {
    int idx = rand() % TETROMINO_COUNT;
    memcpy(curTetro, tetrominoes[idx], sizeof(curTetro));
    curX = FIELD_WIDTH / 2 - 2;
    curY = 0;
}

int canMove(int nx, int ny) {
    int ok = 1;
    for (int py = 0; py < 4 && ok; py++) {
        for (int px = 0; px < 4 && ok; px++) {
            if (curTetro[py][px]) {
                int fx = nx + px;
                int fy = ny + py;
                if (fy >= FIELD_HEIGHT || fx < 0 || fx >= FIELD_WIDTH)
                    ok = 0;
                else if (fy >= 0 && field[fy][fx])
                    ok = 0;
            }
        }
    }
    return ok;
}

void placePiece() {
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (curTetro[py][px]) {
                int fx = curX + px;
                int fy = curY + py;
                if (fy >= 0 && fy < FIELD_HEIGHT &&
                    fx >= 0 && fx < FIELD_WIDTH) {
                    field[fy][fx] = 1;
                }
            }
        }
    }
}

void userKey () {
    int ch = getch();
    switch(ch) {
        case KEY_ENTER:
        case '\n':
            enter_key = 1;
            break;
        // case KEY_BREAK: пауза
        // Завершение игры - ? q?
        case KEY_LEFT:
            if (canMove(curX-1, curY)) curX--;
            break;
        case KEY_RIGHT:
            if (canMove(curX+1, curY)) curX++;
            break;
        case KEY_DOWN:
            if (canMove(curX, curY+1)) curY++;
            break;
        // вращение фигуры - r?
        default:
            break;
    }
}

int main() {
    srand(time(NULL));
    memset(field, 0, sizeof(field));
    init_screen();
    do {
        clear();
        mvprintw(10, 10, "PRESS \"ENTER\" TO PLAY THE GAME");
        refresh();
        userKey();
        napms(50);
    } while (enter_key == 0);
    
    spawnPiece();

    int tick = 0;
    int running = 1;
    while (running) {
        userKey();
        napms(50);
        tick++;

        if (tick % 5 == 0) {
            if (canMove(curX, curY + 1)) {
                curY++;
            } else {
                placePiece();
                spawnPiece();
                if (!canMove(curX, curY)) {
                    running = 0; /* game over */
                }
            }
        }
        draw();
    }

    nodelay(stdscr, false);
    refresh();
    getch();
    endwin();
    return 0;
}
