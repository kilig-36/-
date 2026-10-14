#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define WIDTH  20   // 地图宽度
#define HEIGHT 20   // 地图高度

typedef struct {
    int x, y;
} Point;

int gameover;
int score;
Point snake[WIDTH * HEIGHT];
int snake_len;
Point food;
int dir_x, dir_y;   // 当前移动方向

void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void hide_cursor() {
    CONSOLE_CURSOR_INFO ci;
    ci.dwSize = 100;
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

void draw() {
    int i, j;
    gotoxy(0, 0);
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                printf("#");          // 边界
            }
            else if (i == food.y && j == food.x) {
                printf("$");          // 食物
            }
            else if (i == snake[0].y && j == snake[0].x) {
                printf("@");          // 蛇头
            }
            else {
                int is_body = 0;
                int k;
                for (k = 1; k < snake_len; k++) {
                    if (snake[k].y == i && snake[k].x == j) {
                        printf("o");  // 蛇身
                        is_body = 1;
                        break;
                    }
                }
                if (!is_body) printf(" ");
            }
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
    printf("WASD/方向键移动, q退出\n");
}

void gen_food() {
    int ok = 0;
    while (!ok) {
        food.x = rand() % (WIDTH - 2) + 1;
        food.y = rand() % (HEIGHT - 2) + 1;
        ok = 1;
        int k;
        for (k = 0; k < snake_len; k++) {
            if (snake[k].x == food.x && snake[k].y == food.y) {
                ok = 0;
                break;
            }
        }
    }
}

void init() {
    gameover = 0;
    score = 0;
    snake_len = 1;
    snake[0].x = WIDTH / 2;
    snake[0].y = HEIGHT / 2;
    dir_x = 1; dir_y = 0;
    srand((unsigned)GetTickCount());
    gen_food();
    hide_cursor();
}

void step() {
    int i;
    // 保存旧尾（吃到食物时加长用）
    Point tail = snake[snake_len - 1];

    // 蛇身前移（从尾到头）
    for (i = snake_len - 1; i > 0; i--)
        snake[i] = snake[i - 1];

    // 蛇头前进
    snake[0].x += dir_x;
    snake[0].y += dir_y;

    // 撞墙判定
    if (snake[0].x <= 0 || snake[0].x >= WIDTH - 1 ||
        snake[0].y <= 0 || snake[0].y >= HEIGHT - 1) {
        gameover = 1;
        return;
    }

    // 撞自己判定
    for (i = 1; i < snake_len; i++) {
        if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
            gameover = 1;
            return;
        }
    }

    // 吃到食物
    if (snake[0].x == food.x && snake[0].y == food.y) {
        score += 10;
        snake[snake_len] = tail;   // 把旧尾加回来加长
        snake_len++;
        gen_food();
    }
}

void input() {
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 224 || ch == 0) {          // 方向键
            ch = _getch();
            switch (ch) {
                case 72: if (dir_y == 0) { dir_x = 0; dir_y = -1; } break; // 上
                case 80: if (dir_y == 0) { dir_x = 0; dir_y =  1; } break; // 下
                case 75: if (dir_x == 0) { dir_x = -1; dir_y = 0; } break; // 左
                case 77: if (dir_x == 0) { dir_x =  1; dir_y = 0; } break; // 右
            }
        } else {
            switch (ch) {
                case 'w': case 'W': if (dir_y == 0) { dir_x = 0; dir_y = -1; } break;
                case 's': case 'S': if (dir_y == 0) { dir_x = 0; dir_y =  1; } break;
                case 'a': case 'A': if (dir_x == 0) { dir_x = -1; dir_y = 0; } break;
                case 'd': case 'D': if (dir_x == 0) { dir_x =  1; dir_y = 0; } break;
                case 'q': case 'Q': gameover = 1; break;
            }
        }
    }
}

int main() {
    init();
    while (!gameover) {
        draw();
        input();
        step();
        Sleep(120);   // 速度，越小越快
    }
    gotoxy(0, HEIGHT + 2);
    printf("Game Over! 得分: %d\n", score);
    return 0;
}
