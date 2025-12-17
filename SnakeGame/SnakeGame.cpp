// SnakeGame.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <vector>
#include <iostream>
#include <windows.h>
#include <conio.h>

#define snake_symbol '#'
#define fruit_symbol '0'

#define map_scale_Y 20
#define map_scale_X 41

#define arrow_LEFT 75
#define arrow_RIGHT 77
#define arrow_UP 72
#define arrow_DOWN 80

using namespace std;

bool game = true;

class Snake {
private:
    vector<int> curr_pos = { 0, 0 };
    vector<int> prev_pos = { 0, 0 };

public:
    Snake* next_Snake;

    Snake(int x, int y) {
        curr_pos[0] = x;
        curr_pos[1] = y;

        next_Snake = nullptr;
    }

    ~Snake() {}

    void add(int x, int y) {
        Snake* temp = this;

        while (temp->next_Snake != nullptr) {
            temp = temp->next_Snake;
        }

        if (temp->next_Snake == nullptr) {
            temp->next_Snake = new Snake(temp->prev_pos[0], temp->prev_pos[1]);
        }
    }

    void set_new_position(int x, int y) {
        this->prev_pos[0] = curr_pos[0];
        this->prev_pos[1] = curr_pos[1];
        this->curr_pos[0] = x;
        this->curr_pos[1] = y;

        if (this->next_Snake != nullptr) {
            this->next_Snake->set_new_position(this->prev_pos[0], this->prev_pos[1]);
        }
    }

    vector<int> get_curr_position() {
        return this->curr_pos;
    }
};

void DrawMap(vector<vector<char>> map, int totalFruits) {
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

void SetupMap(vector<vector<char>> &map) {
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

void SpawnFruit(vector<vector<char>> &map) {
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

Snake* SpawnSnake(vector<vector<char>>& map) {
    int randomPlaceX = rand() % (map_scale_X - 1);
    int randomPlaceY = rand() % (map_scale_Y);

    Snake* snake = new Snake(randomPlaceX, randomPlaceY);

    map[snake->get_curr_position()[1]][snake->get_curr_position()[0]] = snake_symbol;

    return snake;
}

void MoveSnake(vector<vector<char>>& map, int KeyPressedCode, int* p_totalFruits, Snake* p_snake) {
    
}

int main()
{
    system("mode con cols=41 lines=21");
    srand(time(0));

    vector<vector<char>> map(map_scale_Y, vector<char> (map_scale_X));

    int KeyPressedCode = 0;
    vector<int> keysCode = { arrow_UP , arrow_LEFT , arrow_DOWN , arrow_RIGHT };

    int totalFruits = 0;

    SetupMap(map);
    Snake* snake = SpawnSnake(map);
    SpawnFruit(map);
    DrawMap(map, totalFruits);

    KeyPressedCode = _getch();

    while (game) {
        
        for (auto keyCode : keysCode)
            if (GetAsyncKeyState(keyCode) != 0)
                KeyPressedCode = keyCode;

        SpawnFruit(map);
        MoveSnake(map, KeyPressedCode, &totalFruits, snake);
        DrawMap(map, totalFruits);
        Sleep(150);
    }

    cout << "GAME OVER!" << endl;
}