#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <windows.h>
#include <time.h>

#define MAP_HEIGHT 20
#define MAP_WIDTH 60
#define EMPTY ' '
#define MAP_FRAME '#'
#define APPLE '*'
#define SNAKE 'o'

typedef struct{
    int x, y;
} TObject;

char map[MAP_HEIGHT][MAP_WIDTH + 1];
int x[1044];
int y[1044];
TObject apple;

int snakeLength = 3;
bool left = false;
bool right = true;
bool up = false;
bool down = false;
bool inGame = true;

void initMap() {
    for (int i = 0; i < MAP_WIDTH; i++)
        map[0][i] = MAP_FRAME;
    map[0][MAP_WIDTH] = '\0';
    sprintf(map[MAP_HEIGHT - 1], map[0]);
    for (int j = 0; j < MAP_WIDTH; j++){
        if (j == 0 || j == MAP_WIDTH - 1)
            map[1][j] = MAP_FRAME;
        else
            map[1][j] = EMPTY;
    }
    map[1][MAP_WIDTH] = '\0';
    for (int k = 2; k < MAP_HEIGHT - 1; k++)
        sprintf(map[k], map[1]);
}

void showMap() {
    for (int i = 0; i < MAP_HEIGHT; i++)
        printf("%s\n", map[i]);
}

void initSnake() {
    for (int i = 0; i < snakeLength; i++) {
        x[i] = 5 - i;
        y[i] = 5;
    }
}

void putSnakeOnMap() {
    for (int i = 0; i < snakeLength; i++)
        map[y[i]][x[i]] = SNAKE;
}

void SetCur(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void moveSnake() {
    for (int i = snakeLength; i > 0; i--) {
        x[i] = x[i - 1];
        y[i] = y[i - 1];
        map[y[i]][x[i]] = EMPTY;
    }
    if (left) x[0]--;
    if (right) x[0]++;
    if (up) y[0]--;
    if (down) y[0]++;
}

void checkKeyState() {
    if (GetKeyState('D') < 0 && left != true) {
        right = true;
        up = false;
        down = false;
    }
    if (GetKeyState('A') < 0 && right != true) {
        left = true;
        up = false;
        down = false;
    }
    if (GetKeyState('W') < 0 && down != true) {
        right = false;
        up = true;
        left = false;
    }
    if (GetKeyState('S') < 0 && up != true) {
        right = false;
        down = true;
        left = false;
    }
}

void createApple(TObject *obj);

void checkCollisions() {
    int head_x = x[0];
    int head_y = y[0];
    if (map[head_y][head_x] == MAP_FRAME) inGame = false;
    if (map[head_y][head_x] == APPLE) {
        snakeLength++;
        createApple(&apple);
    }
    for (int i = snakeLength; i > 0; i--) {
        if (i > 4 && head_x == x[i] && head_y == y[i]) {
            inGame = false;
        }
    }
}

void createApple(TObject *obj) {
    obj->x = 3 + rand() % 53;
    obj->y = 2 + rand() % 16;
    map[obj->y][obj->x] = APPLE;
}

int main() {

    srand(time(NULL));
    initSnake();
    initMap();
    createApple(&apple);
    do {
        SetCur(0, 0);
        putSnakeOnMap();
        showMap();
        checkKeyState();
        moveSnake();
        checkCollisions();
        Sleep(100);
    }while (inGame && GetKeyState(VK_ESCAPE) >= 0);

    return 0;
}
