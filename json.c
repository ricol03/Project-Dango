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
        return NULL;
    }

    printf("\n\n%ls\n", jsonstring);

    if (check == 1) {
        wchar_t * check = wcsstr(jsonstring, resseparator);

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

    wchar_t * start = wcsstr(jsonstring, key);
    
    //se existir
    if (start != NULL) {
        //move o ponteiro para a posição após a string imagekey
        start += wcslen(key);
        wchar_t * end = wcsstr(start, endquote);

        if (end != NULL) {
            //calcula o tamanho entre o início e o fim do link da imagem
            size_t length = end - start;

            //coloca na struct a string da posição start até start + length
            wcsncpy(value, start, length);
            //termina a string com um \0
            
            if (check != 3) {
                value[length] = '\0';
                return value;
            }
        } else {
            return NULL;
        }
    } else {
        MessageBox(NULL, L"The program wasn't able to retrieve a value, functionality limited.", L"Error", MB_ICONERROR);
        return NULL;
    }
}

wchar_t * getGenres(wchar_t * jsonstring, const wchar_t * key, TCHAR value[][32]) {
    /*const wchar_t * endquote = L"\"";
    const wchar_t * middlecomma = L"\",\"";
    const wchar_t * endbracket = L"\"]";

    int commacounter = 0;

    if (jsonstring == NULL) {
        MessageBox(NULL, L"Error with JSON string", L"Error", MB_ICONERROR | MB_OK);
        return NULL;
    }

    wchar_t * start;
    BOOL check = TRUE;

    //while (check != FALSE) {
        start = wcsstr(jsonstring, key);

        int i = 0;

        for (i; 1; i++) {
            commacounter += (start[i] == ',');
            if (start[i] == ']') 
                break; 
        }

        printf("\n\ncomma counter: %d", commacounter);
        
    //}
    
    
    for (int j = 0; j < i; j++) {
        
        if (j == 0)
           start = wcsstr(jsonstring, key);

        printf("\nstart: %ls\n", start);
        
        //se existir
        if (start != NULL) {
            //move o ponteiro para a posição após a string imagekey
            if (j == 0)
                start += wcslen(key);
            else {
                //int j = i;
                //start += wcslen(value[--j]);
                start += wcslen(middlecomma);
            }
                
            wchar_t * end;
    
            

            if (end != NULL && j <= i - 1) {
                end = wcsstr(start, middlecomma);
           
                printf("\nend: %ls\n", end);
                //calcula o tamanho entre o início e o fim do link da imagem
                size_t length = end - start;

                printf("\nlength: %d\n", length);

                //coloca na struct a string da posição start até start + length
                wcsncpy(value[j], start, length);

                printf("\nvalue: %ls\n", value[j]);

                //wcscpy(jsonstring, end);

                start = wcsstr(end, middlecomma);

                if (start != NULL)
                    continue;
                else 
                    break;
            
            } else {

                printf("\n\n\nveio aqui");
                end = wcsstr(start, endbracket);

                //if (end != NULL) {
                    //calcula o tamanho entre o início e o fim do link da imagem
                    size_t length = end - start;

                    printf("\nlength: %d\n", length);

                    //coloca na struct a string da posição start até start + length
                    wcsncpy(value[j], start, length);

                    printf("\nvalue: %ls\n", value[j]);
                //} 

                //break;
                //return NULL;
            }
        } else {
            MessageBox(NULL, L"The program wasn't able to retrieve a value, functionality limited.", L"Error", MB_ICONERROR);
            return NULL;
        }
    } 

    for (int i = 0; i < 5; i++) {
        printf("\nvalue %d: %s", i, value[i]);
    }*/
    
    const wchar_t * middlecomma = L"\",\"";
    const wchar_t * endbracket = L"\"]";
    int genreCount = 0;

    if (jsonstring == NULL) {
        MessageBox(NULL, L"Error with JSON string", L"Error", MB_ICONERROR | MB_OK);
        return NULL;
    }

    int i = 0, commacounter = 0;

    wchar_t * start = wcsstr(jsonstring, key);
    if (start == NULL) {
        MessageBox(NULL, L"Key not found in JSON string", L"Error", MB_ICONERROR);
        return NULL;
    }

    for (i; 1; i++) {
        commacounter += (start[i] == ',');
        if (start[i] == ']') 
            break; 
    }

    // Move the start pointer to the first genre after the key
    start += wcslen(key);

    // Extract genres
    for (int i = 0; i <= commacounter; i++) { 
        wchar_t * end = wcsstr(start, middlecomma);
        if (i == commacounter) {
            // If no more commas, look for the closing bracket
            end = wcsstr(start, endbracket);
        }

        if (end != NULL) {
            size_t length = end - start;

            // Copy the genre into the value array
            wcsncpy(value[i], start, length);

            genreCount++;

            // Move start to after the comma for the next genre
            start = end + wcslen(middlecomma);
        } else {
            break;
        }
    }

    // Print the genres for debugging
    for (int i = 0; i < genreCount; i++) {
        wprintf(L"Genre %d: %ls\n", i + 1, value[i]);
    }

    return jsonstring;

}

