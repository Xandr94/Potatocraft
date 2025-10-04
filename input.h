#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <windows.h>

struct {
    BOOL keyW;
    BOOL keyA;
    BOOL keyS;
    BOOL keyD;
    BOOL keySpace;
    BOOL keyShift;
    BOOL keyCtrl;

    BOOL isPaused;
} input;

void WndResize(int x, int y);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // INPUT_H_INCLUDED
