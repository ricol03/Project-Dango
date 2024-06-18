#include "tools.h"

//criação da união para albergar um de dois tipos de dados
connections test;

connections getjsonid(char* jsonstring, const char* key, TCHAR* value, int check) {
    const char* endquote = "\"";
    const char* resseparator = "},{";

    if (jsonstring == NULL || key == NULL) {
        MessageBox(NULL, jsonstring, "Erro", MB_ICONERROR | MB_OK);
        return test;
    }

    if (check == 1) {
        char * check = strstr(jsonstring, resseparator);
        if (check == NULL) {
            jsonstring = NULL;
            test.value = jsonstring;
            return test;
        } 
    } 

    char * start = strstr(jsonstring, key);

    //se existir
    if (start != NULL) {
        //move o ponteiro para a posição após a string key
        start += strlen(key);

        char * end = strstr(start, endquote);

        if (end != NULL) {
            //calcula o tamanho entre o início e o fim do link da key
            size_t length = end - start;

            //coloca na struct a string da posição start até start + length
            strncpy(value, start, length);
            //termina a string com um \0
            value[length] = '\0';
            test.value = start;

            return test;    
        }
    }
}

int getepisodesnum(char* jsonstring, const char* key) {

    const char* start = strstr(jsonstring, key);

    if (start != NULL) {
        //Verifica se a chave é única e não é parte de outra chave
        if (start[strlen(key) - 1] == ':' || start[strlen(key) - 1] == ' ') {
            int value;
            if (sscanf(start + strlen(key), "%d", &value) == 1) {
                return value;
            } else {
                return 0;
            }
        }
    }

    return 0;
}

int parseresultsjson(char* jsonstring, result results[]) {

    //cria uma string do início da chave de cada atributo json
    const char* idkey = "\"id\":\"";
    const char* titlekey = "\"title\":\"";
    const char* ratingkey = "\"rating\":\"";
    const char* reldatekey = "\"releaseDate\":\"";
    const char* typekey = "\"type\":\"";

    for (int i = 0; i < 8; i++) {
        getjsonid(jsonstring, idkey, results[i].id, 0);
        getjsonid(jsonstring, titlekey, results[i].title, 0);
        getjsonid(jsonstring, ratingkey, results[i].rating, 0);
        getjsonid(jsonstring, reldatekey, results[i].releasedate, 0);
        test = getjsonid(jsonstring, typekey, results[i].type, 1);
        jsonstring = test.value;

        if (jsonstring == NULL)
            return i;
    }

    //caso jsonstring não seja nulo após o for, sabemos que a pesquisa originou mais de 8 resultados
    return 8;        
}

int parseepisodesjson(HWND hwnd, char * resultid, char* jsonstring, episode episodes[]) {

    int number = epnumConnection(hwnd, resultid);

    const char* totalepisodeskey = "\"totalEpisodes\":";
    const char* episodekey = "\"episodes\":";

    char * newstring = strstr(jsonstring, totalepisodeskey);
    char * newstring2 = strstr(newstring, episodekey);

    //cria uma string do início da chave de cada atributo json
    const char* idkey = "\"id\":\"";
    const char* numberkey = "\"number\":\"";
    const char* titlekey = "\"title\":\"";

    for (int i = 0; i < number; i++) {
        getjsonid(newstring2, idkey, episodes[i].id, 0);
        getjsonid(newstring2, numberkey, episodes[i].number, 0);
        test = getjsonid(newstring2, titlekey, episodes[i].title, 0);
        newstring2 = test.value;

        if (newstring2 == NULL)
            return i;
    }

    return number;
}

char * getlinkjson(HWND hwnd, char * jsonstring) {
    
    const char * linkkey = "\"url\":\"";
    const char * endquote = "\"";
    
    char * start = strstr(jsonstring, linkkey);
    if (start == NULL) {
        return NULL;
    }
    
    start += strlen(linkkey);
    char * end = strstr(start, endquote);
    if (end == NULL) {
        return NULL;
    }
    
    size_t len = end - start;
    char * value = malloc(len + 1);
    if (value == NULL) {
        MessageBox(hwnd, "Error while allocating memory!", "Error", MB_OK | MB_ICONERROR);
        return NULL;
    }
    
    strncpy(value, start, len);
    value[len] = '\0';
    
    return value;
}