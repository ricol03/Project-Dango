#include <Windows.h>

#ifndef FILE_H_
#define FILE_H_

void createDirectory();
int checkDirectory(TCHAR * pathname);
wchar_t * checkImage(wchar_t * showid);
wchar_t * createImagePath(wchar_t * showid, wchar_t * fileextension);
void readyingFile();
int readSettings();
int saveSettings();

#endif