#include <Windows.h>

#ifndef MAIN_H_
#define MAIN_H_

LRESULT CALLBACK WindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT SearchWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT SettingsWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
void videoWindowWrapper(void* data);
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, PSTR lpcmdline, int nshowcmd);

#endif