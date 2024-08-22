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

wchar_t * winHttpGetResponse(HINTERNET hRequest, HINTERNET hConnect, HINTERNET hSession);
wchar_t * winHttpGetResponseBin(HINTERNET hRequest, HINTERNET hConnect, HINTERNET hSession, wchar_t * filepath);

void parseRequestText(wchar_t * uri, wchar_t * query);
LPCSTR TparseRequestText(wchar_t * uri, wchar_t * query);
void parseRequestText2(wchar_t * uri, wchar_t * query);
LPWSTR serverAddressInitializer(wchar_t * server);

int searchConnection(HWND hwnd, wchar_t * query, result results[]);
wchar_t * TsearchConnection(HWND hwnd, wchar_t * query, result results[]);
int episodesConnection(HWND hwnd, wchar_t * resultid, episode episodes[]);
int epnumConnection(HWND hwnd, wchar_t * resultid);
wchar_t * eplinkConnection(HWND hwnd, wchar_t * epid);
animeinfo getInfoConnection(HWND hwnd, wchar_t * epid, animeinfo info);
int getTrendsConnection(HWND hwnd, trendinganimeinfo shows[]);
wchar_t * imageConnection(HWND hwnd, wchar_t * imageurl, wchar_t * showid);

#endif