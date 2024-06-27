#include "tools.h"
#include "connections.h"

//para este tipo de dados precisamos sempre de alocar memória
LPSTR requestedquery = NULL;
LPSTR requestedquery2 = NULL;

extern connections test;
extern trendinganimeinfo shows[12];

//função para gerir redirects
char * winHttpGetResponse(HINTERNET hRequest, HINTERNET hConnect, HINTERNET hSession) {
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = NULL;
    BOOL bResults = FALSE;

    char * responseString = NULL;
    size_t totalSize = 0;

    DWORD dwStatusCode = 0;
    DWORD dwSizeStatusCode = sizeof(dwStatusCode);

    //verifica o código de estado
    if (WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, NULL, &dwStatusCode, &dwSizeStatusCode, NULL)) {
        //caso seja um redirect permanente
        if (dwStatusCode == 308) {
            wchar_t szLocation[256];
            DWORD dwSizeLocation = sizeof(szLocation) / sizeof(szLocation[0]);

            //obtém o header 'Location'
            if (WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_LOCATION, NULL, szLocation, &dwSizeLocation, NULL)) {
                WinHttpCloseHandle(hRequest); 

                //cria um novo handle através de URL_COMPONENTS para a nova 'Location'
                URL_COMPONENTS urlComp;
                memset(&urlComp, 0, sizeof(urlComp));
                urlComp.dwStructSize = sizeof(urlComp);

                wchar_t szHostName[256];
                wchar_t szUrlPath[256];
                urlComp.lpszHostName = szHostName;
                urlComp.dwHostNameLength = sizeof(szHostName) / sizeof(szHostName[0]);
                urlComp.lpszUrlPath = szUrlPath;
                urlComp.dwUrlPathLength = sizeof(szUrlPath) / sizeof(szUrlPath[0]);

                // acrescenta o URL aos componentes
                WinHttpCrackUrl(szLocation, 0, 0, &urlComp);

                hConnect = WinHttpConnect(hSession, urlComp.lpszHostName, urlComp.nPort, 0);
                hRequest = WinHttpOpenRequest(hConnect, L"GET", urlComp.lpszUrlPath, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

                if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
                    if (WinHttpReceiveResponse(hRequest, NULL)) {
                        return winHttpGetResponse(hRequest, hConnect, hSession);
                    }
                }

                WinHttpCloseHandle(hRequest);
                WinHttpCloseHandle(hConnect);

                return NULL;
            }
        } else if (dwStatusCode == 200) {
            // se o pedido for bem-sucedido, lê os dados
            do {
                dwSize = 0;
                if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                    printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
                    break;
                }

                if (dwSize > 0) {

                    pszOutBuffer = (LPSTR)malloc(dwSize + 1);
                    if (!pszOutBuffer) {
                        printf("Out of memory\n");
                        break;
                    }

                    ZeroMemory(pszOutBuffer, dwSize + 1);

                    if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                        printf("Error %u in WinHttpReadData.\n", GetLastError());
                        free(pszOutBuffer);
                        break;
                    }

                    pszOutBuffer[dwDownloaded] = '\0';

                    //faz realloc da variável para acrescentar novos dados
                    responseString = (char*)realloc(responseString, totalSize + dwDownloaded + 1);
                    if (!responseString) {
                        printf("Out of memory\n");
                        free(pszOutBuffer);
                        break;
                    }

                    //
                    memcpy(responseString + totalSize, pszOutBuffer, dwDownloaded + 1);
                    totalSize += dwDownloaded;

                    free(pszOutBuffer);
                }
            } while (dwSize > 0);

            bResults = TRUE;
        }
    } else {
        printf("Error %u in WinHttpQueryHeaders.\n", GetLastError());
    }

    return responseString;
}


//junta duas strings numa única
void parseRequestText(char * uri, char * query) {
    char buffer[256]; 
    sprintf(buffer, "%s%s", uri, query);
    
    if (requestedquery != NULL) {
        free(requestedquery);
        requestedquery = NULL;
    }
    
    //alocamos memória para este long pointer constant wide string, fazendo o tamanho do buffer vezes o tamanho de um wide char
    requestedquery = (LPSTR)malloc((strlen(buffer) + 1) * sizeof(wchar_t));
    
    mbstowcs((wchar_t*) requestedquery, buffer, strlen(buffer) + 1);
}

