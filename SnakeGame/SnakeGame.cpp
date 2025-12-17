#include <vector>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>

#define snake_body_symbol '#'
#define snake_head_symbol '%'
#define snake_dead_symbol 'X'
#define fruit_symbol '0'

#define map_scale_Y 20
#define map_scale_X 40

#define arrow_LEFT 37
#define arrow_RIGHT 39
#define arrow_UP 38
#define arrow_DOWN 40

using namespace std;

bool game = true;

// singly linked list
class Snake {
private:
    vector<int> curr_pos = { 0, 0 };
    vector<int> prev_pos = { 0, 0 };

public:
    Snake* next_Snake;
    bool isHead = false;

    Snake(int x, int y, bool _isHead) {
        curr_pos[0] = x;
        curr_pos[1] = y;

        next_Snake = nullptr;

        // cosmetic option
        isHead = _isHead;
    }

    ~Snake() {}

    void add() {
        Snake* temp = this;

        while (temp->next_Snake != nullptr) {
            temp = temp->next_Snake;
        }

        if (temp->next_Snake == nullptr) {
            temp->next_Snake = new Snake(temp->prev_pos[0], temp->prev_pos[1], false);
        }
    }

    void add(int x, int y) {
        Snake* temp = this;

        while (temp->next_Snake != nullptr) {
            temp = temp->next_Snake;
        }

        if (temp->next_Snake == nullptr) {
            temp->next_Snake = new Snake(x, y, false);
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
    // it's easier for the console to output everything with a single string than with several chars, as I understand it...
    // (i wanted to use the console buffer)
    string str = "";
    str.reserve(map_scale_X * (map_scale_Y + 1));

    for (int i = 0; i < map_scale_Y; i++) {
        for (int j = 0; j < map_scale_X; j++) {
            str += map[i][j];
        }
        str += "\n";
    }

    str += "Score: " + to_string(totalFruits) + "\n";
    cout << str << flush;
}

void game_over(vector<vector<char>>& map) {
    game = false;
    for (int y = 0; y < map_scale_Y; y++) {
        for (int x = 0; x < map_scale_X; x++) {
            if (map[y][x] == snake_head_symbol) {
                map[y][x] = snake_dead_symbol;
            }
        }
    }
}

void SetupMap(vector<vector<char>>& map) {
    for (int i = 0; i < map_scale_Y; i++) {
        for (int j = 0; j < map_scale_X; j++) {
            map[i][j] = ' ';
        }
    }
}

void SpawnFruit(vector<vector<char>>& map) {
    // check fruit already exist and if is it then leave from function
    for (int i = 0; i < map_scale_Y; i++) {
        for (int j = 0; j < map_scale_X; j++) {
            if (map[i][j] == fruit_symbol)
                return;
        }
    }

    // take random values
    int randomPlaceX = rand() % (map_scale_X - 2);
    int randomPlaceY = rand() % (map_scale_Y - 1);

    // if there is something on these values, then change them until it is empty
    while (map[randomPlaceY][randomPlaceX] == snake_body_symbol or map[randomPlaceY][randomPlaceX] == snake_head_symbol) {
        randomPlaceX = rand() % (map_scale_X - 2);
        randomPlaceY = rand() % (map_scale_Y - 1);
    }

    // spawn fruit
    map[randomPlaceY][randomPlaceX] = fruit_symbol;
}

Snake* SpawnSnake(vector<vector<char>>& map) {
    int randomPlaceX = rand() % (map_scale_X - 2);
    int randomPlaceY = rand() % (map_scale_Y - 1);

    Snake* snake = new Snake(randomPlaceX, randomPlaceY, true);
    snake->add(randomPlaceX, randomPlaceY - 1);

    map[randomPlaceY][randomPlaceX] = snake_head_symbol;
    map[randomPlaceY - 1][randomPlaceX] = snake_body_symbol;

    return snake;
}

void MoveSnake(vector<vector<char>>& map, int KeyPressedCode, int* p_totalFruits, Snake* p_snake) {
    vector<int> curr_pos = p_snake->get_curr_position();

    switch (KeyPressedCode) {
    case arrow_DOWN:
        curr_pos[1] += 1; break;
    case arrow_UP:
        curr_pos[1] -= 1; break;
    case arrow_RIGHT:
        curr_pos[0] += 1; break;
    case arrow_LEFT:
        curr_pos[0] -= 1; break;
    default: break;
    }

    // no borders logistic
    if (curr_pos[0] == map_scale_X - 1) { curr_pos[0] = 0; }
    if (curr_pos[0] < 0) { curr_pos[0] = map_scale_X - 1; }
    if (curr_pos[1] == map_scale_Y) { curr_pos[1] = 0; }
    if (curr_pos[1] < 0) { curr_pos[1] = map_scale_Y - 1; }

    // if the snake's body is in front of it, then we launch a "game over"
    if (map[curr_pos[1]][curr_pos[0]] == snake_body_symbol) { game_over(map); return; }

    // if there is a fruit in front of the snake, then create a new body and remove the fruit
    if (map[curr_pos[1]][curr_pos[0]] == fruit_symbol) { p_snake->add(); *p_totalFruits = *p_totalFruits + 1; map[curr_pos[1]][curr_pos[0]] = ' '; }

    // moving the snake
    p_snake->set_new_position(curr_pos[0], curr_pos[1]);

    // clear snake from map
    for (int y = 0; y < map_scale_Y; y++) {
        for (int x = 0; x < map_scale_X; x++) {
            if (map[y][x] == snake_body_symbol or map[y][x] == snake_head_symbol) {
                map[y][x] = ' ';
            }
        }
    }

    // draw new snake on map
    Snake* temp = p_snake;
    while (temp != nullptr) {
        for (int y = 0; y < map_scale_Y; y++) {
            for (int x = 0; x < map_scale_X; x++) {
                if (x == temp->get_curr_position()[0] && y == temp->get_curr_position()[1]) {
                    map[y][x] = temp->isHead ? snake_head_symbol : snake_body_symbol;
                }
            }
        }

        temp = temp->next_Snake;
    }
}

int main()
{
    system("mode con cols=41 lines=23");
    srand(time(0));

    vector<vector<char>> map(map_scale_Y, vector<char>(map_scale_X));

    int KeyPressedCode = arrow_DOWN;
    vector<int> keysCode = { arrow_UP , arrow_LEFT , arrow_DOWN , arrow_RIGHT };

    int totalFruits = 0;

    SetupMap(map);
    Snake* snake = SpawnSnake(map);
    SpawnFruit(map);
    DrawMap(map, totalFruits);

    cout << "press any button to start\n";
    _getch();
    system("mode con cols=41 lines=22");

    while (game) {

        for (auto keyCode : keysCode) {
            if (GetAsyncKeyState(keyCode) != 0) {
                if (keyCode == arrow_DOWN && KeyPressedCode == arrow_UP) { continue; }
                if (keyCode == arrow_UP && KeyPressedCode == arrow_DOWN) { continue; }
                if (keyCode == arrow_RIGHT && KeyPressedCode == arrow_LEFT) { continue; }
                if (keyCode == arrow_LEFT && KeyPressedCode == arrow_RIGHT) { continue; }

                KeyPressedCode = keyCode;
            }
        }

        SpawnFruit(map);
        MoveSnake(map, KeyPressedCode, &totalFruits, snake);
        DrawMap(map, totalFruits);
        Sleep(150 - totalFruits);
    }

    cout << "GAME OVER!" << endl;
}