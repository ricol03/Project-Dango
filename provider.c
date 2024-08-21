#include "tools.h"

extern wchar_t provider[32];

wchar_t strmatrix[MAXCONNECTIONS][64];

wchar_t auxmatrix1[MAXCONNECTIONS][64] = {L"/anime/gogoanime/", L"/anime/gogoanime/info/", L"/anime/gogoanime/watch/", L"d", L"e"};
wchar_t auxmatrix2[MAXCONNECTIONS][64] = {L"/anime/zoro/", L"/anime/zoro/info?id=", L"/anime/zoro/watch?episodeId=", L"d", L"e"};


/*int checkNumProviders() {
    int i = 0;
    while (table->value[i] != NULL) {
        ++i;
    }
}*/

//TODO: how do I know when I must change strings if the user swaps provider?? (solved)
int selectStringMatrix() {
    //if (strmatrix[0] == NULL) {
        if (!wcscmp(provider, PROVIDER1)){
            for (int i = 0; i < MAXCONNECTIONS; i++) {
                wcscpy(strmatrix[i], auxmatrix1[i]);
                printf("\n\ntest %d: %ls", i, strmatrix[i]);
            }
        } else if (!wcscmp(provider, PROVIDER2)) {
            //TODO: move this to provider.h
            
            for (int i = 0; i < MAXCONNECTIONS; i++) {
                wcscpy(strmatrix[i], auxmatrix2[i]);
                printf("\n\ntest %d: %ls", i, strmatrix[i]);
            }
        }   
    /*} else {
        MessageBox(NULL, "Matrix has already been initialized!", "Warning", MB_ICONWARNING);
    }*/
    
}

int initializeStringMatrix() {
    if (strmatrix == NULL) {
        for (int i = 0; i < checkNumProviders(); i++) {
            switch (i) {
                case 0:
                    
                    break;
                
                default:
                    break;
            }
            
        }
        
    } else {
        MessageBox(NULL, "Matrix has already been initialized!", "Warning", MB_ICONWARNING);
    }
    
}