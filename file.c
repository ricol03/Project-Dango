#include "tools.h"

FILE * fr;
FILE * fw;

extern HWND hproviderlist;

const char provider[32];
const char lang[5];

void readyingFile() {
    fr = fopen(SETTINGSFILENAME, "r");
    fw = fopen(SETTINGSFILENAME, "w");
}

int readSettings() {
    fr = fopen(SETTINGSFILENAME, "r+b");

    if (fr != NULL) {
        //MessageBox(NULL, "Settings found", "Info", MB_ICONINFORMATION);
        char savedProvider[32];
        fread(savedProvider, sizeof(savedProvider), 1, fr);

        strcpy(provider, savedProvider);

        printf("Original: %s\n", provider);
        printf("Resultado: %s", savedProvider);
        MessageBox(NULL, savedProvider, "Info", MB_ICONINFORMATION);
    } else {
        MessageBox(NULL, "Settings not found", "Error", MB_ICONERROR);
    } 

    fclose(fr);
}

int saveSettings() {
    fw = fopen(SETTINGSFILENAME, "w+b");
    
    fwrite(provider, sizeof(provider), 1, fw);
    fwrite(lang, sizeof(lang), 1, fw);

    //fprintf(fw, provider);

    fclose(fw);
}