void parseRequestText2(char * uri, char * query) {
    char buffer[256]; 
    sprintf(buffer, "%s%s", uri, query);
    
    if (requestedquery2 != NULL) {
        free(requestedquery2);
        requestedquery2 = NULL;
    }
    
    //alocamos memória para este long pointer constant wide string, fazendo o tamanho do buffer vezes o tamanho de um wide char
    requestedquery2 = (LPCWSTR)malloc((strlen(buffer) + 1) * sizeof(wchar_t));
    
    mbstowcs((wchar_t*) requestedquery2, buffer, strlen(buffer) + 1);
}

/**********************************************************/

int searchConnection(HWND hwnd, char * query, result results[]) {

    parseRequestText("/anime/zoro/", query);

    //Cria um handle de sessão de hinternet
    HINTERNET hsession = WinHttpOpen(L"Dango/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hsession == NULL) {
        MessageBox(hwnd, "WinHttpOpen failed!", "Error", MB_ICONERROR);
        return 0;
    }

    //Atribuir protocolos à conexão
    DWORD dwprotocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
    WinHttpSetOption(hsession, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwprotocols, sizeof(dwprotocols));

    //Estabelece contacto com o domínio especificado
    HINTERNET hconnect = WinHttpConnect(hsession, L"consumet-one-sigma.vercel.app", INTERNET_DEFAULT_PORT, 0);
    if (hconnect == NULL) {
        MessageBox(hwnd, "WinHttpConnect failed!", "Error", MB_ICONERROR);
        return 0;
    }

    //Cria um pedido HTTPS
    HINTERNET hrequest = WinHttpOpenRequest(hconnect, L"GET", requestedquery, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (hrequest == NULL) {
        printf("%d", GetLastError());
        MessageBox(hwnd, "error", "Error", MB_ICONERROR);
        WinHttpCloseHandle(hsession);
        GetLastError();
        return 0;
    }

    char * jsonstring;

    if (WinHttpSendRequest(hrequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        if (WinHttpReceiveResponse(hrequest, NULL)) {
            jsonstring = winHttpGetResponse(hrequest, hconnect, hsession);
            return parseResultsJson(jsonstring, results);
        }
    } else
        return 0;
}

int episodesConnection(HWND hwnd, char * resultid, episode episodes[]) {

    parseRequestText2("/anime/zoro/info?id=", resultid);

    HINTERNET hsession2 = WinHttpOpen(L"Dango/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hsession2 == NULL) {
        MessageBox(hwnd, "WinHttpOpen failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return 0;
    }

    DWORD dwprotocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
    WinHttpSetOption(hsession2, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwprotocols, sizeof(dwprotocols));
    
    HINTERNET hconnect2 = WinHttpConnect(hsession2, L"consumet-one-sigma.vercel.app", INTERNET_DEFAULT_PORT, 0);
    if (hconnect2 == NULL) {
        MessageBox(hwnd, "WinHttpConnect failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return 0;
    }

    HINTERNET hrequest2 = WinHttpOpenRequest(hconnect2, L"GET", requestedquery2, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (hrequest2 == NULL) {
        MessageBox(hwnd, "WinHttpOpenRequest failed!", "Error", MB_ICONERROR);
        wprintf(L"\n %s | %lu", requestedquery2, GetLastError());
        WinHttpCloseHandle(hconnect2);
        WinHttpCloseHandle(hsession2);
        return 0;
    }

    char * jsonstring;
    
    if (WinHttpSendRequest(hrequest2, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        if (WinHttpReceiveResponse(hrequest2, NULL)) {
            jsonstring = winHttpGetResponse(hrequest2, hconnect2, hsession2);
            return parseEpisodesJson(hwnd, resultid, jsonstring, episodes);
        }
    } else
        return 0;
}

int epnumConnection(HWND hwnd, char * resultid) {

    parseRequestText("/anime/zoro/info?id=", resultid);

    HINTERNET hsession = WinHttpOpen(L"Dango/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hsession == NULL) {
        MessageBox(hwnd, "WinHttpOpen failed!", "Error", MB_ICONERROR);
        return 0;
    }

    DWORD dwprotocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
    WinHttpSetOption(hsession, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwprotocols, sizeof(dwprotocols));

    //Estabelece contacto com o domínio especificado
    HINTERNET hconnect = WinHttpConnect(hsession, L"consumet-one-sigma.vercel.app", INTERNET_DEFAULT_PORT, 0);
    if (hconnect == NULL) {
        MessageBox(hwnd, "WinHttpConnect failed!", "Error", MB_ICONERROR);
        return 0;
    }

    //Cria um pedido HTTP
    HINTERNET hrequest = WinHttpOpenRequest(hconnect, L"GET", requestedquery, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (hrequest == NULL) {
        printf("%d", GetLastError());
        MessageBox(hwnd, "error", "Error", MB_ICONERROR);
        WinHttpCloseHandle(hsession);
        GetLastError();
        return 0;
    }
    
    char * jsonstring = NULL;
    const char* epnumkey = "\"totalEpisodes\":";

    if (WinHttpSendRequest(hrequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        if (WinHttpReceiveResponse(hrequest, NULL)) {
            jsonstring = winHttpGetResponse(hrequest, hconnect, hsession);
            return getEpisodesNum(jsonstring, epnumkey);
        }
    } else
        return 0;
}

char * eplinkConnection(HWND hwnd, char * epid) {
    parseRequestText2("/anime/zoro/watch?episodeId=", epid);

    HINTERNET hsession2 = WinHttpOpen(L"Dango/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hsession2 == NULL) {
        MessageBox(hwnd, "WinHttpOpen failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return 0;
    }

    DWORD dwprotocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
    WinHttpSetOption(hsession2, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwprotocols, sizeof(dwprotocols));
    
    HINTERNET hconnect2 = WinHttpConnect(hsession2, L"consumet-one-sigma.vercel.app", INTERNET_DEFAULT_PORT, 0);
    if (hconnect2 == NULL) {
        MessageBox(hwnd, "WinHttpConnect failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return 0;
    }

    HINTERNET hrequest2 = WinHttpOpenRequest(hconnect2, L"GET", requestedquery2, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (hrequest2 == NULL) {
        MessageBox(hwnd, "WinHttpOpenRequest failed!", "Error", MB_ICONERROR);
        wprintf(L"\n %s | %lu", requestedquery2, GetLastError());
        WinHttpCloseHandle(hconnect2);
        WinHttpCloseHandle(hsession2);
        return 0;
    }

    char * jsonstring;

    if (WinHttpSendRequest(hrequest2, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        if (WinHttpReceiveResponse(hrequest2, NULL)) {
            jsonstring = winHttpGetResponse(hrequest2, hconnect2, hsession2);
            return getLinkJson(hwnd, jsonstring);
        }   
    } else
        return 0;
}

int getinfoConnection(HWND hwnd, trendinganimeinfo shows[]) {
    HINTERNET hsession2 = WinHttpOpen(L"Dango/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hsession2 == NULL) {
        MessageBox(hwnd, "WinHttpOpen failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return 0;
    }

    DWORD dwprotocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
    WinHttpSetOption(hsession2, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwprotocols, sizeof(dwprotocols));
    
    HINTERNET hconnect2 = WinHttpConnect(hsession2, L"consumet-one-sigma.vercel.app", INTERNET_DEFAULT_PORT, 0);
    if (hconnect2 == NULL) {
        MessageBox(hwnd, "WinHttpConnect failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return 0;
    }

    HINTERNET hrequest2 = WinHttpOpenRequest(hconnect2, L"GET", L"/anime/gogoanime/popular", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (hrequest2 == NULL) {
        MessageBox(hwnd, "WinHttpOpenRequest failed!", "Error", MB_ICONERROR);
        wprintf(L"\n %s | %lu", requestedquery2, GetLastError());
        WinHttpCloseHandle(hconnect2);
        WinHttpCloseHandle(hsession2);
        return 0;
    }

    char * jsonstring;

    if (WinHttpSendRequest(hrequest2, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        if (WinHttpReceiveResponse(hrequest2, NULL)) {
            jsonstring = winHttpGetResponse(hrequest2, hconnect2, hsession2);
            return getTrendingInfo(jsonstring, shows);
        }   
    } else
        return 0;
}