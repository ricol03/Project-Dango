#include "tools.h"

extern char provider[32];

char auxmatrix1[MAXCONNECTIONS][64] = {"/anime/gogoanime/", "/anime/gogoanime/info/", "/anime/gogoanime/watch/", "d", "e"};
char auxmatrix2[MAXCONNECTIONS][64] = {"/anime/zoro/", "/anime/zoro/info?id=", "/anime/zoro/watch?episodeId=", "d", "e"};


/*int checkNumProviders() {
    int i = 0;
    while (table->value[i] != NULL) {
        ++i;
    }
}*/

//TODO: how do I know when I must change strings if the user swaps provider??
int selectStringMatrix() {
    //if (strmatrix[0] == NULL) {
        if (!strcmp(provider, PROVIDER1)){
            for (int i = 0; i < MAXCONNECTIONS; i++) {
                strcpy(strmatrix[i], auxmatrix1[i]);
                printf("\n\ntest %d: %s", i, strmatrix[i]);
            }
        } else if (!strcmp(provider, PROVIDER2)) {
            //TODO: move this to provider.h
            
            for (int i = 0; i < MAXCONNECTIONS; i++) {
                strcpy(strmatrix[i], auxmatrix2[i]);
                printf("\n\ntest %d: %s", i, strmatrix[i]);
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