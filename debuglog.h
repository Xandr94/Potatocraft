#ifndef DEBUGLOG_H_INCLUDED
#define DEBUGLOG_H_INCLUDED

#include <windows.h>

int CreateDebugThread(HWND *phwnd, HANDLE *phThread);
void CloseDebugThread(HANDLE *phThread);

#endif // DEBUGLOG_H_INCLUDED
