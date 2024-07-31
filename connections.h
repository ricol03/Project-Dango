#include <Windows.h>
#include <winhttp.h>
#include "wmcreate.h"

#ifndef CONNECTIONS_H_
#define CONNECTIONS_H_

//number of different connections made
#define MAXCONNECTIONS 5

typedef union {
    char* value;
    int number;
} connections;

char* winHttpGetResponse(HINTERNET hRequest, HINTERNET hConnect, HINTERNET hSession);

void parseRequestText(char * uri, char * query);
LPCSTR TparseRequestText(char * uri, char * query);
void parseRequestText2(char * uri, char * query);
LPSTR serverAddressInitializer();

int searchConnection(HWND hwnd, char * query, result results[]);
char * TsearchConnection(HWND hwnd, char * query, result results[]);
int episodesConnection(HWND hwnd, char * id, episode episodes[]);
int epnumConnection(HWND hwnd, char * resultid);
char * eplinkConnection(HWND hwnd, char * epid);
int getinfoConnection(HWND hwnd, trendinganimeinfo shows[]);

#endif