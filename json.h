#include <Windows.h>
#include <winhttp.h>
#include "connections.h"

#ifndef JSON_H_
#define JSON_H_

connections getjsonid(char* jsonstring, const char* key, TCHAR* value, int check);
int getepisodesnum(char* jsonstring, const char* key);
int parseresultsjson(char* jsonstring, result results[]);
int parseepisodesjson(HWND hwnd, char * resultid, char* jsonstring, episode episodes[]);
char * getlinkjson(HWND hwnd, char * jsonstring);


#endif