#include "tools.h"

FILE * fr;
FILE * fw;

extern HWND hproviderlist;

char provider[32];
char lang[5];

void readyingFile() {
    fr = fopen(SETTINGSFILENAME, "r");
    fw = fopen(SETTINGSFILENAME, "w");
}

int readSettings() {
    fr = fopen(SETTINGSFILENAME, "r+b");

    if (fr != NULL) {
        MessageBox(NULL, "Settings found", "Info", MB_ICONINFORMATION);
        char savedProvider[32] = {0};
        fread(savedProvider, sizeof(savedProvider), 1, fr);

        strcpy(provider, savedProvider);

        MessageBox(NULL, savedProvider, "Info", MB_ICONINFORMATION);
        MessageBox(NULL, provider, "Info", MB_ICONINFORMATION);
    } else {
        MessageBox(NULL, "Settings not found", "Error", MB_ICONERROR);
    } 

    fclose(fr);
}

int saveSettings() {
    fw = fopen(SETTINGSFILENAME, "w+b");

    fwrite(provider, sizeof(provider), 1, fw);
    //fwrite(lang, sizeof(lang), 1, fw);

    //fprintf(fw, provider);

    MessageBox(NULL, provider, "Info", MB_ICONASTERISK);

    fclose(fw);
}

