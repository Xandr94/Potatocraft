#include <windows.h>
#include <gl/gl.h>

#include "input.h"
#include "game.h"

void WndResize(int x, int y)
{
    glViewport(0,0, x,y);

    float k = x / (float)y;
    float sz = 0.1;
    glLoadIdentity();
    glFrustum(-k*sz,k*sz, -sz,sz, sz*2,100);
}

void HandleKeyInput(WPARAM key, BOOL isDown) {
    switch (key) {
        case VK_ESCAPE:
            if (isDown && !input.isPaused)
            {
                input.isPaused = TRUE;
                ShowCursor(TRUE);
            }
        break;
        case 'W': input.keyW = isDown; break;
        case 'A': input.keyA = isDown; break;
        case 'S': input.keyS = isDown; break;
        case 'D': input.keyD = isDown; break;
        case VK_SPACE: input.keySpace = isDown; break;
        case VK_SHIFT: input.keyShift = isDown; break;
        case VK_CONTROL: input.keyCtrl = isDown; break;
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_SIZE:
            WndResize(LOWORD(lParam),HIWORD(lParam));
        break;

        case WM_LBUTTONDOWN:
            switch (wParam)
            {
                case VK_LBUTTON:
                    if (input.isPaused == TRUE)
                    {
                        input.isPaused = FALSE;
                        ShowCursor(FALSE);
                    }
                break;
            }

        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            if ((lParam & (1 << 30)) == 0) //30-й бит lParam показывает, что это первое нажатие клавиши
                HandleKeyInput(wParam, TRUE);
        }
        break;
        case WM_KEYUP:
        {
            HandleKeyInput(wParam, FALSE);
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
