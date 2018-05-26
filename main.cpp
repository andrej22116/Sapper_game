#include <iostream>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <cstdlib>
#include <string>
#include <queue>
#include <string.h>
#include <globalvariables.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

//===========================INITIALIZE FUNCTIONS===========================//
void initializationGame();
void initializationGlut();
void initializationGl();


//=============================CONTROL FUNCTIONS============================//
void mainRendererFunction();
void mainKeyboardFunction(unsigned char key, int x, int y);
void mainMouseClickFunction(int button, int state, int x, int y);
void mainMouseMotionFunction(int x, int y);
void mainResizeFunction(int width, int height);


//=============================DRAWING FUNCTIONS============================//
// Header drawing
void drawNumber(char numSymbol, int x, int y);
void drawEmptyNumber(int x, int y);
void drawTwoHopers(int x, int y);
int drawNumberString(char* numString, int x, int y);
void drawFlagsLimits();
void drawTime();
void drawHeader();
// Controls drawing
void drawButton(int x, int y, int width, int height, ButtonID id);
// Field drawing
void drawCell(int x, int y);
void drawField();
void drawMine(int x, int y);
void drawCellNumber(int x, int y);
void drawFillCell(int x, int y);
void drawCellFlag(int x, int y);

void drawWidthController();
void drawHeightController();
void drawMinesController();

void drawCells();

// Other
void drawGameOver();
void drawLeadersTable(int x, int y);
void drawText(const char* text, float x, float y);

// ScenesDrawing
void sceneRenderer_MainMenu();
void sceneRenderer_NewGame();
void sceneRenderer_MakeGame();
void sceneRenderer_Game();
void sceneRenderer_Leaders();
void sceneRenderer_Victory();


//=============================LOGICK FUNCTIONS=============================//
void makeNewGame();
void generateField();
void updateCell(int x, int y);
bool getCellUnderMouse(int& x, int& y, int cursorPos_x, int cursor_Pos_y);
void openCell(int cell_x, int cell_y);
void openFreeCells(int startCell_x, int startCell_y);
void setCellFlag(int x, int y);
void gameTimer(int value);
void checkVictory();

void calculateFieldLocation();

// Scenes control logick
void mouseClickFunction_MainMenu(int button, int state, int x, int y);
void mouseMotionFunction_MainMenu(int x, int y);

void mouseClickFunction_NewGame(int button, int state, int x, int y);
void mouseMotionFunction_NewGame(int x, int y);

void mouseClickFunction_MakeGame(int button, int state, int x, int y);
void mouseMotionFunction_MakeGame(int x, int y);

void mouseClickFunction_Game(int button, int state, int x, int y);
void mouseMotionFunction_Game(int x, int y);

void mouseClickFunction_Leaders(int button, int state, int x, int y);
void mouseMotionFunction_Leaders(int x, int y);

void mouseClickFunction_Victory(int button, int state, int x, int y);
void mouseMotionFunction_Victory(int x, int y);
void keyboardFunction_Victory(unsigned char key, int x, int y);


//=========================DATA CONTROLING FUNCTIONS========================//
void saveGame();
void loadGame();
void saveLeadersTable();
void loadLeadersTable();

void loadAllTextures();

GLuint loadTexture(string pathToTexture);

int main(int argc, char** argv)
{
    cout << "Hello World!" << endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);


    if (!glutCreateWindow("Pawel Pasechnik: porn!!!"))
    {
        cout << "Error on create window!" << endl;
        return -1;
    }

    initializationGlut();
    initializationGl();
    initializationGame();

    glutMainLoop();
    return 0;
}

//=============================CONTROL FUNCTIONS============================//
void mainRendererFunction()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    if ( g_gameScenes[g_currentScene].renderFunction )
    {
        g_gameScenes[g_currentScene].renderFunction();
    }

    glutSwapBuffers();
}

void mainKeyboardFunction(unsigned char key, int x, int y)
{
    if ( g_gameScenes[g_currentScene].keyboardFunction )
    {
        g_gameScenes[g_currentScene].keyboardFunction(key, x, y);
    }
}

void mainMouseClickFunction(int button, int state, int x, int y)
{
    if (g_gameScenes[g_currentScene].mouseClickFunction)
    {
        g_gameScenes[g_currentScene].mouseClickFunction(button, state, x, y);
    }
}

void mainMouseMotionFunction(int x, int y)
{
    if (g_gameScenes[g_currentScene].mouseFunction)
    {
        memset(&g_gameMouse, 0, sizeof(g_gameMouse));
        g_gameScenes[g_currentScene].mouseFunction(x, y);
    }
}

void mainResizeFunction(int width, int height)
{
    if (width < 600)
    {
        glutReshapeWindow(600, height);
    }
    else if (height < 600)
    {
        glutReshapeWindow(width, 600);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glViewport(0, 0, width, height);

    g_windowWidth = width;
    g_windowHeight = height;

    g_windowCenter_x = g_windowWidth / 2.0;
    g_windowCenter_y = g_windowHeight / 2.0;

    g_gameFieldPos_x = (g_windowWidth / 2.0) - (g_game.field.width / 2.0 * g_cellSize);
    g_gameFieldPos_y = ((g_windowHeight + g_headerHeight) / 2.0) - (g_game.field.height / 2.0 * g_cellSize);

    glMatrixMode(GL_MODELVIEW);
}


//===========================INITIALIZE FUNCTIONS===========================//
void initializationGame()
{
    loadLeadersTable();

    loadAllTextures();
    // Initialize Main menu functions
    g_gameScenes[GameScene_MainMenu].renderFunction = sceneRenderer_MainMenu;
    g_gameScenes[GameScene_MainMenu].mouseClickFunction = mouseClickFunction_MainMenu;
    g_gameScenes[GameScene_MainMenu].mouseFunction = mouseMotionFunction_MainMenu;
    g_gameScenes[GameScene_MainMenu].keyboardFunction = nullptr;

    // Initialize New game functions
    g_gameScenes[GameScene_NewGame].renderFunction = sceneRenderer_NewGame;
    g_gameScenes[GameScene_NewGame].mouseFunction = mouseMotionFunction_NewGame;
    g_gameScenes[GameScene_NewGame].keyboardFunction = nullptr;
    g_gameScenes[GameScene_NewGame].mouseClickFunction = mouseClickFunction_NewGame;

    // Initialize Make game functions
    g_gameScenes[GameScene_MakeGame].renderFunction = sceneRenderer_MakeGame;
    g_gameScenes[GameScene_MakeGame].mouseFunction = mouseMotionFunction_MakeGame;
    g_gameScenes[GameScene_MakeGame].keyboardFunction = nullptr;
    g_gameScenes[GameScene_MakeGame].mouseClickFunction = mouseClickFunction_MakeGame;

    // Initialize Game functions
    g_gameScenes[GameScene_Game].renderFunction = sceneRenderer_Game;
    g_gameScenes[GameScene_Game].mouseFunction = mouseMotionFunction_Game;
    g_gameScenes[GameScene_Game].keyboardFunction = nullptr;
    g_gameScenes[GameScene_Game].mouseClickFunction = mouseClickFunction_Game;

    // Initialize Leaders functions
    g_gameScenes[GameScene_Leaders].renderFunction = sceneRenderer_Leaders;
    g_gameScenes[GameScene_Leaders].mouseFunction = mouseMotionFunction_Leaders;
    g_gameScenes[GameScene_Leaders].keyboardFunction = nullptr;
    g_gameScenes[GameScene_Leaders].mouseClickFunction = mouseClickFunction_Leaders;

    // Initialize Victory functions
    g_gameScenes[GameScene_Wictory].renderFunction = sceneRenderer_Victory;
    g_gameScenes[GameScene_Wictory].mouseFunction = mouseMotionFunction_Victory;
    g_gameScenes[GameScene_Wictory].keyboardFunction = keyboardFunction_Victory;
    g_gameScenes[GameScene_Wictory].mouseClickFunction = mouseClickFunction_Victory;
    /*
    loadHighscoresTable();
    g_currentGameScene = GameScene_MainMenu;
    memset(&g_game, 0, sizeof(g_game));
    */
}

void initializationGlut()
{
    glutMouseFunc(mainMouseClickFunction);
    glutPassiveMotionFunc(mainMouseMotionFunction);
    glutDisplayFunc(mainRendererFunction);
    glutIdleFunc(mainRendererFunction);
    glutReshapeFunc(mainResizeFunction);
    glutKeyboardFunc(mainKeyboardFunction);
}

void initializationGl()
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
}



