#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <windows.h>

#define enemyCnt 40
struct {
    float x,y,z;
    BOOL active;
} enemy[enemyCnt];

typedef struct{
    float r,g,b;
} TColor;

typedef struct{
    TColor clr;
} TCell;

#define pw 40
#define ph 40
//RAND_MAX = 0x7FFF (32767)
TCell map[pw][ph];

void Game_Init(HWND *hwnd, HDC *hDC, HGLRC *hRC);
void Game_Move(HWND *hwnd);
void Game_Show();
void Game_Free(HWND *hwnd, HDC *hDC, HGLRC *hRC);

#endif // GAME_H_INCLUDED
