
#include "debuglog.h"
#include <stdio.h>

#include "camera.h"


DWORD WINAPI DebugThread(LPVOID lpParameter)
{
    HWND *phwnd = (HWND*)lpParameter;
    while(TRUE) 
    {
        RECT client_rect;
        RECT window_rect;
        GetClientRect(*phwnd, &client_rect);
        GetWindowRect(*phwnd, &window_rect);
        system("cls");
        printf("client_rect.left=%ld\n",client_rect.left);
        printf("client_rect.right=%ld\n",client_rect.right);
        printf("client_rect.top=%ld\n",client_rect.top);
        printf("client_rect.bottom=%ld\n",client_rect.bottom);
        printf("\n");
        printf("window_rect.left=%ld\n",window_rect.left);
        printf("window_rect.right=%ld\n",window_rect.right);
        printf("window_rect.top=%ld\n",window_rect.top);
        printf("window_rect.bottom=%ld\n",window_rect.bottom);
        printf("\n");
        extern struct SCamera camera;
        printf("camera_pos=(%f, %f, %f)\n",camera.x, camera.y, camera.z);
        printf("camera_rot=(%f, %f, %f)\n",camera.Xrot, 0.0f, camera.Zrot);
        Sleep(200);
    }
}

int CreateDebugThread(HWND *phwnd, HANDLE *phThread)
{
    *phThread = CreateThread(
        NULL,
        0,
        DebugThread,
        phwnd,
        0,
        NULL);
    if (*phThread == NULL)
    {
        printf("Error creating thread DebugThread.\n");
        return 1;
    }
    return 0;
}

void CloseDebugThread(HANDLE *phThread) {
    if (*phThread) CloseHandle(*phThread);
}
