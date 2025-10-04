#include <GL/gl.h>
#include <windows.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14
#endif
#ifndef M_PI_2
#define M_PI_2 1.57
#endif
#ifndef M_PI_4
#define M_PI_4 0.78
#endif

#include "camera.h"



struct SCamera camera = {0,0,1.7,90,0};



void Camera_Rotation(float xAngle, float zAngle)
{
    camera.Zrot += zAngle;
    if (camera.Zrot < 0) camera.Zrot += 360;
    if (camera.Zrot > 360) camera.Zrot -= 360;
    camera.Xrot += xAngle;
    if (camera.Xrot < 0) camera.Xrot = 0;
    if (camera.Xrot > 180) camera.Xrot = 180;
}

void Camera_AutoMoveByMouse(int centerX, int centerY, float speed)
{
    POINT cur;
    POINT base = {centerX, centerY};
    GetCursorPos(&cur);
    Camera_Rotation( (base.y - cur.y) * speed, (base.x - cur.x) * speed);
    SetCursorPos(base.x, base.y);
}

void Camra_MoveDirection(int forwardMove, int rightMove, int verticalMove, float speed)
{
    camera.z += verticalMove * speed;

    //Поворот камеры в плоскости
    float ugol = -camera.Zrot / 180 * M_PI;

    if (forwardMove > 0)
        ugol += rightMove > 0 ? M_PI_4 : (rightMove < 0 ? -M_PI_4 : 0);
    if (forwardMove < 0)
        ugol += M_PI + (rightMove > 0 ? -M_PI_4 : (rightMove < 0 ? M_PI_4 : 0));
    if (forwardMove == 0)
    {
        ugol += rightMove > 0 ? M_PI_2 : -M_PI_2;
        if (rightMove == 0) speed = 0;
    }

    if (speed != 0)
    {
        camera.x += sin(ugol) * speed;
        camera.y += cos(ugol) * speed;
    }
}
