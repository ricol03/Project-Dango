#include <Windows.h>
#include <winhttp.h>
#include "wmcreate.h"

#ifndef CONNECTIONS_H_
#define CONNECTIONS_H_

typedef union {
    char* value;
    int number;
} connections;

char* winHttpGetResponse(HINTERNET hRequest, HINTERNET hConnect, HINTERNET hSession);

void parseRequestText(char * uri, char * query);
LPSTR TparseRequestText(char * uri, char * query);
void parseRequestText2(char * uri, char * query);

int searchConnection(HWND hwnd, char * query, result results[]);
int episodesConnection(HWND hwnd, char * resultid, episode episodes[]);
int epnumConnection(HWND hwnd, char * resultid);
char * eplinkConnection(HWND hwnd, char * epid);
char * TeplinkConnection(HWND hwnd, char * epid);

#endif