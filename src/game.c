#include <windows.h>
#include <gl/gl.h>
#include <math.h>

#include "game.h"
#include "main.h"
#include "render.h"
#include "input.h"
#include "camera.h"

struct SPlayer
{
    float x,y,z,speed;
    BOOL sprint;
    BOOL flyMode;
} player;

void Player_Init(float x,float y,float z,float speed,BOOL sprint,BOOL flyMode)
{
    player.x = x;
    player.y = y;
    player.z = z;
    player.speed = speed;
    player.flyMode = flyMode;
    player.sprint = sprint;
}

void Player_Move(HWND *hwnd)
{
    Camra_MoveDirection(input.keyW ? 1 : (input.keyS ? -1 : 0),
                        input.keyD ? 1 : (input.keyA ? -1 : 0),
                        input.keySpace ? 1 : (input.keyShift ? -1 : 0),
                        input.keyCtrl ? player.speed*2 : player.speed);
    player.x = camera.x;
    player.y = camera.y;
    player.z = camera.z;

    RECT rect;
    GetClientRect(*hwnd, &rect);
    POINT center = { rect.right / 2, rect.bottom / 2 };
    ClientToScreen(*hwnd, &center);
    //SetCursorPos(center.x, center.y); // Центрируем курсор

    Camera_AutoMoveByMouse(center.x, center.y, 0.25);
}



void Map_Init()
{
    for (int i = 0; i < pw; i++)
        for (int j = 0; j < ph; j++)
        {
            //Случайное смещение цвета от 0.01 до 0.20
            float dc = (rand() % 20) * 0.01;
            //Случайный цвет
            map[i][j].clr.r = 0.31 + dc;
            map[i][j].clr.g = 0.5 + dc;
            map[i][j].clr.b = 0.13 + dc;
        }
}



void Enemy_Init()
{
    for (int i = 0; i < enemyCnt; i++)
    {
        enemy[i].active = TRUE;
        enemy[i].x = rand() % pw;
        enemy[i].y = rand() % ph;
        enemy[i].z = rand() % 5;
    }
}



void Game_Move(HWND *hwnd)
{
    Player_Move(hwnd);
}


void Game_Init(HWND *hwnd, HDC *hDC, HGLRC *hRC)
{
    /* enable OpenGL for the window */
    EnableOpenGL(*hwnd, hDC, hRC);

    Map_Init();
    Enemy_Init();
    Player_Init(0,0,0,0.1,0,0);

    RECT rect;
    GetClientRect(*hwnd, &rect);
    WndResize(rect.right,rect.bottom);
    //WndResize(300,200); // Пример ручного изменения размера окна

    POINT center = { rect.right / 2, rect.bottom / 2 };
    ClientToScreen(*hwnd, &center);
    SetCursorPos(center.x, center.y);

    ShowCursor(FALSE);

    Render_Init();
}

void Game_Free(HWND *hwnd, HDC *hDC, HGLRC *hRC)
{
    FreeRenderer();
    /* shutdown OpenGL */
    DisableOpenGL(*hwnd, *hDC, *hRC);
}
