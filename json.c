#include "tools.h"

extern char provider[32];

//criação da união para albergar um de dois tipos de dados
connections test;

wchar_t * getJsonId(wchar_t * jsonstring, const wchar_t * key, TCHAR * value, int check) {
    const wchar_t * endquote = L"\"";
    const wchar_t * resseparator = L"},{";
    const wchar_t * endbracket = L"}]}";

    if (jsonstring == NULL) {
        MessageBox(NULL, L"Error with JSON string", L"Error", MB_ICONERROR | MB_OK);
        //return test;
        return NULL;
    }

    wprintf(L"String de entrada: \n\n%ls\n\n", jsonstring);

    //printf("\n\n cheque %d", check);

    //MessageBox(NULL, L"chega", L"aqui", MB_ICONASTERISK);

    if (check == 1) {
        wchar_t * check = wcsstr(jsonstring, resseparator);
        //printf("OLHA AQUI: \n\n%s\n\n", check);

        //if there isn't a match with resseparator
        if (check == NULL) {
            wchar_t * check2 = wcsstr(jsonstring, endbracket);
            //if there isn't a match with endbracket
            if (check2 == NULL) {
                MessageBox(NULL, L"Couldn't find the end bracket normally. Forcefully ending...", L"Error", MB_ICONERROR);
                return NULL;
            } else {
                return NULL;
            }
            
            return jsonstring;
        } else {
            check += wcslen(resseparator);
            printf("\n\ndeu aí o coiso maluco: %s", check);
            return check;
        }
    }

    //printf("\n\nantes do cheque: \n%s", jsonstring);z
    //printf("\n\n cheque %d", check);
    //"verifica" se a string imagekey existe na jsonstring

    //ele queima-se todo aqui man, vê lá isso
    wprintf(L"\n\nBefore check: \n%ls", jsonstring);
    wprintf(L"\n\nKey: %ls\n\n", key);
    wchar_t * start = wcsstr(jsonstring, key);
    wprintf(L"Start value: %ls\n", start);
    wprintf(L"JSON value: %s\n", jsonstring);

    printf("valor do start: %s", start);
    printf("valor do json: %s", jsonstring);
    
    //se existir
    if (start != NULL) {
        //move o ponteiro para a posição após a string imagekey
        start += wcslen(key);

        //printf("\n\npasso 2");

        wchar_t * end = wcsstr(start, endquote);
        printf("\n\n\nEle dá print disto amigo (end): %s", end);

        if (end != NULL) {
            //printf("\n\npasso 3");
            //calcula o tamanho entre o início e o fim do link da imagem
            size_t length = end - start;

            //coloca na struct a string da posição start até start + length
            wcsncpy(value, start, length);
            //termina a string com um \0
            
            if (check != 3) {
                value[length] = '\0';
                printf("\nvalor: %s", value);

                /*test.value = start;
                return test;    */
                return value;
            }
        } else {
            return NULL;
        }
    } else {
        MessageBox(NULL, (LPCWSTR)jsonstring, L"Error", MB_ICONERROR);
        return NULL;
    }
}

int getEpisodesNum(wchar_t * jsonstring, const wchar_t * key) {

    //printf("\n\n\nele chegou ao getepisodesnum");
    //printf("\ncom isto: %s", jsonstring);

    const wchar_t * start = strstr(jsonstring, key);
    //printf("\n\n\nstart do episodes: %s", start);
    //printf("\n\n\n\nposição do key %c", start[strlen(key)]);

    if (start != NULL) {
        // Check if the key is standalone (not part of another key)
        if (start[strlen(key) - 1] == ':' || start[strlen(key) - 1] == ' ') {
            int value;
            if (sscanf(start + strlen(key), "%d", &value) == 1) {
                //printf("\n\n\n\n\nencontrou o coiso bom: %d", value);
                return value;
            } else {
                //printf("\n\n\n\n\nnão encontrou nada doutor");
            }
                
            //printf("\n\n\n\n\n\nstarto: %s", start);
            //printf("\n\nFound key: %s\n", key);
            
        } else {
            //printf("\n\n\n\n\nnão verificou direitinho");
        }
        
    } else {
        //printf("\n\n\n\n\nnem o start é nulo");
    }
}