wchar_t * getLinkDomain(wchar_t * url) {
    const wchar_t * protocol = L"https://";
    const wchar_t * endslash = L"/";

    wchar_t * value = (wchar_t *)malloc(128 * sizeof(wchar_t));
    ZeroMemory(value, 128+1);

    wchar_t * start = wcsstr(url, protocol);
    
    //se existir
    if (start != NULL) {
        //move o ponteiro para a posição após a string imagekey
        start += wcslen(protocol);
        wchar_t * end = wcsstr(start, endslash);

        if (end != NULL) {
            //calcula o tamanho entre o início e o fim do link da imagem
            size_t length = end - start;

            //coloca na struct a string da posição start até start + length
            wcsncpy(value, start, length);
            value[length] = '\0';
            printf("\n\nvalue: %ls", value);
            return value;
        } else {
            return NULL;
        }
    } else {
        MessageBox(NULL, L"The program wasn't able to retrieve the link, functionality limited.", L"Error", MB_ICONERROR);
        return NULL;
    }

}

wchar_t * getLinkSubdomain(wchar_t * url, wchar_t * server) {

    printf("\n\nseerver: %ls", server);

    const wchar_t * endslash = L"/";
    wchar_t * value = (wchar_t *)malloc(128 * sizeof(wchar_t));

    wchar_t * start = wcsstr(url, server);

    printf("\n\n\nstart: %ls", start);
    
    //se existir
    if (start != NULL) {
        //move o ponteiro para a posição após a string imagekey
        start += wcslen(server);
        wchar_t * end = wcsstr(start, endslash);

        printf("\n\n\nend: %ls", end);

        return end;

        /*if (end != NULL) {
            //calcula o tamanho entre o início e o fim do link da imagem
            size_t length = end - start;

            //coloca na struct a string da posição start até start + length
            wcsncpy(value, end, length);
            printf("\n\nvalue: %ls", value);
            return value;
        } else {
            return NULL;
        }*/
    } else {
        MessageBox(NULL, L"The program wasn't able to retrieve the subdomain, functionality limited.", L"Error", MB_ICONERROR);
        return NULL;
    }
}

wchar_t * getFileExtension(wchar_t * url) {
    const wchar_t * jpgextension = L".jpg";
    const wchar_t * pngextension = L".png";
    const wchar_t * gifextension = L".gif";

    if (wcsstr(url, jpgextension) != NULL)
        return jpgextension;
    else if (wcsstr(url, pngextension) != NULL)
        return pngextension;
    else if (wcsstr(url, gifextension) != NULL)
        return gifextension;
    else
        return NULL;
}

