#include <Windows.h>

#ifndef WMCREATE_H_
#define WMCREATE_H_

typedef struct {
    TCHAR id[128];
    TCHAR title[128];
    TCHAR rating[4];
    TCHAR releasedate[4];
    TCHAR type[5];
} result;

typedef struct {
    TCHAR id[256];
    TCHAR number[3];
    TCHAR title[256];
} episode;

//test struct
typedef struct trendinganimeinfo {
    char id[128];
    char title[128];
    char imageurl[256];
} trendinganimeinfo;

boolean testsMinutils();
void createUtils();
BOOL CALLBACK enumVisibleChildWindowsProc(HWND hwnd, LPARAM lparam);
void destroyVisibleChildWindows(HWND hwndparent);
int homeWindow(HWND hwnd);
int searchWindow(HWND hwnd);
int settingsWindow(HWND hwnd);
BOOL hideSearch();
BOOL showSearch();
int searchResults(HWND hwnd, char * query);
BOOL showSettings();
BOOL hideSettings();

#endif