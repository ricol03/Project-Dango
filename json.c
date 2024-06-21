#include "tools.h"

//criação da união para albergar um de dois tipos de dados
connections test;

char * getJsonId(char * jsonstring, const char * key, TCHAR * value, int check) {
    const char * endquote = "\"";
    const char * resseparator = "},{";

    if (jsonstring == NULL) {
        MessageBox(NULL, "Error with JSON string", "Error", MB_ICONERROR | MB_OK);
        //return test;
        return NULL;
    }
    
    //printf("%s", jsonstring);

    //printf("\n\n cheque %d", check);

    if (check == 1) {
        char * check = strstr(jsonstring, resseparator);
        if (check == NULL) {
            return jsonstring;
        } else {
            check += strlen(resseparator);
            return check;
        }
    }

    //printf("\n\nantes do cheque: \n%s", jsonstring);
    //printf("\n\n cheque %d", check);
    //"verifica" se a string imagekey existe na jsonstring

    //ele queima-se todo aqui man, vê lá isso
    //printf("%s", jsonstring);
    char * start = strstr(jsonstring, key);
    //printf("\n\n cheque %d", check);
    printf("\n\n\nEle dá print disto amigo (start): %s", start);

    if (check == 2) {
        //printf("valor do start: %s", start);
        //printf("valor do json: %s", jsonstring);
    }
    
    //se existir
    if (start != NULL) {
        //move o ponteiro para a posição após a string imagekey
        start += strlen(key);

        //printf("\n\npasso 2");

        char * end = strstr(start, endquote);
        printf("\n\n\nEle dá print disto amigo (end): %s", end);

        if (end != NULL) {
            //printf("\n\npasso 3");
            //calcula o tamanho entre o início e o fim do link da imagem
            size_t length = end - start;

            //coloca na struct a string da posição start até start + length
            strncpy(value, start, length);
            //termina a string com um \0
            
            if (check == 3) {
                return NULL;
            } else {
                value[length] = '\0';
                /*test.value = start;
                return test;    */
                return value;
            }
            
            
        }
    }
}

int getEpisodesNum(char * jsonstring, const char * key) {

    //printf("\n\n\nele chegou ao getepisodesnum");
    //printf("\ncom isto: %s", jsonstring);

    const char* start = strstr(jsonstring, key);
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

int parseResultsJson(char * jsonstring, result results[]) {

    //cria uma string do início da chave de cada atributo json
    const char * idkey = "\"id\":\"";
    const char * titlekey = "\"title\":\"";
    const char * ratingkey = "\"rating\":\"";
    const char * reldatekey = "\"releaseDate\":\"";
    const char * typekey = "\"type\":\"";

    //printf("\n\n\nchegou ao parseresults");

    //printf("\n\n\n\nprint aí man: %s", jsonstring);

    for (int i = 0; i < 8; i++) {
        getJsonId(jsonstring, idkey, results[i].id, 0);
        //printf("\n\nid do resultado: %s", results[i].id);
        getJsonId(jsonstring, titlekey, results[i].title, 0);
        //printf("\n\ntítulo do resultado: %s", results[i].title);
        getJsonId(jsonstring, ratingkey, results[i].rating, 0);
        //printf("\n\nrating do resultado: %s", results[i].rating);
        getJsonId(jsonstring, reldatekey, results[i].releasedate, 0);
        //printf("\n\ndata de lançamento do resultado: %s", results[i].releasedate);
        
        //anteriormente test (union)
        jsonstring = getJsonId(jsonstring, typekey, results[i].type, 1);
        //printf("\n\ntipo do resultado: %s", results[i].type);
        //jsonstring = test.value;

        if (jsonstring == NULL)
            return i;
    }

    return 8;        
}

int parseEpisodesJson(HWND hwnd, char * resultid, char * jsonstring, episode episodes[]) {

    int number = epnumConnection(hwnd, resultid);

    printf("que número é este meu senhor? %d", number);
    const char * totalepisodeskey = "\"totalEpisodes\":";
    const char * episodekey = "\"episodes\":";

    printf("\n\n\n vê lá a string no peej: %s", jsonstring);

    char * newstring = strstr(jsonstring, totalepisodeskey);
    char * newstring2 = strstr(newstring, episodekey);

    //cria uma string do início da chave de cada atributo json
    const char * idkey = "\"id\":\"";
    const char * numberkey = "\"number\":\"";
    const char * titlekey = "\"title\":\"";

    for (int i = 0; i < number; i++) {
        getJsonId(newstring2, idkey, episodes[i].id, 0);
        getJsonId(newstring2, numberkey, episodes[i].number, 0);
        
        //anteriormente test (union)
        newstring2 = getJsonId(newstring2, titlekey, episodes[i].title, 1);
        //newstring2 = test.value;

        if (newstring2 == NULL)
            return i;
    }

    return number;
}

char * getLinkJson(HWND hwnd, char * jsonstring) {
    
    const char * linkkey = "\"url\":";
    const char * endquote = "\"";

    char * value = malloc(512);
    //malloc(value);

    char * start = strstr(jsonstring, linkkey);
    printf("\n\n\nEle dá print disto amigo (start): %s", start);

    if (start != NULL) {
        // Check if the key is standalone (not part of another key)

        printf("\n\n\n\n\n\n\nchar do síituio: %c", start[strlen(linkkey) - 1]);
        if (start[strlen(linkkey) - 1] == ':' || start[strlen(linkkey) - 1] == ' ') {
            
            if (sscanf(start + strlen(linkkey), "\"%343s\"", value) == 1) {
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

int getTrendingInfo(char * jsonstring, trendinganimeinfo results[]) {
    const char * idkey = "{\"id\":\"";
    const char * titlekey = "\"title\":\"";
    const char * imagekey = "\"image\":\"";
    const char * endquote = "\"";

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