int getEpisodesNum(wchar_t * jsonstring, const wchar_t * key) {

    const wchar_t * endcomma = L",";

    //printf("\n\n\nele chegou ao getepisodesnum");
    //printf("\ncom isto: %ls", jsonstring);

    wchar_t * start = wcsstr(jsonstring, key);
    printf("\n\n\nstart do episodes: %ls", start);
    printf("\n\n\n\nposição do key: %c", start[wcslen(key)]);

    if (start != NULL) {
        // Check if the key is standalone (not part of another key)
        //if (start[wcslen(key) - 1] == ':' || start[wcslen(key) - 1] == ' ') {

            start += wcslen(key);

            int value = 0;

            wchar_t * end = wcsstr(start, endcomma);
       
            if (end != NULL) {
                size_t length = end - start;

                wchar_t * a = (wchar_t*)malloc(4 * sizeof(wchar_t));

                wcsncpy(a, start, length);

                printf("\n\nvalue char: %ls", a);

                value = _wtoi(a);

                printf("\n\nvalue int: %d", value);

                return value;

            }




            /*if (wscanf(start + wcslen(key), "%d", &value) == 1) {
                //printf("\n\n\n\n\nencontrou o coiso bom: %d", value);


                MessageBox(NULL, L"Encontrou o valor de episoódios", L"Info", MB_ICONINFORMATION);
                return value;
            } else {
                MessageBox(NULL, L"N deu", L"Info", MB_ICONINFORMATION);

                //printf("\n\n\n\n\nnão encontrou nada doutor");
            }*/


                
            //printf("\n\n\n\n\n\nstarto: %ls", start);
            //printf("\n\nFound key: %ls\n", key);
            
        /*} else {
            printf("\n\n\n\n\nnão verificou direitinho");
        }*/
        
    } else {
        MessageBox(NULL, L"N deu 2", L"Info", MB_ICONINFORMATION);
        printf("\n\n\n\n\nnem o start é nulo");
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
        //MessageBox(NULL, L"antes do izaias", L"WARNING", MB_ICONWARNING);
        //MessageBox(NULL, results[i].id, L"Info", MB_ICONINFORMATION);
        //printf("\n\nid do resultado: %s", results[i].id);
        getJsonId(jsonstring, titlekey, results[i].title, 0);
        //MessageBox(NULL, results[i].title, L"Info", MB_ICONINFORMATION);
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

    //printf("que número é este meu senhor? %d", number);
    const wchar_t * totalepisodeskey = L"\"totalEpisodes\":";
    const wchar_t * episodekey = L"\"episodes\":";

    //printf("\n\n\n vê lá a string no peej: %s", jsonstring);

    wchar_t * newstring = wcsstr(jsonstring, totalepisodeskey);
    wchar_t * newstring2 = wcsstr(newstring, episodekey);

    //cria uma string do início da chave de cada atributo json
    const wchar_t * idkey = L"\"id\":\"";
    const wchar_t * numberkey = L"\"number\":";
    const wchar_t * titlekey = L"\"title\":\"";

    for (int i = 0; i < number; i++) {
        getJsonId(newstring2, idkey, episodes[i].id, 0);
        //MessageBox(NULL, episodes[i].id, L"Info", MB_ICONEXCLAMATION);
        getJsonId(newstring2, numberkey, episodes[i].number, 0);
        //MessageBox(NULL, episodes[i].number, L"Info", MB_ICONEXCLAMATION);
        
        //anteriormente test (union)
        getJsonId(newstring2, titlekey, episodes[i].title, 0);
        //MessageBox(NULL, episodes[i].title, L"Info", MB_ICONEXCLAMATION);
        //newstring2 = test.value;

        newstring2 = getJsonId(newstring2, NULL, NULL, 1);

        if (newstring2 == NULL) {
            return ++i;
        }
            
    }

    return number;
}

char * getLinkJson(HWND hwnd, char * jsonstring) {

    if (jsonstring == NULL) {
        return NULL;
    }
    
    const char * linkkey = "\"url\":\"";
    const char * endquote = "\"";
    const char * separator = "\",\"";

    char * value;
    //malloc(value);

    char * start = strstr(jsonstring, linkkey);
    printf("\n\n\nEle dá print disto amigo (start): %s", start);


    start += strlen(linkkey);

    if (start != NULL) {
        // Check if the key is standalone (not part of another key)

        char * end = strstr(start, separator);

        if (end != NULL) {
            size_t length = end - start;

            printf("\n\nlength: %d", length);

            printf("\n\nend: %s", end);

            value = (char *)malloc(length * sizeof(char));

            // Copy the genre into the value array
            strncpy(value, start, length);
            value[length] = '\0';

            printf("\n\nvalue: %s", value);

            if (strlen(value) > length) {
                int i = strlen(value);
                char c;

                for (i; i > length; i--) {
                    value[i] == '\0';
                }

                printf("value: novo %s", value);
            }
            

            return value;
        }
        


        /*printf("\n\n\n\n\n\n\nchar do síituio: %c", start[wcslen(linkkey) - 1]);
        if (start[wcslen(linkkey) - 1] == ':' || start[wcslen(linkkey) - 1] == ' ') {
            
            if (wscanf(start + wcslen(linkkey), "\"%2048s\"", value) == 1) {
                printf("\n\n\n\n\nencontrou o coiso bom: %s", value);
                return value;
            } else {
                printf("\n\n\n\n\nnão encontrou nada doutor");
                printf("\n\n\n\n\nnão encontrou o coiso bom: %s", value);
                free(value);
                return NULL;
            }
                
            printf("\n\n\n\n\n\nstarto: %ls", start);
            printf("\n\nFound key: %ls\n", linkkey);
            
        } else {
            printf("\n\n\n\n\nnão verificou direitinho");
            free(value);
            return NULL;
        }*/
        

        
        
    } else {
        printf("\n\n\n\n\nnem o start é nulo");
        free(value);
        return NULL;
    }
}

int getQualitiesJson(wchar_t * jsonstring, stream streams[]) {
    const wchar_t * linkkey = L"\"url\":\"";
    const wchar_t * qualitykey = L"\"quality\":\"";

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
    const wchar_t * idkey = L"{\"id\":\"";
    const wchar_t * titlekey = L"\"title\":\"";
    const wchar_t * imagekey = L"\"image\":\"";
    const wchar_t * endquote = L"\"";

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
    //MessageBox(NULL, L"Chegou aqui ao show", L"Info", MB_ICONINFORMATION);

    const wchar_t * genreskey = L"\"genres\":[\"";
    const wchar_t * totalepkey = L"\"totalEpisodes\":";
    const wchar_t * imagekey = L"\"image\":\"";
    const wchar_t * datekey = L"\"releaseDate\":\"";
    const wchar_t * descriptionkey = L"\"description\":\"";
    const wchar_t * dubkey = L"\"subOrDub\":\"";
    const wchar_t * typekey = L"\"type\":\"";
    const wchar_t * statuskey = L"\"status\":\"";
    const wchar_t * endquote = L"\"";

    BOOL isdub;

    getGenres(jsonstring, genreskey, show.genres);
    getJsonId(jsonstring, totalepkey, show.episodes, 3);
    getJsonId(jsonstring, imagekey, show.imageurl, 3);
    getJsonId(jsonstring, datekey, show.date, 3);
    getJsonId(jsonstring, descriptionkey, show.description, 3);
    //getJsonId(jsonstring, dubkey, show.isdub, 3);
    //MessageBox(NULL, show.isdub, "Info", MB_ICONINFORMATION);
    getJsonId(jsonstring, typekey, show.type, 3);
    getJsonId(jsonstring, statuskey, show.status, 3); 
    

    show.isempty = FALSE;

    return show;
    
}

int convertWideToMulti(wchar_t * widestring, char * multistring) {

    ZeroMemory(multistring, strlen(multistring));

    printf("\n\nstrlen multistring: %d", strlen(multistring));
    printf("\n\nwcslen widestring: %d", wcslen(widestring));

    /*wchar_t a = L"a";
    int i;

    for (i = 0; 0 == wcscmp(a, L"\0"); i++) {
        a = widestring[i];
    }*/
    

    if (widestring != NULL) {
        wcstombs(multistring, widestring, wcslen(widestring));
        //WideCharToMultiByte(CP_UTF8, 0, widestring, -1, multistring, i, NULL, NULL);

        printf("\n\nold string: %ls\\", widestring);
        printf("\n\nnew string: %s\\", multistring);

        return 1;
    } else {
        printf("\nCould not convert!");
        return 0;
    }

    /*int bytesRequired = WideCharToMultiByte(CP_UTF8, 0, widestring, -1, NULL, 0, NULL, NULL);
    
    if (bytesRequired > 0 && bytesRequired <= sizeof(widestring)) {
        WideCharToMultiByte(CP_UTF8, 0, widestring, -1, multistring, sizeof(widestring), NULL, NULL);
    } else {
        printf("Error: Buffer size is too small or conversion failed.\n");
    }*/
    
}