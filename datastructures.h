#pragma once

#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <GL/gl.h>
#include <GL/glut.h>

#include <vector>


////////////////////////// Other
struct Time {
    int hours;
    int minutes;
    int seconds;
};


struct Leaders {
    char name[21];
    Time time;
};


////////////////////////// Game scene
enum GameSceneType {
    GameScene_MainMenu = 0,
    GameScene_Leaders,
    GameScene_NewGame,
    GameScene_MakeGame,
    GameScene_Game,
    GameScene_Liders,
    GameScene_Wictory,
    GameScene_AmountScenes
};

struct GameScene {
    void (*renderFunction)(void);
    void (*mouseFunction)(int x, int y);
    void (*keyboardFunction)(unsigned char key, int x, int y);
    void (*mouseClickFunction)(int button, int state, int x, int y);
};


////////////////////////// Mouse and buttons
enum ButtonID {
    Button_NoButton,
    Button_NewGame,
    Button_Load,
    Button_Liders,
    Button_Ok,
    Button_Yahoo,
    Button_MainMenu,
    Button_Exit,

    Button_EasyGame,
    Button_MediumGame,
    Button_HardGame,
    Button_UserGame,

    Button_Width_add,
    Button_Width_sub,
    Button_Height_add,
    Button_Height_sub,
    Button_Mines_add,
    Button_Mines_sub,

    ButtonsAmount
};

struct GameMouse {
    int x;
    int y;
    bool mouseOnCell;
    int cell_x;
    int cell_y;
    bool mouseOnButton;
    ButtonID buttonId;
    bool mouseLeftClick;
    bool mouseRightClick;
};

////////////////////////// Game cell
enum GameCellType {
    Cell_bomb = -1,
    Cell_free = 0,
    Cell_1 = 1,
    Cell_2 = 2,
    Cell_3 = 3,
    Cell_4 = 4,
    Cell_5 = 5,
    Cell_6 = 6,
    Cell_7 = 7,
    Cell_8 = 8
};

struct GameCell {
    bool isOpened;
    bool isSuspect;
    GameCellType type;
};


////////////////////////// Game
struct GameField {
    std::vector<std::vector<GameCell>> field;
    std::vector<std::pair<int, int>> mines;
    int width;
    int height;
    int bombsAmount;
    int flagsAmount;
};

struct Game {
    GameField field;
    Time time;
    int gameID;
    bool gameOver;
};

struct GameFieldSetings {
    int width;
    int height;
    int minesAmount;
};


#endif // DATASTRUCTURES_H