//=============================DRAWING FUNCTIONS============================//
// Header drawing
void drawField()
{
    int width = g_game.field.width * g_cellSize;
    int height = g_game.field.height * g_cellSize;

    int start_x = g_gameFieldPos_x;
    int end_x = start_x + width;
    int start_y = g_gameFieldPos_y;
    int end_y = start_y + height;

    glColor3fv(g_fieldBackgroundColor);
    glBegin(GL_QUADS);
    {
        glVertex2i(start_x, start_y);
        glVertex2i(end_x, start_y);
        glVertex2i(end_x, end_y);
        glVertex2i(start_x, end_y);
    }
    glEnd();


    glColor3fv(g_fieldLinesColor);
    glBegin(GL_LINES);
    {
        int x = start_x + g_cellSize;
        int y = start_y + g_cellSize;
        for (int i = g_game.field.width - 1; i >= 0; i--)
        {
            glVertex2i(x, start_y);
            glVertex2i(x, end_y);
            x += g_cellSize;
        }

        for (int i = g_game.field.height - 1; i >= 0; i--)
        {
            glVertex2i(start_x, y);
            glVertex2i(end_x, y);
            y += g_cellSize;
        }
    }
    glEnd();
}

void drawHeader()
{
    glColor3fv(g_headerColor);
    glBegin(GL_QUADS);
    {
        glVertex2f(0, 0);
        glVertex2f(0, g_headerHeight);
        glVertex2f(g_windowWidth, g_headerHeight);
        glVertex2f(g_windowWidth, 0);
    }
    glEnd();
}

