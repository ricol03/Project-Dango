#include "tools.h"

FILE * fr;
FILE * fw;

extern HWND hproviderlist;
extern HWND hserverbox, hportbox;

char server[32];
char provider[32];

char lang[5];

void readyingFile() {
    fr = fopen(SETTINGSFILENAME, "r");
    fw = fopen(SETTINGSFILENAME, "w");
}

int readSettings() {
    fr = fopen(SETTINGSFILENAME, "r+b");

    if (fr != NULL) {
        //char savedProvider[32] = {0};
        fread(server, sizeof(server), 1, fr);
        fread(provider, sizeof(provider), 1, fr);
        
        //strcpy(provider, savedProvider);
    } else {
        MessageBox(NULL, "Settings not found", "Error", MB_ICONERROR);
    } 

    fclose(fr);
}

int saveSettings() {
    fw = fopen(SETTINGSFILENAME, "wb");

    GetWindowText(hserverbox, server, 64);

    MessageBox(NULL, server, "Info", MB_ICONINFORMATION);

    fwrite(server, sizeof(server), 1, fw);
    fwrite(provider, sizeof(provider), 1, fw);
    
    //fwrite(lang, sizeof(lang), 1, fw);

    //fprintf(fw, provider);

    fclose(fw);
}