int parseResultsJson(wchar_t * jsonstring, result results[]) {

    //cria uma string do início da chave de cada atributo json
    const wchar_t * idkey = L"\"id\":\"";
    const wchar_t * titlekey = L"\"title\":\"";
    const wchar_t * ratingkey = L"\"rating\":\"";
    const wchar_t * reldatekey = L"\"releaseDate\":\"";
    const wchar_t * typekey = L"\"type\":\"";

    //printf("\n\n\nchegou ao parseresults");

    //printf("\n\n\n\nprint aí man: %s", jsonstring);

    //TODO: change this to a do while or a while
    for (int i = 0; i < 8; i++) {
        getJsonId(jsonstring, idkey, results[i].id, 0);
        MessageBox(NULL, L"antes do izaias", L"WARNING", MB_ICONWARNING);
        MessageBox(NULL, results[i].id, L"Info", MB_ICONINFORMATION);
        //printf("\n\nid do resultado: %s", results[i].id);
        getJsonId(jsonstring, titlekey, results[i].title, 0);
        MessageBox(NULL, results[i].title, L"Info", MB_ICONINFORMATION);
        //printf("\n\ntítulo do resultado: %s", results[i].title);
        //getJsonId(jsonstring, ratingkey, results[i].rating, 0);
        //printf("\n\nrating do resultado: %s", results[i].rating);
        //getJsonId(jsonstring, reldatekey, results[i].releasedate, 0);
        //printf("\n\ndata de lançamento do resultado: %s", results[i].releasedate);
        
        //anteriormente test (union)
        jsonstring = getJsonId(jsonstring, typekey, results[i].type, 1);
        //printf("\n\ntipo do resultado: %s", results[i].type);
        //jsonstring = test.value;

        if (jsonstring == NULL) {
            if (i == 0)
                return 1;
            else 
                return i;
        }
            
    }

    return 8;        
}

int parseEpisodesJson(HWND hwnd, wchar_t * resultid, wchar_t * jsonstring, episode episodes[]) {

    int number = epnumConnection(hwnd, resultid);

    printf("que número é este meu senhor? %d", number);
    const wchar_t * totalepisodeskey = "\"totalEpisodes\":";
    const wchar_t * episodekey = "\"episodes\":";

    printf("\n\n\n vê lá a string no peej: %s", jsonstring);

    wchar_t * newstring = strstr(jsonstring, totalepisodeskey);
    wchar_t * newstring2 = strstr(newstring, episodekey);

    //cria uma string do início da chave de cada atributo json
    const wchar_t * idkey = "\"id\":\"";
    const wchar_t * numberkey = "\"number\":";
    const wchar_t * titlekey = "\"title\":\"";

    for (int i = 0; i < number; i++) {
        getJsonId(newstring2, idkey, episodes[i].id, 0);
        getJsonId(newstring2, numberkey, episodes[i].number, 0);
        
        //anteriormente test (union)
        getJsonId(newstring2, titlekey, episodes[i].title, 0);
        //newstring2 = test.value;

        newstring2 = getJsonId(newstring2, NULL, NULL, 1);

        if (newstring2 == NULL) {
            return ++i;
        }
            
    }

    return number;
}