void drawNumber(char numSymbol, int x, int y)
{
    glBindTexture(GL_TEXTURE_2D, g_textureNumbers);
    glBegin(GL_QUADS);
    {
        float texOffset = (textureNumber_oneNumberWidth + textureNumber_onePixelWidth) * (numSymbol - '0');
        glTexCoord2f(texOffset, 0); glVertex2f(x, y);
        glTexCoord2f(texOffset, 1); glVertex2f(x, y + 50);

        glTexCoord2f(texOffset + textureNumber_oneNumberWidth, 1);
        glVertex2f(float(x) + textureNumber_oneNumberWidth_int, y + 50);

        glTexCoord2f(texOffset + textureNumber_oneNumberWidth, 0);
        glVertex2f(float(x) + textureNumber_oneNumberWidth_int, y);

    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawEmptyNumber(int x, int y)
{
    glBindTexture(GL_TEXTURE_2D, g_textureNumbers);
    glBegin(GL_QUADS);
    {
        float texOffset = textureNumber_oneNumberWidth;
        glTexCoord2f(texOffset, 0); glVertex2f(x, y);
        glTexCoord2f(texOffset, 1); glVertex2f(x, y + 50);

        glTexCoord2f(texOffset + textureNumber_onePixelWidth, 1);
        glVertex2f(float(x) + textureNumber_onePixelWidth_int, y + 50);

        glTexCoord2f(texOffset + textureNumber_onePixelWidth, 0);
        glVertex2f(float(x) + textureNumber_onePixelWidth_int, y);

    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawTwoHopers(int x, int y)
{
    glBindTexture(GL_TEXTURE_2D, g_textureNumbers);
    glBegin(GL_QUADS);
    {
        float texOffset = (textureNumber_oneNumberWidth + textureNumber_onePixelWidth) * 10;
        glTexCoord2f(texOffset, 0); glVertex2f(x, y);
        glTexCoord2f(texOffset, 1); glVertex2f(x, y + 50);

        glTexCoord2f(texOffset + textureNumber_onePixelWidth, 1);
        glVertex2f(float(x) + textureNumber_onePixelWidth_int, y + 50);

        glTexCoord2f(texOffset + textureNumber_onePixelWidth, 0);
        glVertex2f(float(x) + textureNumber_onePixelWidth_int, y);

    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

int drawNumberString(char* numString, int x, int y)
{
    drawEmptyNumber(x, y);
    x += 7;
    while(*numString)
    {
        drawNumber(*numString, x, y);
        x += 35;
        drawEmptyNumber(x, y);
        x += 7;
        numString++;
    }

    return x;
}

void drawFlagsLimits()
{
    glColor4f(1, 1, 1, 1);
    char buffer[10] = {0};
    sprintf(buffer, "%.3d", g_game.field.flagsAmount);

    drawNumberString(buffer, 0, 5);
}

void drawTime()
{
    glColor4f(1, 1, 1, 1);
    char buffer[10] = {0};
    int x = g_windowWidth - 287;

    sprintf(buffer, "%.2d", g_game.time.hours);
    x = drawNumberString(buffer, x, 5);
    drawTwoHopers(x, 5);
    x += 7;

    sprintf(buffer, "%.2d", g_game.time.minutes);
    x = drawNumberString(buffer, x, 5);
    drawTwoHopers(x, 5);
    x += 7;

    sprintf(buffer, "%.2d", g_game.time.seconds);
    drawNumberString(buffer, x, 5);

    drawEmptyNumber(x, 5);

}

// Controls drawing
void drawButton(int x, int y, int width, int height, ButtonID id)
{
    if (g_gameMouse.mouseOnButton && g_gameMouse.buttonId == id)
    {
        glColor3f(0.2f, 0.8f, 0.1f);
    }
    else
    {
        glColor3f(0.8f, 0.1f, 0.2f);
    }

    glBindTexture(GL_TEXTURE_2D, g_texturesButtons[id]);

    glBegin(GL_QUADS);
    {
        glTexCoord2i(0, 0); glVertex2i(x, y);
        glTexCoord2i(0, 1); glVertex2i(x, y + height);
        glTexCoord2i(1, 1); glVertex2i(x + width, y + height);
        glTexCoord2i(1, 0); glVertex2i(x + width, y);
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

// Field drawing
void drawCell(int x, int y)
{
    float field_x = g_gameFieldPos_x + (x * g_cellSize);
    float field_y = g_gameFieldPos_y + (y * g_cellSize);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0); glVertex2f(field_x, field_y);
        glTexCoord2f(0, 1); glVertex2f(field_x, field_y + g_cellSize - 1);
        glTexCoord2f(1, 1); glVertex2f(field_x + g_cellSize - 1, field_y + g_cellSize - 1);
        glTexCoord2f(1, 0); glVertex2f(field_x + g_cellSize - 1, field_y);
    }
    glEnd();
}

void drawMine(int x, int y)
{
    glColor4f(1,1,1,1);
    glBindTexture(GL_TEXTURE_2D, g_textureMine);
    drawCell(x, y);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawCellNumber(int x, int y)
{
    glColor4f(1,1,1,1);
    glBindTexture(GL_TEXTURE_2D, g_texturesNumbers[g_game.field.field[y][x].type]);
    drawCell(x, y);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawFillCell(int x, int y)
{
    glColor4f(0.6f, 0.4f, 0.1f, 1.0f);
    drawCell(x, y);
}

void drawCellFlag(int x, int y)
{
    glColor4f(1,1,1,1);
    glBindTexture(GL_TEXTURE_2D, g_textureFlag);
    drawCell(x, y);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawCells()
{
    for (int y = 0; y < g_game.field.height; y++)
    {
        for (int x = 0; x < g_game.field.width; x++)
        {
            if (g_game.field.field[y][x].isOpened)
            {
                if (g_game.field.field[y][x].type > 0)
                {
                    drawCellNumber(x, y);
                }
                else if (g_game.field.field[y][x].type < 0)
                {
                    drawMine(x, y);
                }
            }
            else
            {
                drawFillCell(x, y);
                if (g_game.field.field[y][x].isSuspect)
                {
                    drawCellFlag(x, y);
                }
                else if (g_game.gameOver
                         && g_game.field.field[y][x].type == Cell_bomb)
                {
                    drawMine(x, y);
                }
            }
        }
    }
}


void drawWidthController()
{
    glColor4f(1,1,1,1);
    drawButton(g_windowCenter_x + 5, g_windowCenter_y - 105, 35, 50, Button_Width_sub);
    drawButton(g_windowCenter_x + 131, g_windowCenter_y - 105, 35, 50, Button_Width_add);

    glColor4f(1,1,1,1);
    char buffer[10];
    sprintf(buffer, "%.2d", g_gameSetings.width);
    drawNumberString(buffer, g_windowCenter_x + 40, g_windowCenter_y - 105);

    glColor4f(0,1,1,1);
    glBindTexture(GL_TEXTURE_2D, g_textureWidth);
    glBegin(GL_QUADS);
    {
        glTexCoord2i(0, 0); glVertex2i(g_windowCenter_x - 255, g_windowCenter_y - 115);
        glTexCoord2i(0, 1); glVertex2i(g_windowCenter_x - 255, g_windowCenter_y - 45);
        glTexCoord2i(1, 1); glVertex2i(g_windowCenter_x - 15, g_windowCenter_y - 45);
        glTexCoord2i(1, 0); glVertex2i(g_windowCenter_x - 15, g_windowCenter_y - 115);
    }
    glEnd();
}

void drawHeightController()
{
    glColor4f(1,1,1,1);
    drawButton(g_windowCenter_x + 5, g_windowCenter_y - 35, 35, 50, Button_Height_sub);
    drawButton(g_windowCenter_x + 131, g_windowCenter_y - 35, 35, 50, Button_Height_add);

    glColor4f(1,1,1,1);
    char buffer[10];
    sprintf(buffer, "%.2d", g_gameSetings.height);
    drawNumberString(buffer, g_windowCenter_x + 40, g_windowCenter_y - 35);

    glColor4f(1,0,1,1);
    glBindTexture(GL_TEXTURE_2D, g_textureHeight);
    glBegin(GL_QUADS);
    {
        glTexCoord2i(0, 0); glVertex2i(g_windowCenter_x - 255, g_windowCenter_y - 45);
        glTexCoord2i(0, 1); glVertex2i(g_windowCenter_x - 255, g_windowCenter_y + 35);
        glTexCoord2i(1, 1); glVertex2i(g_windowCenter_x - 15, g_windowCenter_y + 35);
        glTexCoord2i(1, 0); glVertex2i(g_windowCenter_x - 15, g_windowCenter_y - 45);
    }
    glEnd();
}
void drawMinesController()
{
    glColor4f(1,1,1,1);
    drawButton(g_windowCenter_x + 5, g_windowCenter_y + 45, 35, 50, Button_Mines_sub);
    drawButton(g_windowCenter_x + 131, g_windowCenter_y + 45, 35, 50, Button_Mines_add);

    glColor4f(1,1,1,1);
    char buffer[10];
    sprintf(buffer, "%.2d", g_gameSetings.minesAmount);
    drawNumberString(buffer, g_windowCenter_x + 40, g_windowCenter_y + 45);

    glColor4f(1,1,0,1);
    glBindTexture(GL_TEXTURE_2D, g_textureMines);
    glBegin(GL_QUADS);
    {
        glTexCoord2i(0, 0); glVertex2i(g_windowCenter_x - 255, g_windowCenter_y + 35);
        glTexCoord2i(0, 1); glVertex2i(g_windowCenter_x - 255, g_windowCenter_y + 105);
        glTexCoord2i(1, 1); glVertex2i(g_windowCenter_x - 15, g_windowCenter_y + 105);
        glTexCoord2i(1, 0); glVertex2i(g_windowCenter_x - 15, g_windowCenter_y + 35);
    }
    glEnd();
}


void drawGameOver()
{
    glColor4f(0.5f, 0, 0, 0.7f);
    glBegin(GL_QUADS);
    {
        glVertex2i(0, g_headerHeight);
        glVertex2i(0, g_windowHeight);
        glVertex2i(g_windowWidth, g_windowHeight);
        glVertex2i(g_windowWidth, g_headerHeight);
    }
    glEnd();

    int windowCenter_x = g_windowWidth / 2;
    int windowCenter_y = (g_windowHeight + g_headerHeight) / 2;
    glColor4f(1, 1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, g_textureGameOver);
    glBegin(GL_QUADS);
    {
        glTexCoord2i(0, 0); glVertex2i(windowCenter_x - 260, windowCenter_y - 40);
        glTexCoord2i(0, 1); glVertex2i(windowCenter_x - 260, windowCenter_y + 40);
        glTexCoord2i(1, 1); glVertex2i(windowCenter_x + 260, windowCenter_y + 40);
        glTexCoord2i(1, 0); glVertex2i(windowCenter_x + 260, windowCenter_y - 40);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawLeadersTable(int x, int y)
{
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    {
        glVertex2i(g_windowCenter_x - 250 + x, g_windowCenter_y - 220 + y);
        glVertex2i(g_windowCenter_x - 250 + x, g_windowCenter_y + 220 + y);
        glVertex2i(g_windowCenter_x + 250 + x, g_windowCenter_y + 220 + y);
        glVertex2i(g_windowCenter_x + 250 + x, g_windowCenter_y - 220 + y);
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    int y_offset = g_windowCenter_y - 190 + y;
    glBegin(GL_LINES);
    {
        glVertex2i(g_windowCenter_x - 20 + x, g_windowCenter_y - 220 + y);
        glVertex2i(g_windowCenter_x - 20 + x, g_windowCenter_y + 220 + y);

        glVertex2i(g_windowCenter_x + 110 + x, g_windowCenter_y - 220 + y);
        glVertex2i(g_windowCenter_x + 110 + x, g_windowCenter_y + 220 + y);

        for (int i = 0; i < 10; i++)
        {
            glVertex2i(g_windowCenter_x - 250 + x, y_offset);
            glVertex2i(g_windowCenter_x + 250 + x, y_offset);

            y_offset += 41;
        }
    }
    glEnd();

    char buffer[30];
    y_offset = g_windowCenter_y - 190 + y;
    for (int i = 0; i < 10; i++)
    {

        sprintf(buffer, "%.2d.", i + 1);
        drawText(buffer, g_windowCenter_x - 245, y_offset + 27);

        sprintf(buffer, "%s", g_leaders[i].name);
        drawText(buffer, g_windowCenter_x - 210, y_offset + 27);

        sprintf(buffer, "%.2d:%.2d:%.2d", g_leaders[i].time.hours,
                g_leaders[i].time.minutes, g_leaders[i].time.seconds);
        drawText(buffer, g_windowCenter_x + 10, y_offset + 27);

        sprintf(buffer, "%.2dx%.2d:%.3d", g_leaders[i].fieldWidth,
                g_leaders[i].fieldHeight, g_leaders[i].fieldMinesAmount);
        drawText(buffer, g_windowCenter_x + 135, y_offset + 27);
        y_offset += 41;
    }



    drawText("NAME", g_windowCenter_x - 230, g_windowCenter_y - 200);
    drawText("TIME", g_windowCenter_x + 20, g_windowCenter_y - 200);
    drawText("WxH:Mines", g_windowCenter_x + 130, g_windowCenter_y - 200);
}

void drawText(const char* text, float x, float y)
{
    glRasterPos2i(x, y);
    int textLen = strlen(text);
    for(int i = 0; i < textLen; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,(int)text[i]);
    }
}

// ScenesDrawing
void sceneRenderer_MainMenu()
{
    glColor3f(0.2f, 0.2f, 0.18f);
    glBegin(GL_QUADS);
    {
        glVertex2i(0, 0);
        glVertex2i(0, g_windowHeight);
        glVertex2i(g_windowWidth, g_windowHeight);
        glVertex2i(g_windowWidth, 0);
    }
    glEnd();
    drawButton(g_windowCenter_x - 255, g_windowCenter_y - 140, 510, 70, Button_NewGame);
    drawButton(g_windowCenter_x - 255, g_windowCenter_y - 70, 510, 70, Button_Load);
    drawButton(g_windowCenter_x - 255, g_windowCenter_y, 510, 70, Button_Liders);
    drawButton(g_windowCenter_x - 255, g_windowCenter_y + 70, 510, 70, Button_Exit);
}

void sceneRenderer_NewGame()
{
    glColor3f(0.2f, 0.2f, 0.18f);
    glBegin(GL_QUADS);
    {
        glVertex2i(0, 0);
        glVertex2i(0, g_windowHeight);
        glVertex2i(g_windowWidth, g_windowHeight);
        glVertex2i(g_windowWidth, 0);
    }
    glEnd();
    drawButton(g_windowCenter_x - 150, g_windowCenter_y - 150, 145, 145, Button_EasyGame);
    drawButton(g_windowCenter_x + 5, g_windowCenter_y - 150, 145, 145, Button_MediumGame);
    drawButton(g_windowCenter_x - 150, g_windowCenter_y + 5, 145, 145, Button_HardGame);
    drawButton(g_windowCenter_x + 5, g_windowCenter_y + 5, 145, 145, Button_UserGame);
    drawButton(g_windowCenter_x - 255, g_windowCenter_y + 200, 510, 70, Button_MainMenu);
}

void sceneRenderer_MakeGame()
{
    glColor3f(0.2f, 0.2f, 0.18f);
    glBegin(GL_QUADS);
    {
        glVertex2i(0, 0);
        glVertex2i(0, g_windowHeight);
        glVertex2i(g_windowWidth, g_windowHeight);
        glVertex2i(g_windowWidth, 0);
    }
    glEnd();

    drawWidthController();
    drawHeightController();
    drawMinesController();

    drawButton(g_windowCenter_x - 255, g_windowCenter_y + 130, 510, 70, Button_NewGame);
    drawButton(g_windowCenter_x - 255, g_windowCenter_y + 200, 510, 70, Button_MainMenu);
}

void sceneRenderer_Game()
{
    drawField();
    drawCells();

    drawHeader();
    drawFlagsLimits();
    drawTime();

    if (g_game.gameOver)
    {
        drawGameOver();
    }

    drawButton(133, 5, 180, 25, Button_NewGame);
    drawButton(133, 30, 180, 25, Button_MainMenu);
}

void sceneRenderer_Leaders()
{
    glColor3f(0.08f, 0.09f, 0.1f);
    glBegin(GL_QUADS);
    {
        glVertex2i(0, 0);
        glVertex2i(0, g_windowHeight);
        glVertex2i(g_windowWidth, g_windowHeight);
        glVertex2i(g_windowWidth, 0);
    }
    glEnd();

    glColor3f(0.8f, 0.8f, 1.0f);
    drawText("Table of leaders", g_windowCenter_x - 70, g_windowCenter_y - 250);
    drawLeadersTable(0, 0);
    drawButton(g_windowCenter_x - 250, g_windowCenter_y + 225, 500, 70, Button_Ok);
}

void sceneRenderer_Victory()
{
    glColor3f(0.08f, 0.09f, 0.1f);
    glBegin(GL_QUADS);
    {
        glVertex2i(0, 0);
        glVertex2i(0, g_windowHeight);
        glVertex2i(g_windowWidth, g_windowHeight);
        glVertex2i(g_windowWidth, 0);
    }
    glEnd();

    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    {
        glVertex2i(g_windowCenter_x - 250, g_windowCenter_y - 270);
        glVertex2i(g_windowCenter_x - 250, g_windowCenter_y - 230);
        glVertex2i(g_windowCenter_x + 250, g_windowCenter_y - 230);
        glVertex2i(g_windowCenter_x + 250, g_windowCenter_y - 270);
    }
    glEnd();

    glColor3f(0.8f, 0.8f, 1.0f);
    drawText("You're MALADEC: ", g_windowCenter_x - 70, g_windowCenter_y - 280);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("Enter your name: ", g_windowCenter_x - 240, g_windowCenter_y - 240);
    drawText(g_leaders[10].name, g_windowCenter_x - 100, g_windowCenter_y - 240);

    drawLeadersTable(0, 0);
    drawButton(g_windowCenter_x - 250, g_windowCenter_y + 225, 500, 70, Button_Yahoo);
}

//=============================LOGICK FUNCTIONS=============================//
void makeNewGame()
{
    Game newGame;
    newGame.gameID = g_game.gameID + 1;

    newGame.gameOver = false;
    memset(&newGame.time, 0, sizeof(Time));
    newGame.field.width = g_gameSetings.width;
    newGame.field.height = g_gameSetings.height;
    newGame.field.flagsAmount = newGame.field.bombsAmount = g_gameSetings.minesAmount;
    newGame.field.field.resize(newGame.field.height);
    newGame.time.hours = 0;
    newGame.time.minutes = 0;
    newGame.time.seconds = 0;
    for (auto& line : newGame.field.field)
    {
        line.resize(newGame.field.width);
        for (auto& num : line)
        {
            num.type = Cell_free;
        }
    }

    g_game = std::move(newGame);

    generateField();
    calculateFieldLocation();

    glutTimerFunc(1000, gameTimer, g_game.gameID);
}

void generateField()
{
    std::vector<GameCell> field(g_game.field.width * g_game.field.height);
    for (auto& cell : field)
    {
        cell.isOpened = false;
        cell.type = Cell_free;
    }
    for (int i = 0; i < g_game.field.bombsAmount; i++)
    {
        field[i].type = Cell_bomb;
    }

    srand(time(nullptr));
    std::random_shuffle(field.begin(), field.end());

    for (int i = 0; i < g_game.field.height; i++)
    {
        auto iterBegin = field.begin() + (i * g_game.field.width);
        auto iterEnd = iterBegin + g_game.field.width;
        g_game.field.field[i].assign(iterBegin, iterEnd);
    }

    for (int y = 0; y < g_game.field.height; y++)
    {
        for (int x = 0; x < g_game.field.width; x++)
        {
            if (g_game.field.field[y][x].type == Cell_bomb)
            {
                for (int i = -1; i < 2; i++)
                {
                    updateCell(x + i, y - 1);
                    if (i != 0)
                    {
                        updateCell(x + i, y);
                    }
                    updateCell(x + i, y + 1);
                }

                g_game.field.mines.push_back({x, y});
            }
        }
    }
}

void updateCell(int x, int y)
{
    if (x < 0 || x >= g_game.field.width
            || y < 0 || y >= g_game.field.height
            || g_game.field.field[y][x].type == Cell_bomb)
    {
        return;
    }
    g_game.field.field[y][x].type = GameCellType(g_game.field.field[y][x].type + 1);
}

bool getCellUnderMouse(int& x, int& y, int cursorPos_x, int cursorPos_y)
{
    if (cursorPos_x < g_gameFieldPos_x || cursorPos_y < g_gameFieldPos_y)
    {
        return false;
    }

    x = (cursorPos_x - g_gameFieldPos_x) / g_cellSize;
    y = (cursorPos_y - g_gameFieldPos_y) / g_cellSize;

    if (x >= g_game.field.width || y >= g_game.field.height)
    {
        return false;
    }

    return true;
}

void openCell(int x, int y)
{
    if (g_game.field.field[y][x].isSuspect)
    {
        return;
    }

    if (g_game.field.field[y][x].type == Cell_bomb)
    {
        g_game.gameOver = true;
        g_game.field.field[y][x].isOpened = true;
        return;
    }
    else if (g_game.field.field[y][x].type == Cell_free)
    {
        openFreeCells(x, y);
    }
    else
    {
        g_game.field.field[y][x].isOpened = true;
    }

    checkVictory();
}

void openFreeCells(int startCell_x, int startCell_y)
{
    queue<pair<int, int>> queueOfClosedCells;
    queueOfClosedCells.push({startCell_x, startCell_y});

    while(!queueOfClosedCells.empty())
    {
        auto cellCoords = queueOfClosedCells.front();
        queueOfClosedCells.pop();

        int x = cellCoords.first;
        int y = cellCoords.second;

        if (x >= 0 && x < g_game.field.width
                && y >= 0 && y < g_game.field.height
                && !g_game.field.field[y][x].isOpened)
        {
            g_game.field.field[y][x].isOpened = true;
        }
        else
        {
             continue;
        }

        if (g_game.field.field[y][x].isSuspect)
        {
            g_game.field.field[y][x].isSuspect = false;
            g_game.field.flagsAmount++;
        }

        if (g_game.field.field[y][x].type != 0)
        {
            continue;
        }

        for (int i = -1; i < 2; i++)
        {
            queueOfClosedCells.push({x + i, y - 1});
            if (i != 0)
            {
                queueOfClosedCells.push({x + i, y});
            }
            queueOfClosedCells.push({x + i, y + 1});
        }
    }
}

void setCellFlag(int x, int y)
{
    if (!g_game.field.field[y][x].isSuspect
            && !g_game.field.field[y][x].isOpened
            && g_game.field.flagsAmount > 0)
    {
        g_game.field.field[y][x].isSuspect = true;
        g_game.field.flagsAmount--;
    }
    else if (g_game.field.field[y][x].isSuspect)
    {
        g_game.field.field[y][x].isSuspect = false;
        g_game.field.flagsAmount++;
    }
}

void gameTimer(int value)
{
    if (g_game.gameID != value || g_game.gameOver)
    {
        return;
    }
    if (g_currentScene != GameScene_Game)
    {
        glutTimerFunc(1000, gameTimer, value);
        return;
    }

    g_game.time.seconds++;
    if (g_game.time.seconds == 60)
    {
        g_game.time.minutes++;
        g_game.time.seconds = 0;
        saveGame();
    }
    if (g_game.time.minutes == 60)
    {
        g_game.time.hours++;
        g_game.time.minutes = 0;
    }

    glutTimerFunc(1000, gameTimer, value);
    glutPostRedisplay();
}

void checkVictory()
{
    for (auto& mine : g_game.field.mines)
    {
        int cell_x = mine.first;
        int cell_y = mine.second;

        for(int y_offset = -1; y_offset < 2; y_offset++)
        {
            int y = cell_y + y_offset;
            if (y < 0 || y >= g_game.field.height)
            {
                continue;
            }

            for(int x_offset = -1; x_offset < 2; x_offset++)
            {
                int x = cell_x + x_offset;
                if (x < 0 || x >= g_game.field.width
                        || (y_offset == 0 && x_offset == 0))
                {
                    continue;
                }

                if (!g_game.field.field[y][x].isOpened && g_game.field.field[y][x].type != Cell_bomb)
                {
                    return;
                }
            }
        }
    }

    memset(&g_leaders[10], 0, sizeof(Leader));
    g_leaders[10].time = std::move(g_game.time);
    g_leaders[10].fieldWidth = g_game.field.width;
    g_leaders[10].fieldHeight = g_game.field.height;
    g_leaders[10].fieldMinesAmount = g_game.field.bombsAmount;
    g_currentScene = GameScene_Wictory;
}


void calculateFieldLocation()
{
    g_gameFieldPos_x = (g_windowWidth / 2.0) - (g_game.field.width / 2.0 * g_cellSize);
    g_gameFieldPos_y = ((g_windowHeight + g_headerHeight) / 2.0) - (g_game.field.height / 2.0 * g_cellSize);
}


// Scenes control logick
void mouseClickFunction_MainMenu(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && g_gameMouse.mouseOnButton)
    {
        if (g_gameMouse.buttonId == Button_NewGame)
        {
            //makeNewGame();
            //glutReshapeWindow(g_game.field.width * g_cellSize, g_game.field.height * g_cellSize + g_headerHeight);
            //g_currentScene = GameScene_Game;
            g_currentScene = GameScene_NewGame;
        }
        else if (g_gameMouse.buttonId == Button_Load)
        {
            loadGame();
            calculateFieldLocation();
            g_currentScene = GameScene_Game;
        }
        else if (g_gameMouse.buttonId == Button_Liders)
        {
            g_currentScene = GameScene_Leaders;
        }
        else if (g_gameMouse.buttonId == Button_Exit)
        {
            exit(0);
        }
    }
}

void mouseMotionFunction_MainMenu(int x, int y)
{
    if ( x >= g_windowCenter_x - 255 && x <= g_windowCenter_x + 255
         && y >= g_windowCenter_y - 140 && y <= g_windowCenter_y + 140 )
    {
        g_gameMouse.mouseOnButton = true;
        if ( y < g_windowCenter_y - 70)
        {
            g_gameMouse.buttonId = Button_NewGame;
        }
        else if (y < g_windowCenter_y)
        {
            g_gameMouse.buttonId = Button_Load;
        }
        else if (y < g_windowCenter_y + 70)
        {
            g_gameMouse.buttonId = Button_Liders;
        }
        else
        {
            g_gameMouse.buttonId = Button_Exit;
        }
    }
}


void mouseClickFunction_NewGame(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && g_gameMouse.mouseOnButton)
    {
        if (g_gameMouse.buttonId == Button_EasyGame)
        {
            g_gameSetings.width = g_gameSetings.height = 10;
            g_gameSetings.minesAmount = 10;
            makeNewGame();

            g_currentScene = GameScene_Game;
        }
        else if (g_gameMouse.buttonId == Button_MediumGame)
        {
            g_gameSetings.width = g_gameSetings.height = 16;
            g_gameSetings.minesAmount = 40;
            makeNewGame();

            g_currentScene = GameScene_Game;
        }
        else if (g_gameMouse.buttonId == Button_HardGame)
        {
            g_gameSetings.width = 30;
            g_gameSetings.height = 20;
            g_gameSetings.minesAmount = 99;
            makeNewGame();

            g_currentScene = GameScene_Game;
        }
        else if (g_gameMouse.buttonId == Button_UserGame)
        {
            g_gameSetings.width = 10;
            g_gameSetings.height = 10;
            g_gameSetings.minesAmount = 10;
            g_currentScene = GameScene_MakeGame;
        }
        else if (g_gameMouse.buttonId == Button_MainMenu)
        {
            g_currentScene = GameScene_MainMenu;
        }
    }
}

void mouseMotionFunction_NewGame(int x, int y)
{
    if (x >= g_windowCenter_x - 255 && x <= g_windowCenter_x + 255
                 && y >= g_windowCenter_y + 200 && y <= g_windowCenter_y + 270)
    {
        g_gameMouse.mouseOnButton = true;
        g_gameMouse.buttonId = Button_MainMenu;
    }
    else if ( x >= g_windowCenter_x - 150 && x <= g_windowCenter_x - 5 )
    {
        g_gameMouse.mouseOnButton = true;
        if ( y >= g_windowCenter_y - 150 && y <= g_windowCenter_y + 5 )
        {
            g_gameMouse.buttonId = Button_EasyGame;
        }
        else if (y >= g_windowCenter_y + 5 && y <= g_windowCenter_y + 150)
        {
            g_gameMouse.buttonId = Button_HardGame;
        }
    }
    else if ( x >= g_windowCenter_x + 5 && x <= g_windowCenter_x + 150 )
    {
        g_gameMouse.mouseOnButton = true;
        if ( y >= g_windowCenter_y - 150 && y <= g_windowCenter_y + 5 )
        {
            g_gameMouse.buttonId = Button_MediumGame;
        }
        else if (y >= g_windowCenter_y + 5 && y <= g_windowCenter_y + 150)
        {
            g_gameMouse.buttonId = Button_UserGame;
        }
    }
}


void mouseClickFunction_MakeGame(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && g_gameMouse.mouseOnButton)
    {
        switch(g_gameMouse.buttonId)
        {
        case Button_NewGame: {
            makeNewGame();
            g_currentScene = GameScene_Game;
        } break;
        case Button_MainMenu: {
            g_currentScene = GameScene_MainMenu;
        } break;
        case Button_Width_add: {
            g_gameSetings.width += g_gameSetings.width < 30 ? 1 : 0;
        } break;
        case Button_Width_sub: {
            g_gameSetings.width -= g_gameSetings.width > 3 ? 1 : 0;
        } break;
        case Button_Height_add: {
            g_gameSetings.height += g_gameSetings.height < 20 ? 1 : 0;
        } break;
        case Button_Height_sub: {
            g_gameSetings.height -= g_gameSetings.height > 3 ? 1 : 0;
        } break;
        case Button_Mines_add: {
            g_gameSetings.minesAmount += g_gameSetings.minesAmount < (g_gameSetings.width * g_gameSetings.height / 2) ? 1 : 0;
        } break;
        case Button_Mines_sub: {
            g_gameSetings.minesAmount -= g_gameSetings.minesAmount > 1 ? 1 : 0;
        } break;
        }
    }
}

void mouseMotionFunction_MakeGame(int x, int y)
{
    if (x >= g_windowCenter_x - 255 && x <= g_windowCenter_x + 255
                 && y >= g_windowCenter_y + 130 && y <= g_windowCenter_y + 270)
    {
        g_gameMouse.mouseOnButton = true;
        if (y < g_windowCenter_y + 200)
        {
            g_gameMouse.buttonId = Button_NewGame;
        }
        else
        {
            g_gameMouse.buttonId = Button_MainMenu;
        }
    }
    if (x >= g_windowCenter_x + 5 && x <= g_windowCenter_x + 40)
    {
        if (y >=  g_windowCenter_y - 105 && y <=  g_windowCenter_y - 55)
        {
            g_gameMouse.mouseOnButton = true;
            g_gameMouse.buttonId = Button_Width_sub;
        }
        else if (y >=  g_windowCenter_y - 35 && y <=  g_windowCenter_y + 15)
        {
            g_gameMouse.mouseOnButton = true;
            g_gameMouse.buttonId = Button_Height_sub;
        }
        else if (y >=  g_windowCenter_y + 45 && y <=  g_windowCenter_y + 95)
        {
            g_gameMouse.mouseOnButton = true;
            g_gameMouse.buttonId = Button_Mines_sub;
        }
    }
    else if (x >= g_windowCenter_x + 131 && x <= g_windowCenter_x + 166)
    {
        if (y >=  g_windowCenter_y - 105 && y <=  g_windowCenter_y - 55)
        {
            g_gameMouse.mouseOnButton = true;
            g_gameMouse.buttonId = Button_Width_add;
        }
        else if (y >=  g_windowCenter_y - 35 && y <=  g_windowCenter_y + 15)
        {
            g_gameMouse.mouseOnButton = true;
            g_gameMouse.buttonId = Button_Height_add;
        }
        else if (y >=  g_windowCenter_y + 45 && y <=  g_windowCenter_y + 95)
        {
            g_gameMouse.mouseOnButton = true;
            g_gameMouse.buttonId = Button_Mines_add;
        }
    }
}


void mouseClickFunction_Game(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON
            && state == GLUT_DOWN)
    {
        if (g_gameMouse.mouseOnButton && g_gameMouse.buttonId == Button_NewGame)
        {
            makeNewGame();
        }
        else if (g_gameMouse.mouseOnButton && g_gameMouse.buttonId == Button_MainMenu)
        {
            saveGame();
            g_currentScene = GameScene_MainMenu;
        }
        else if (g_gameMouse.mouseOnCell)
        {
            openCell(g_gameMouse.cell_x, g_gameMouse.cell_y);
        }
    }
    else if (button == GLUT_RIGHT_BUTTON
             && state == GLUT_DOWN
             && g_gameMouse.mouseOnCell)
    {
        setCellFlag(g_gameMouse.cell_x, g_gameMouse.cell_y);
    }
}

void mouseMotionFunction_Game(int x, int y)
{
    int cell_x = 0;
    int cell_y = 0;
    if ( y >= 5 && y <= 55 && x >= 133 && x <= 313 )
    {
        g_gameMouse.mouseOnButton = true;
        if (y <= 30)
        {
            g_gameMouse.buttonId = Button_NewGame;
        }
        else
        {
            g_gameMouse.buttonId = Button_MainMenu;
        }
    }
    else if (!g_game.gameOver && getCellUnderMouse(cell_x, cell_y, x, y))
    {
        g_gameMouse.mouseOnCell = true;
        g_gameMouse.cell_x = cell_x;
        g_gameMouse.cell_y = cell_y;
    }
}


void mouseClickFunction_Leaders(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && g_gameMouse.mouseOnButton)
    {
        g_currentScene = GameScene_MainMenu;
    }
}

void mouseMotionFunction_Leaders(int x, int y)
{
    if ( x >= g_windowCenter_x - 250 && x <= g_windowCenter_x + 250
         && y >= g_windowCenter_y + 225 && y <= g_windowCenter_y + 295 )
    {
        g_gameMouse.mouseOnButton = true;
        g_gameMouse.buttonId = Button_Ok;
    }
}


void mouseClickFunction_Victory(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN
            && g_gameMouse.mouseOnButton && g_leaders[10].name[0])
    {
        sort(g_leaders.begin(), g_leaders.end(), [](const Leader& left, const Leader& right){
            if (!left.name[0]) return false;
            if (!right.name[0]) return true;

            bool minesEq = left.fieldMinesAmount == right.fieldMinesAmount;
            bool hoursEq = left.time.hours == right.time.hours;
            bool minsEq = left.time.minutes == right.time.minutes;

            if (left.fieldMinesAmount > right.fieldMinesAmount)
            {
                return true;
            }
            else if (left.fieldWidth * left.fieldHeight < right.fieldWidth * right.fieldHeight)
            {
                return true;
            }
            else if (minesEq && left.time.hours < right.time.hours)
            {
                return true;
            }
            else if (minesEq && hoursEq && left.time.minutes < right.time.minutes)
            {
                return true;
            }
            else if (minesEq && hoursEq && minsEq && left.time.seconds < right.time.seconds)
            {
                return true;
            }
            return false;
        });

        saveLeadersTable();
        g_currentScene = GameScene_Leaders;
    }
}

void mouseMotionFunction_Victory(int x, int y)
{
    if ( x >= g_windowCenter_x - 250 && x <= g_windowCenter_x + 250
         && y >= g_windowCenter_y + 225 && y <= g_windowCenter_y + 295 )
    {
        g_gameMouse.mouseOnButton = true;
        g_gameMouse.buttonId = Button_Yahoo;
    }
}

void keyboardFunction_Victory(unsigned char key, int x, int y)
{
    int textlen = strlen(g_leaders[10].name);
    if (key == 8)
    {
        if (textlen != 0)
        {
            textlen--;
        }
        g_leaders[10].name[textlen] = '\0';
        return;
    }
    else if (key >= 25 && textlen < 10)
    {
        g_leaders[10].name[textlen] = key;
    }
}


//=========================DATA CONTROLING FUNCTIONS========================//
void saveGame()
{
    ofstream fout("save.bin", ios::binary);
    if (!fout)
    {
        return;
    }

    fout.write((char*)&g_game.field.width, sizeof(int));
    fout.write((char*)&g_game.field.height, sizeof(int));
    fout.write((char*)&g_game.field.bombsAmount, sizeof(int));
    fout.write((char*)&g_game.field.flagsAmount, sizeof(int));
    fout.write((char*)&g_game.time, sizeof(Time));

    for (auto& fieldLine : g_game.field.field)
    {
        fout.write((char*)fieldLine.data(), sizeof(GameCell) * g_game.field.width);
    }

    fout.write((char*)g_game.field.mines.data(), sizeof(pair<int,int>) * g_game.field.bombsAmount);

    fout.close();
}

void loadGame()
{
    ifstream fin("save.bin", ios::binary);
    if (!fin)
    {
        g_gameSetings.width = g_gameSetings.height = 10;
        g_gameSetings.minesAmount = 10;
        makeNewGame();
        return;
    }

    fin.read((char*)&g_game.field.width, sizeof(int));
    fin.read((char*)&g_game.field.height, sizeof(int));
    fin.read((char*)&g_game.field.bombsAmount, sizeof(int));
    fin.read((char*)&g_game.field.flagsAmount, sizeof(int));
    fin.read((char*)&g_game.time, sizeof(Time));

    g_game.field.field.resize(g_game.field.height);
    for (auto& fieldLine : g_game.field.field)
    {
        fieldLine.resize(g_game.field.width);
        fin.read((char*)fieldLine.data(), sizeof(GameCell) * g_game.field.width);
    }

    g_game.field.mines.resize(g_game.field.bombsAmount);
    fin.read((char*)g_game.field.mines.data(), sizeof(pair<int,int>) * g_game.field.bombsAmount);

    g_gameSetings.width = g_game.field.width;
    g_gameSetings.height = g_game.field.height;
    g_gameSetings.minesAmount = g_game.field.bombsAmount;

    g_game.gameID++;

    fin.close();
    glutTimerFunc(1000, gameTimer, g_game.gameID);
}

void saveLeadersTable()
{
    ofstream fout("leaders.bin", ios::binary);
    if (!fout)
    {
        return;
    }
    fout.write((char*)g_leaders.data(), sizeof(Leader) * 10);
    fout.close();
}

void loadLeadersTable()
{
    ifstream fin("leaders.bin", ios::binary);
    if (!fin)
    {
        memset(g_leaders.data(), 0, sizeof(Leader) * 10);
        return;
    }
    fin.read((char*)g_leaders.data(), sizeof(Leader) * 10);
    fin.close();
}

GLuint loadTexture(string pathToTexture)
{
    int imageWidth = 0;
    int imageHeight = 0;
    int imageNrChannels = 0;
    unsigned char* data = stbi_load(pathToTexture.c_str(), &imageWidth, &imageHeight, &imageNrChannels, 0);
    if (data)
    {
        int hasAlpha = imageNrChannels - 3;
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB + hasAlpha, imageWidth, imageHeight, 0, GL_RGB + hasAlpha, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

        return texture;
    }
    return 0;
}

void loadAllTextures()
{
    g_textureNumbers = loadTexture("./img/Numbers.png");
    g_textureMine = loadTexture("./img/Mine.png");
    g_textureFlag = loadTexture("./img/Flag.png");
    g_texturesButtons[Button_NewGame] = loadTexture("./img/Buttons/NewGameText.png");
    g_texturesButtons[Button_Exit] = loadTexture("./img/Buttons/ExitGameText.png");
    g_texturesButtons[Button_Liders] = loadTexture("./img/Buttons/LeadersText.png");
    g_texturesButtons[Button_Load] = loadTexture("./img/Buttons/LoadGameText.png");
    g_texturesButtons[Button_Ok] = loadTexture("./img/Buttons/OkText.png");
    g_texturesButtons[Button_Yahoo] = loadTexture("./img/Buttons/YahooText.png");
    g_texturesButtons[Button_MainMenu] = loadTexture("./img/Buttons/MainMenuText.png");
    g_texturesButtons[Button_EasyGame] = loadTexture("./img/GameModes/Easy.png");
    g_texturesButtons[Button_MediumGame] = loadTexture("./img/GameModes/Medium.png");
    g_texturesButtons[Button_HardGame] = loadTexture("./img/GameModes/Hard.png");
    g_texturesButtons[Button_UserGame] = loadTexture("./img/GameModes/UserPro.png");

    g_texturesButtons[Button_Width_add] = loadTexture("./img/Buttons/RightArrow.png");
    g_texturesButtons[Button_Height_add] = g_texturesButtons[Button_Width_add];
    g_texturesButtons[Button_Mines_add] = g_texturesButtons[Button_Width_add];

    g_texturesButtons[Button_Width_sub] = loadTexture("./img/Buttons/LeftArrow.png");
    g_texturesButtons[Button_Height_sub] = g_texturesButtons[Button_Width_sub];
    g_texturesButtons[Button_Mines_sub] = g_texturesButtons[Button_Width_sub];

    g_textureWidth = loadTexture("./img/GameSettings/Width.png");
    g_textureHeight = loadTexture("./img/GameSettings/Height.png");
    g_textureMines = loadTexture("./img/GameSettings/Mines.png");

    g_textureGameOver = loadTexture("./img/GameOverText.png");

    g_texturesNumbers[0] = 0;
    char path[] = "./img/Numbers/1.png";
    for (int i = 1; i <= 8; i++)
    {
        path[14] = '0' + i;
        g_texturesNumbers[i] = loadTexture(path);
    }
}
