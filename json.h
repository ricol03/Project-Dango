#include <Windows.h>
#include <winhttp.h>
#include "connections.h"
#include "wmcreate.h"

#ifndef JSON_H_
#define JSON_H_



char * getJsonId(char* jsonstring, const char* key, TCHAR* value, int check);
int getEpisodesNum(char* jsonstring, const char* key);
int parseResultsJson(char* jsonstring, result results[]);
int parseEpisodesJson(HWND hwnd, char * resultid, char* jsonstring, episode episodes[]);
char * getLinkJson(HWND hwnd, char * jsonstring);
int getQualitiesJson(char* jsonstring, stream streams[]);
int getShowInfo(char * jsonstring, animeinfo show);
int getTrendingShows(char * jsonstring, trendinganimeinfo results[]);

#endif