wchar_t * getLinkJson(HWND hwnd, wchar_t * jsonstring) {

    if (jsonstring == NULL) {
        return NULL;
    }
    
    const wchar_t * linkkey = "\"url\":";
    const wchar_t * endquote = "\"";

    wchar_t * value = malloc(2048*sizeof(wchar_t));
    //malloc(value);

    wchar_t * start = strstr(jsonstring, linkkey);
    printf("\n\n\nEle dá print disto amigo (start): %s", start);

    if (start != NULL) {
        // Check if the key is standalone (not part of another key)

        printf("\n\n\n\n\n\n\nchar do síituio: %c", start[strlen(linkkey) - 1]);
        if (start[strlen(linkkey) - 1] == ':' || start[strlen(linkkey) - 1] == ' ') {
            
            if (sscanf(start + strlen(linkkey), "\"%2048s\"", value) == 1) {
                printf("\n\n\n\n\nencontrou o coiso bom: %s", value);
                return value;
            } else {
                printf("\n\n\n\n\nnão encontrou nada doutor");
                printf("\n\n\n\n\nnão encontrou o coiso bom: %s", value);
                free(value);
                return NULL;
            }
                
            printf("\n\n\n\n\n\nstarto: %s", start);
            printf("\n\nFound key: %s\n", linkkey);
            
        } else {
            printf("\n\n\n\n\nnão verificou direitinho");
            free(value);
            return NULL;
        }
        

        
        
    } else {
        printf("\n\n\n\n\nnem o start é nulo");
        free(value);
        return NULL;
    }
}

int getQualitiesJson(wchar_t * jsonstring, stream streams[]) {
    const wchar_t * linkkey = "\"url\":\"";
    const wchar_t * qualitykey = "\"quality\":\"";

    int i = 0;
    while (jsonstring != NULL) {
        getJsonId(jsonstring, linkkey, streams[i].link, 0);
        getJsonId(jsonstring, qualitykey, streams[i].quality, 0);
        jsonstring = getJsonId(jsonstring, NULL, NULL, 1);
        ++i;
    }
    
    return 1;
}

int getTrendingShows(wchar_t * jsonstring, trendinganimeinfo results[]) {
    const wchar_t * idkey = "{\"id\":\"";
    const wchar_t * titlekey = "\"title\":\"";
    const wchar_t * imagekey = "\"image\":\"";
    const wchar_t * endquote = "\"";

    printf(" - - - - trendinginfo\n\n\n");
    printf("%s", jsonstring);

    for (int i = 0; i < 12; i++) {
        getJsonId(jsonstring, idkey, results[i].id, 3);
        printf("\n\nid do resultado: %s", results[i].id);
        getJsonId(jsonstring, titlekey, results[i].title, 3);
        printf("\n\ntítulo do resultado: %s", results[i].title);
        getJsonId(jsonstring, imagekey, results[i].imageurl, 3);
        printf("\n\nimagem do resultado: %s", results[i].imageurl);
        jsonstring = getJsonId(jsonstring, NULL, NULL, 1);

        printf("\n\n\n\n\n %d", i);

        if (jsonstring == NULL)
            return i;
    }
}

animeinfo getShowInfo(wchar_t * jsonstring, animeinfo show) {
    const wchar_t * totalepkey = "\"totalEpisodes\":";
    const wchar_t * imagekey = "\"image\":\"";
    const wchar_t * datekey = "\"releaseDate\":\"";
    const wchar_t * descriptionkey = "\"description\":\"";
    const wchar_t * dubkey = "\"subOrDub\":\"";
    const wchar_t * typekey = "\"type\":\"";
    const wchar_t * statuskey = "\"status\":\"";
    const wchar_t * endquote = "\"";

    BOOL isdub;

    getJsonId(jsonstring, totalepkey, show.episodes, 3);
    getJsonId(jsonstring, imagekey, show.imageurl, 3);
    getJsonId(jsonstring, datekey, show.date, 3);
    getJsonId(jsonstring, descriptionkey, show.description, 3);
    //getJsonId(jsonstring, dubkey, show.isdub, 3);
    MessageBox(NULL, show.isdub, "Info", MB_ICONINFORMATION);
    getJsonId(jsonstring, typekey, show.type, 3);
    getJsonId(jsonstring, statuskey, show.status, 3); 

    show.isempty = FALSE;

    return show;
    
}

