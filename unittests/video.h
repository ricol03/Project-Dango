#include <Windows.h>

#ifndef VIDEO_H_
#define VIDEO_H_

LRESULT videoWindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
HWND videoWindowMain(HWND hwnd, HINSTANCE hinstance);
int initializeLink(HWND hwnd, char * link);
int videoWindow(HWND hwnd, char * link);

#endif