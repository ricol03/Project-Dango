#include "tools.h"

FILE * fr;
FILE * fw;

extern HWND hproviderlist;

extern const wchar_t provider;

void readyingFile() {
    fr = fopen(SETTINGSFILENAME, "rb");
    fw = fopen(SETTINGSFILENAME, "wb");
}

int readSettings() {
    fr = fopen(SETTINGSFILENAME, "rb");

    fseek(fr, 0, SEEK_END);
    if (ftell(fr) > 0) {
        MessageBox(NULL, "Settings found", "Info", MB_ICONINFORMATION);
        //provider = fread()
    } else {
        MessageBox(NULL, "Settings not found", "Error", MB_ICONERROR);
    } 
    
}

int saveSettings() {
    fw = fopen(SETTINGSFILENAME, "wb");
    fwrite(provider, sizeof(provider) * sizeof(wchar_t), NULL, fw);


}
