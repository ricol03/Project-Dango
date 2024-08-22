#include <Windows.h>

#ifndef WMCREATE_H_
#define WMCREATE_H_

typedef struct {
    TCHAR id[128];
    TCHAR title[128];
    TCHAR rating[4];
    TCHAR releasedate[10];
    TCHAR type[16];
} result;

typedef struct {
    TCHAR id[256];
    TCHAR number[3];
    TCHAR title[256];
} episode;

typedef struct {
    TCHAR link[512];
    TCHAR quality[8];
} stream;

typedef struct {
    TCHAR id[128];
    TCHAR title[128];
    TCHAR description[2048];
    TCHAR imageurl[512];
    TCHAR date[10];
    TCHAR episodes[4];
    TCHAR type[16];
    TCHAR status[16];
    //false: sub | true: dub
    BOOL isdub;
    BOOL isempty;
} animeinfo;

//test struct
typedef struct trendinganimeinfo {
    char id[128];
    char title[128];
    char imageurl[256];
} trendinganimeinfo;

void createUtils();
BOOL CALLBACK enumVisibleChildWindowsProc(HWND hwnd, LPARAM lparam);
void destroyVisibleChildWindows(HWND hwndparent);
int homeWindow(HWND hwnd);
int searchWindow(HWND hwnd);
int settingsWindow(HWND hwnd);
BOOL hideSearch();
BOOL showSearch();
int searchResults(HWND hwnd, wchar_t * query);
int infoWindow(HWND hwnd);
BOOL showSettings();
BOOL hideSettings();

int settingsWindow(HWND hwnd);
int networkTab(HWND hwnd);
int providerTab(HWND hwnd);
int updateNotice();
int languageTab(HWND hwnd);

#endif