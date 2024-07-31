#include <windows.h>
#include "connections.h"

#ifndef PROVIDER_H_
#define PROVIDER_H_

#define PROVIDER1 "Gogoanime"
#define PROVIDER2 "Zoro"


char strmatrix[5][64];

/*typedef struct { 
    char * key;
    char * value;
} providers;

static providers table[] = {
    {"PROVIDER1", PROVIDER1}, 
};*/

int checkNumProviders();
int selectStringMatrix();
int initializeStringMatrix();

#endif