#include "tools.h"

FILE * fr;
FILE * fw;

extern HWND hwndnetworktab;

extern HWND hproviderlist;
extern HWND hserverbox, hportbox;

char server[32];
char provider[32];
char protocol[6];
char port[6];

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
        fread(protocol, sizeof(protocol), 1, fr);
        fread(port, sizeof(port), 1, fr);
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

    if (SendDlgItemMessage(hwndnetworktab, IDW_NETWORK_RADIO_HTTP, BM_GETCHECK, 0, 0) == BST_CHECKED) {
        strcpy(protocol, "HTTP");
    } else if (SendDlgItemMessage(hwndnetworktab, IDW_NETWORK_RADIO_HTTPS, BM_GETCHECK, 0, 0) == BST_CHECKED) {
        strcpy(protocol, "HTTPS");
    } else {
        MessageBox(NULL, "No protocol specified, default will be HTTP.", "Warning", MB_ICONWARNING);
        strcpy(protocol, "HTTP");
    }
    
    fwrite(server, sizeof(server), 1, fw);
    fwrite(protocol, sizeof(protocol), 1, fw);
    if (SendDlgItemMessage(hwndnetworktab, IDW_NETWORK_CHECKBOX_PORT, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
        GetWindowText(hportbox, port, sizeof(port));
        printf("000000000");
    } else {
        if (!strcmp(protocol, "HTTP")) {
            printf("aaaaa");
            strcpy(port, "80");
        } else if (!strcmp(protocol, "HTTPS")) {
            printf("bbbbbb");
            strcpy(port, "443");
        } else 
            MessageBox(NULL, "error", "error", MB_ICONERROR); 
    }
    
    fwrite(port, sizeof(port), 1, fw);
    fwrite(provider, sizeof(provider), 1, fw);
    
    
    //fwrite(lang, sizeof(lang), 1, fw);

    //fprintf(fw, provider);

    fclose(fw);
}

