// SnakeGame.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <vector>
#include <iostream>
#include <windows.h>
#include <conio.h>

#define snake_symbol '#'
#define fruit_symbol '0'

#define map_scale_Y 20
#define map_scale_X 41

using namespace std;

class Snake {
public:
    vector<int> curr_pos = {};
    vector<int> prev_pos = {};
    Snake* prev_Snake;
    Snake* next_Snake;

    Snake(int x, int y) {
        curr_pos.push_back(x);
        curr_pos.push_back(y);

        prev_Snake = nullptr;
        next_Snake = nullptr;
    }
};

void drawMap(vector<vector<char>> map, int totalFruits) {
    string str = "";
    str.reserve(map_scale_X * (map_scale_Y + 1));

    for (int i = 0; i < map_scale_Y; i++) {
        for (int j = 0; j < map_scale_X; j++) {
            str += map[i][j];
        }
        str += "\n";
    }

    str += "Score: \n";
    cout << str << flush;
}

void setupMap(vector<vector<char>> &map) {
    for (int i = 0; i < map_scale_Y; i++) {
        for (int j = 0; j < map_scale_X; j++) {
            if (j == map_scale_X-1) {
                map[i][j] = '|';
                continue;
            }
            map[i][j] = ' ';
        }
    }
}

void spawnFruit(vector<vector<char>> &map) {
    // Check fruit already exist and if is it then leave from function
    for (int i = 0; i < map_scale_Y; i++) {
        for (int j = 0; j < map_scale_X; j++) {
            if (map[i][j] == fruit_symbol)
                return;
        }
    }
    
    int randomPlaceX = rand() % (map_scale_X - 1);
    int randomPlaceY = rand() % (map_scale_Y);
    map[randomPlaceY][randomPlaceX] = fruit_symbol;
}

void spawnSnake(vector<vector<char>>& map) {
    int randomPlaceX = rand() % (map_scale_X - 1);
    int randomPlaceY = rand() % (map_scale_Y);
    map[randomPlaceY][randomPlaceX] = snake_symbol;
}

void moveSnake(vector<vector<char>>& map, int isKeyPressed, int* p_totalFruits) {
    int snake_pos_X, snake_pos_Y = 0;

    // find snake
    for (int i = 0; i < map_scale_Y; i++) {
        for (int j = 0; j < map_scale_X; j++) {
            if (map[i][j] == snake_symbol) {
                snake_pos_X = j;
                snake_pos_Y = i;
                map[i][j] = ' ';
            }
        }
    }

    switch (isKeyPressed) {
        case 0:
            if (snake_pos_Y == 0)
                snake_pos_Y = map_scale_Y - 1;
            else
                snake_pos_Y -= 1;
            break;
        case 1:
            if (snake_pos_X == 0)
                snake_pos_X = map_scale_X - 2;
            else
                snake_pos_X -= 1;
            break;
        case 2:
            if (snake_pos_Y == map_scale_Y - 1)
                snake_pos_Y = 0;
            else
                snake_pos_Y += 1;
            break;
        case 3:
            if (snake_pos_X == map_scale_X - 2)
                snake_pos_X = 0;
            else
                snake_pos_X += 1;
            break;
        default:
            break;
    }

    for (int i = 0; i < map_scale_Y; i++) {
        for (int j = 0; j < map_scale_X; j++) {
            if (map[i][j] == fruit_symbol && j == snake_pos_X && i == snake_pos_Y) {
                map[i][j] = ' ';
                *p_totalFruits += 1;
            }
        }
    }

    map[snake_pos_Y][snake_pos_X] = snake_symbol;
}

int main()
{
    HANDLE buff = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD sizeOfBuff;
    sizeOfBuff.X = map_scale_X;
    sizeOfBuff.Y = map_scale_Y;
    SetConsoleScreenBufferSize(buff, sizeOfBuff);
    system("mode con cols=41 lines=21");

    srand(time(0));
    vector<vector<char>> map(map_scale_Y, vector<char> (map_scale_X));
    int isKeyPressed = 2;
    int totalFruits = 0;

    setupMap(map);
    spawnSnake(map);
    spawnFruit(map);
    drawMap(map, totalFruits);

    /*while (true) {
        cout << (GetAsyncKeyState(87) != 0) << endl;
    }*/

    _getch();
    while (true) {
        
        if (GetAsyncKeyState(87) != 0)
            isKeyPressed = 0;
        if (GetAsyncKeyState(65) != 0)
            isKeyPressed = 1;
        if (GetAsyncKeyState(83) != 0)
            isKeyPressed = 2;
        if (GetAsyncKeyState(68) != 0)
            isKeyPressed = 3;

        spawnFruit(map);
        moveSnake(map, isKeyPressed, &totalFruits);
        drawMap(map, totalFruits);
        Sleep(150);
    }
}