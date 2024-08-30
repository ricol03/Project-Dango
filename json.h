#include <Windows.h>
#include <winhttp.h>
#include "connections.h"
#include "wmcreate.h"

#ifndef JSON_H_
#define JSON_H_



wchar_t * getJsonId(wchar_t * jsonstring, const wchar_t * key, TCHAR* value, int check);
wchar_t * getGenres(wchar_t * jsonstring, const wchar_t * key, TCHAR value[][32]);
wchar_t * getLinkDomain(wchar_t * url);
wchar_t * getLinkSubdomain(wchar_t * url, wchar_t * server);
wchar_t * getFileExtension(wchar_t * url);
int getEpisodesNum(wchar_t * jsonstring, const wchar_t * key);
int parseResultsJson(wchar_t * jsonstring, result results[]);
int parseEpisodesJson(HWND hwnd, wchar_t * resultid, wchar_t * jsonstring, episode episodes[]);
wchar_t * getLinkJson(HWND hwnd, wchar_t * jsonstring);
int getQualitiesJson(wchar_t * jsonstring, stream streams[]);
animeinfo getShowInfo(wchar_t * jsonstring, animeinfo show);
int getTrendingShows(wchar_t * jsonstring, trendinganimeinfo results[]);
int convertWideToMulti(wchar_t * widestring, char * multistring);


#endif