#pragma once

#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include <datastructures.h>

//////////////////////// Game scenes (need initialize)
GameScene g_gameScenes[GameScene_AmountScenes];
GameSceneType g_currentScene = GameScene_MainMenu;

//////////////////////// Game data
Game g_game;
GameFieldSetings g_gameSetings;

/////////////////////// Leaders
std::vector<Leader> g_leaders(11);

/////////////////////// Mouse
GameMouse g_gameMouse;

/////////////////////// Pos
int g_windowWidth = 0;
int g_windowHeight = 0;
int g_windowCenter_x = 0;
int g_windowCenter_y = 0;
int g_gameFieldPos_x = 0;
int g_gameFieldPos_y = 0;

/////////////////////// Textures
GLuint g_textureNumbers = 0;
GLuint g_textureMine = 0;
GLuint g_textureFlag = 0;
GLuint g_texturesNumbers[9] = { 0 };
GLuint g_texturesButtons[ButtonsAmount] = { 0 };
GLuint g_textureGameOver = 0;
GLuint g_textureWidth = 0;
GLuint g_textureHeight = 0;
GLuint g_textureMines = 0;



/////////////////////// User interface
const int g_headerHeight = 60;
//const float g_headerColor[] = { 0.1f, 0.1f, 0.2f };
const float g_headerColor[] = { 0, 0, 0 };

const float g_clearColor[] = { 0.75f, 0.75f, 0.75f };


/////////////////////// Game parametrs
const float g_cellSize = 30.0f;

const float g_fieldLinesColor[] = { g_clearColor[0], g_clearColor[1], g_clearColor[2] };
const float g_fieldBackgroundColor[] = { 0.6f, 0.6f, 0.6f };

const int textureNumber_onePixelWidth_int = 7;
const int textureNumber_oneNumberWidth_int = textureNumber_onePixelWidth_int * 5;
const float textureNumber_onePixelWidth = 0.014;
const float textureNumber_oneNumberWidth = textureNumber_onePixelWidth * 5;

#endif // GLOBALVARIABLES_H
