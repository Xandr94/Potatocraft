#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include <windows.h>

void Render_Init();
void Game_Show();
void FreeRenderer();
void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC);
void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC);

#endif // RENDER_H_INCLUDED
