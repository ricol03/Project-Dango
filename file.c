#include "tools.h"

FILE * fr;
FILE * fw;

extern HWND hwndnetworktab;

extern HWND hproviderlist;
extern HWND hserverbox, hportbox;

extern size_t stringsize;

extern DWORD wversion, wmajorversion, wminorversion, wbuild;

wchar_t server[32];
wchar_t provider[32];
wchar_t protocol[6];
wchar_t port[6];

wchar_t lang[5];

TCHAR appdata[128];

void createDirectory() {
    if (wmajorversion == 5) {
        HRESULT result = SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, appdata);
        wcscat(appdata, L"\\ProjectDango\\");

        if (checkDirectory(appdata)) {
            BOOL check = CreateDirectory(appdata, NULL);

            if (check == 0) {
                MessageBox(NULL, L"The folder could not be created", L"Error", MB_ICONERROR);
                printf("\n\nGetLastError: %lu\n\n", GetLastError());
            }
        }
    } else {
        HRESULT result = SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, appdata);
        wcscat(appdata, L"\\AppData\\Local\\ProjectDango\\");

        if (checkDirectory(appdata) == -1) {
            BOOL check = CreateDirectory(appdata, NULL);

            if (check == 0) {
                MessageBox(NULL, L"The folder could not be created", L"Error", MB_ICONERROR);
                printf("\n\nGetLastError: %lu\n\n", GetLastError());
            }
        }
    } 
}

int checkDirectory(TCHAR * pathname) {
    DWORD check = GetFileAttributes(pathname);
    return (int)check;
}

wchar_t * checkImage(wchar_t * showid) {
    wchar_t * imagepath = (wchar_t *)malloc(128 * sizeof(wchar_t));
    wchar_t * imagepathaux = (wchar_t *)malloc(128 * sizeof(wchar_t));

    wcscpy(imagepath, appdata);
    wcscat(imagepath, showid);

    wcscpy(imagepathaux, imagepath);
    wcscat(imagepath, L".jpg");

    printf("\n\nchegou ao checkimage");

    DWORD check = GetFileAttributes(imagepath);
    if (check != -1) {
        printf("   \n\n sabeque é jpg");
        return imagepath;
    } else {
        wcscpy(imagepath, imagepathaux);
        wcscat(imagepath, L".png");

        DWORD check = GetFileAttributes(imagepath);
        if (check != -1) {
            return imagepath;
        } else {
            wcscpy(imagepath, imagepathaux);
            wcscat(imagepath, L".gif");

            DWORD check = GetFileAttributes(imagepath);
            if (check == -1) {
                return NULL;
            } else {
                return imagepath;
            }
        }
    }
}


//TODO: I need to know the file extension of the file to create
wchar_t * createImagePath(wchar_t * showid, wchar_t * fileextension) {

    printf("\n\n\nfoi corajoso e chegou aqui");

    wchar_t * filepath = (wchar_t *)malloc(128 * sizeof(wchar_t));
    wcscpy(filepath, appdata);
    wcscat(filepath, showid);
    wcscat(filepath, fileextension);

    return filepath;

    /*DWORD byteswritten;
    HANDLE hf = CreateFile(filepath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    //FILE * fw = fopen(filepath, "w+b");

    if (hf != NULL) {
        WriteFile(hf, imagedata, stringsize, &byteswritten, NULL);z
        //fwrite(imagedata, 1, 1, fw);
        //fclose(fw);z
        CloseHandle(hf);
        return filepath;
    } else {
        MessageBox(NULL, L"The file could not be created", L"Error", MB_ICONERROR);
        fclose(fw);
        return NULL;
    }*/

    /*HANDLE hf = CreateFile(filepath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hf == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, L"The file could not be created", L"Error", MB_ICONERROR);
        free(filepath);
        return NULL;
    }

    // Escreve os dados da imagem no arquivo
    DWORD byteswritten;
    BOOL result = WriteFile(hf, imagedata, stringsize, &byteswritten, NULL);
    if (!result || byteswritten != stringsize) {
        MessageBox(NULL, L"Failed to write the image data to the file", L"Error", MB_ICONERROR);
        CloseHandle(hf);
        free(filepath);
        return NULL;
    }

    // Fecha o arquivo
    CloseHandle(hf);*/

}

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
        MessageBox(NULL, L"Settings not found", L"Error", MB_ICONERROR);
    } 

    fclose(fr);
}

int saveSettings() {
    fw = fopen(SETTINGSFILENAME, "wb");

    GetWindowText(hserverbox, server, 64);

    MessageBox(NULL, server, L"Info", MB_ICONINFORMATION);

    if (SendDlgItemMessage(hwndnetworktab, IDW_NETWORK_RADIO_HTTP, BM_GETCHECK, 0, 0) == BST_CHECKED) {
        wcscpy(protocol, L"HTTP");
        MessageBox(NULL, L"1", L"Warning", MB_ICONWARNING);
    } else if (SendDlgItemMessage(hwndnetworktab, IDW_NETWORK_RADIO_HTTPS, BM_GETCHECK, 0, 0) == BST_CHECKED) {
        wcscpy(protocol, L"HTTPS");
        MessageBox(NULL, L"2", L"Warning", MB_ICONWARNING);
    } else {
        MessageBox(NULL, L"No protocol specified, default will be HTTP.", L"Warning", MB_ICONWARNING);
        wcscpy(protocol, L"HTTP");
    }
    
    fwrite(server, sizeof(server), 1, fw);
    fwrite(protocol, sizeof(protocol), 1, fw);
    if (SendDlgItemMessage(hwndnetworktab, IDW_NETWORK_CHECKBOX_PORT, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
        GetWindowText(hportbox, port, sizeof(port));
        printf("000000000");
    } else {
        if (wcscmp(protocol, L"HTTP")) {
            printf("aaaaa");
            wcscpy(port, L"80");
        } else if (wcscmp(protocol, L"HTTPS")) {
            printf("bbbbbb");
            wcscpy(port, L"443");
        } else 
            MessageBox(NULL, L"error", L"error", MB_ICONERROR); 
    }
    
    fwrite(port, sizeof(port), 1, fw);
    fwrite(provider, sizeof(provider), 1, fw);
    

    printf("\n%ls", server);
    printf("\n%ls", protocol);
    printf("\n%ls", port);
    printf("\n%ls", provider);
    
    //fwrite(lang, sizeof(lang), 1, fw);

    //fprintf(fw, provider);

    fclose(fw);
}

