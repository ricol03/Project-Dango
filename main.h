#include <Windows.h>

#ifndef MAIN_H_
#define MAIN_H_

LRESULT CALLBACK MainWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK VideoWindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK SearchWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK SettingsWndProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK NetworkTabProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK ProviderTabProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK LangTabProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK InfoWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK EpisodeWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

void videoWindowWrapper(void* data);
DWORD WINAPI settingsWindowWrapper(void* arg);
int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, PWSTR lpcmdline, int nshowcmd);

int checkVersion();
int createSettingsWindow(HINSTANCE hinstance);
int createNetworkTab();
int createProviderTab();
int createLanguageTab();
int createVideoWindow(HINSTANCE hinstance);
int createInfoWindow(HINSTANCE hinstance);
int createEpisodeWindow(HINSTANCE hinstance);

BOOL CALLBACK SetFontProc(HWND hwnd, LPARAM lparam);
BOOL CALLBACK SetBackgroundColorProc(HWND hwnd, LPARAM lparam);
void initializeTheming();
void showNetworkTab();
void showProviderTab();
void showLanguageTab();

#endif