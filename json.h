#include <Windows.h>
#include <winhttp.h>
#include "connections.h"
#include "wmcreate.h"

#ifndef JSON_H_
#define JSON_H_



char * getjsonid(char* jsonstring, const char* key, TCHAR* value, int check);
int getepisodesnum(char* jsonstring, const char* key);
int parseresultsjson(char* jsonstring, result results[]);
int parseepisodesjson(HWND hwnd, char * resultid, char* jsonstring, episode episodes[]);
char * getlinkjson(HWND hwnd, char * jsonstring);
int gettrendinginfo(char * jsonstring, trendinganimeinfo results[]);

#endif