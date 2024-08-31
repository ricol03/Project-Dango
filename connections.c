#include "tools.h"
#include "connections.h"

extern wchar_t strmatrix[MAXCONNECTIONS][64];

extern wchar_t provider[32];
extern wchar_t server[32];
extern wchar_t port[6];

//para este tipo de dados precisamos sempre de alocar memória
LPSTR requestedquery = NULL;
LPSTR requestedquery2 = NULL;

size_t stringsize;

extern connections test;
extern trendinganimeinfo shows[12];

//função para gerir redirects
char * winHttpGetResponse(HINTERNET hRequest, HINTERNET hConnect, HINTERNET hSession, char * responseStringAux) {

    printf("\n%ls", server);
    printf("\n%ls", port);
    printf("\n%ls", provider);

    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = NULL;
    BOOL bResults = FALSE;

    size_t totalSize = 0;

    DWORD dwStatusCode = 0;
    DWORD dwSizeStatusCode = sizeof(dwStatusCode);

    // Verify the status code
    if (WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, NULL, &dwStatusCode, &dwSizeStatusCode, NULL)) {
        
        printf("\n\nstatus code: %lu", dwStatusCode);
        printf("\n\nserver: %ls", server);
        
        if (dwStatusCode == 308) {
            wchar_t szLocation[256];
            DWORD dwSizeLocation = sizeof(szLocation) / sizeof(szLocation[0]);

            // Obtain the 'Location' header
            if (WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_LOCATION, NULL, szLocation, &dwSizeLocation, NULL)) {
                WinHttpCloseHandle(hRequest);

                URL_COMPONENTS urlComp;
                memset(&urlComp, 0, sizeof(urlComp));
                urlComp.dwStructSize = sizeof(urlComp);

                wchar_t szHostName[256];
                wchar_t szUrlPath[256];
                urlComp.lpszHostName = szHostName;
                urlComp.dwHostNameLength = sizeof(szHostName) / sizeof(szHostName[0]);
                urlComp.lpszUrlPath = szUrlPath;
                urlComp.dwUrlPathLength = sizeof(szUrlPath) / sizeof(szUrlPath[0]);

                // Crack the URL into components
                WinHttpCrackUrl(szLocation, 0, 0, &urlComp);

                hConnect = WinHttpConnect(hSession, urlComp.lpszHostName, urlComp.nPort, 0);
                hRequest = WinHttpOpenRequest(hConnect, L"GET", urlComp.lpszUrlPath, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

                if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
                    if (WinHttpReceiveResponse(hRequest, NULL)) {
                        return winHttpGetResponse(hRequest, hConnect, hSession, responseStringAux);
                    }
                }

                WinHttpCloseHandle(hRequest);
                WinHttpCloseHandle(hConnect);

                return NULL;
            } else {
                MessageBox(NULL, L"No location header was found", L"Error", MB_ICONERROR);
            }
        } else if (dwStatusCode == 200) {
            // Read the data if the request is successful
            do {
                dwSize = 0;
                if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                    printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
                    break;
                }

                if (dwSize > 0) {
                    // Allocate memory for the buffer, accounting for wchar_t
                    /*pszOutBuffer = (LPWSTR)malloc((dwSize + sizeof(wchar_t)));
                    if (!pszOutBuffer) {
                        printf("Out of memory\n");
                        break;
                    }

                    ZeroMemory(pszOutBuffer, dwSize + sizeof(wchar_t));

                    if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                        printf("Error %u in WinHttpReadData.\n", GetLastError());
                        free(pszOutBuffer);
                        break;
                    }

                    // Ensure the string is null-terminated properly
                    pszOutBuffer[dwDownloaded / sizeof(wchar_t)] = L'\0';

                    // Reallocate responseString to accommodate the new data
                    wchar_t* tempBuffer = (wchar_t*)realloc(responseString, (totalSize + (dwDownloaded / sizeof(wchar_t)) + 1) * sizeof(wchar_t));
                    if (!tempBuffer) {
                        printf("Out of memory\n");
                        free(pszOutBuffer);
                        break;
                    }

                    responseString = tempBuffer;

                    // Copy the data into the response string
                    memcpy(responseString + totalSize, pszOutBuffer, dwDownloaded);
                    totalSize += dwDownloaded / sizeof(wchar_t);
                    responseString[totalSize] = L'\0';

                    free(pszOutBuffer);*/

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
                    responseStringAux = (char*)realloc(responseStringAux, totalSize + dwDownloaded + 1);
                    if (!responseStringAux) {
                        printf("Out of memory\n");
                        free(pszOutBuffer);
                        break;
                    }

                    //
                    memcpy(responseStringAux + totalSize, pszOutBuffer, dwDownloaded + 1);
                    totalSize += dwDownloaded;

                    free(pszOutBuffer);

                }
            } while (dwSize > 0);

            bResults = TRUE;
        } else {
            
            printf("\n\nerror code: %lu", dwStatusCode);
            printf("\n\nserver: %ls", server);
            MessageBox(NULL, L"Unhandled HTTP code", L"Error", MB_ICONERROR);
        }
    } else {
        printf("Error %u in WinHttpQueryHeaders.\n", GetLastError());
    }

    return responseStringAux;
}

wchar_t * winHttpGetResponseBin(HINTERNET hRequest, HINTERNET hConnect, HINTERNET hSession, wchar_t * filepath) {

    printf("\n%ls", server);
    printf("\n%ls", port);
    printf("\n%ls", provider);

    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = NULL;
    BOOL bResults = FALSE;

    char * responseStringAux = NULL;
    wchar_t * responseString = NULL;
    size_t totalSize = 0;

    DWORD dwStatusCode = 0;
    DWORD dwSizeStatusCode = sizeof(dwStatusCode);

    // Verify the status code
    if (WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, NULL, &dwStatusCode, &dwSizeStatusCode, NULL)) {
        
        printf("\n\nstatus code: %lu", dwStatusCode);
        printf("\n\nserver: %ls", server);
        
        if (dwStatusCode == 308) {
            wchar_t szLocation[256];
            DWORD dwSizeLocation = sizeof(szLocation) / sizeof(szLocation[0]);

            // Obtain the 'Location' header
            if (WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_LOCATION, NULL, szLocation, &dwSizeLocation, NULL)) {
                WinHttpCloseHandle(hRequest);

                URL_COMPONENTS urlComp;
                memset(&urlComp, 0, sizeof(urlComp));
                urlComp.dwStructSize = sizeof(urlComp);

                wchar_t szHostName[256];
                wchar_t szUrlPath[256];
                urlComp.lpszHostName = szHostName;
                urlComp.dwHostNameLength = sizeof(szHostName) / sizeof(szHostName[0]);
                urlComp.lpszUrlPath = szUrlPath;
                urlComp.dwUrlPathLength = sizeof(szUrlPath) / sizeof(szUrlPath[0]);

                // Crack the URL into components
                WinHttpCrackUrl(szLocation, 0, 0, &urlComp);

                hConnect = WinHttpConnect(hSession, urlComp.lpszHostName, urlComp.nPort, 0);
                hRequest = WinHttpOpenRequest(hConnect, L"GET", urlComp.lpszUrlPath, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

                if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
                    if (WinHttpReceiveResponse(hRequest, NULL)) {
                        return winHttpGetResponseBin(hRequest, hConnect, hSession, filepath);
                    }
                }

                WinHttpCloseHandle(hRequest);
                WinHttpCloseHandle(hConnect);

                return NULL;
            } else {
                MessageBox(NULL, L"No location header was found", L"Error", MB_ICONERROR);
            }
        } else if (dwStatusCode == 200) {
            // Read the data if the request is successful

            HANDLE hf = CreateFile(filepath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hf == INVALID_HANDLE_VALUE) {
                MessageBox(NULL, L"The file could not be created", L"Error", MB_ICONERROR);
                //free(filepath);
                return NULL;
            }

            do {
                dwSize = 0;
                if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                    printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
                    break;
                }

                if (dwSize > 0) {
                    pszOutBuffer = (LPSTR)malloc(dwSize);
                    if (!pszOutBuffer) {
                        printf("Out of memory\n");
                        break;
                    }

                    ZeroMemory(pszOutBuffer, dwSize);

                    if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                        printf("Error %u in WinHttpReadData.\n", GetLastError());
                        free(pszOutBuffer);
                        break;
                    } else {
                        

                        // Escreve os dados da imagem no arquivo
                        DWORD byteswritten = 0;
                        BOOL result = WriteFile(hf, pszOutBuffer, dwSize, &byteswritten, NULL);
                        /*if (!result || byteswritten != dwSize) {
                            MessageBox(NULL, L"Failed to write the image data to the file", L"Error", MB_ICONERROR);
                            CloseHandle(hf);
                            //free(filepath);
                            return NULL;
                        }*/
                    }

                    //faz realloc da variável para acrescentar novos dados
                    /*responseStringAux = (char*)realloc(responseStringAux, totalSize + dwDownloaded + 1);
                    if (!responseStringAux) {
                        printf("Out of memory\n");
                        free(pszOutBuffer);
                        break;
                    }

                    //
                    memcpy(responseStringAux + totalSize, pszOutBuffer, dwDownloaded + 1);
                    totalSize += dwDownloaded;*/

                    free(pszOutBuffer);

                }
            } while (dwSize > 0);

            bResults = TRUE;
            CloseHandle(hf);
        } else {
            
            printf("\n\nerror code: %lu", dwStatusCode);
            printf("\n\nserver: %ls", server);
            MessageBox(NULL, L"Unhandled HTTP code", L"Error", MB_ICONERROR);
        }
    } else {
        printf("Error %u in WinHttpQueryHeaders.\n", GetLastError());
    }


    /*printf("\nchar string: %s", responseStringAux);

    //MessageBoxW(NULL, responseStringAux, L"Warning", MB_ICONWARNING);

    //printf("\n\n\nsize of responsestringaux: %d", strlen(responseStringAux));

    int size = MultiByteToWideChar(CP_ACP, 0, responseStringAux, -1, NULL, 0);

    stringsize = size * sizeof(wchar_t);
    responseString = (wchar_t *)malloc(stringsize);

    //mbstowcs((wchar_t*) responseString, responseStringAux, strlen(responseStringAux) + 1);

    MultiByteToWideChar(CP_ACP, 0, responseStringAux, -1, responseString, size);

    //printf("\n\n\n responsestring: \n\n %ls", responseString);*/

    return filepath;
}

char * getCharResponseRaw(HINTERNET hrequest, HINTERNET hconnect, HINTERNET hsession) {
    char * responseStringAux = NULL;
    return winHttpGetResponse(hrequest, hconnect, hsession, responseStringAux);
}

wchar_t * getWideResponseRaw(HINTERNET hrequest, HINTERNET hconnect, HINTERNET hsession) {

    char * responseStringAux = NULL;
    wchar_t * responseString = NULL;

    responseStringAux = winHttpGetResponse(hrequest, hconnect, hsession, responseStringAux);

    int size = MultiByteToWideChar(CP_ACP, 0, responseStringAux, -1, NULL, 0);

    stringsize = size * sizeof(wchar_t);
    responseString = (wchar_t *)malloc(stringsize);

    //mbstowcs((wchar_t*) responseString, responseStringAux, strlen(responseStringAux) + 1);

    MultiByteToWideChar(CP_ACP, 0, responseStringAux, -1, responseString, size);

    return responseString;
}

//joins two strings into one (char only)
void parseRequestText(wchar_t * uri, wchar_t * query) {
    wchar_t buffer[256]; 
    wcscpy(buffer, uri);
    wcscat(buffer, query);

    printf("%ls", buffer);

    //MessageBox(NULL, buffer, L"Info", MB_ICONINFORMATION);
    
    if (requestedquery != NULL) {
        free(requestedquery);
        requestedquery = NULL;
    }
    
    //alocamos memória para este long pointer constant wide string, fazendo o tamanho do buffer vezes o tamanho de um wide char
    requestedquery = (LPSTR)malloc((wcslen(buffer) + 1) * sizeof(wchar_t));
    
    wcscpy(requestedquery, buffer);

    //MessageBox(NULL, requestedquery, L"Warning", MB_ICONWARNING);

    //mbstowcs((wchar_t*) requestedquery, buffer, strlen(buffer) + 1);
}

void parseRequestText2(wchar_t * uri, wchar_t * query) {
    wchar_t buffer[256]; 
    wcscpy(buffer, uri);
    wcscat(buffer, query);

    printf("%ls", buffer);

    //MessageBox(NULL, buffer, L"Info", MB_ICONINFORMATION);
    
    if (requestedquery2 != NULL) {
        free(requestedquery2);
        requestedquery2 = NULL;
    }
    
    //alocamos memória para este long pointer constant wide string, fazendo o tamanho do buffer vezes o tamanho de um wide char
    requestedquery2 = (LPSTR)malloc((wcslen(buffer) + 1) * sizeof(wchar_t));
    
    wcscpy(requestedquery2, buffer);

    //MessageBox(NULL, requestedquery2, L"Warning", MB_ICONWARNING);

    //mbstowcs((wchar_t*) requestedquery, buffer, strlen(buffer) + 1);
}

LPWSTR serverAddressInitializer(wchar_t * server) {
    LPWSTR servernew = NULL;

    servernew = (LPWSTR)malloc((wcslen(server) + 1) * sizeof(wchar_t));

    //mbstowcs((wchar_t*) servernew, server, strlen(server) + 1);

    wcscpy(servernew, server);

    //MultiByteToWideChar(CP_ACP, 0, server, -1, servernew, strlen(server)+1);

    return servernew;
}

/**********************************************************/

int searchConnection(HWND hwnd, wchar_t * query, result results[]) {

    selectStringMatrix();
    parseRequestText(strmatrix[0], query);

    //Cria um handle de sessão de hinternet
    HINTERNET hsession = WinHttpOpen("Dango/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hsession == NULL) {
        MessageBox(hwnd, L"WinHttpOpen failed!", L"Error", MB_ICONERROR);
        printf("\n\nGetLastError: %lu", GetLastError());
        return 0;
    }

    //Atribuir protocolos à conexão
    DWORD dwprotocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
    WinHttpSetOption(hsession, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwprotocols, sizeof(dwprotocols));

    LPWSTR servernew = serverAddressInitializer(server);

    //Estabelece contacto com o domínio especificado
    HINTERNET hconnect = WinHttpConnect(hsession, servernew, (WORD)_wtoi(port), 0);
    if (hconnect == NULL) {
        MessageBox(hwnd, L"WinHttpConnect failed!", L"Error", MB_ICONERROR);
        MessageBox(hwnd, servernew, "Error", MB_ICONERROR);
        MessageBox(hwnd, requestedquery, "INFO", MB_ICONINFORMATION);
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

    wchar_t * jsonstring = NULL;

    if (WinHttpSendRequest(hrequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        if (WinHttpReceiveResponse(hrequest, NULL)) {
            jsonstring = getWideResponseRaw(hrequest, hconnect, hsession);
            return parseResultsJson(jsonstring, results);
        } else {
            MessageBox(hwnd, L"errorinner", L"Error", MB_ICONERROR);
        }
    } else {
        printf("Erro: %lu", GetLastError());
        MessageBox(hwnd, L"Couldn't send the request correctly", L"Error", MB_ICONERROR);
        return 0;
    }
}

int episodesConnection(HWND hwnd, wchar_t * resultid, episode episodes[]) {

    //MessageBox(NULL, L"episodesConnection", L"Info", MB_ICONINFORMATION);

    parseRequestText2(strmatrix[1], resultid);

    HINTERNET hsession2 = WinHttpOpen(L"Dango/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hsession2 == NULL) {
        MessageBox(hwnd, "WinHttpOpen failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return 0;
    }

    DWORD dwprotocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
    WinHttpSetOption(hsession2, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwprotocols, sizeof(dwprotocols));

    LPWSTR servernew = serverAddressInitializer(server);
    
    HINTERNET hconnect2 = WinHttpConnect(hsession2, servernew, (WORD)atoi(port), 0);
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

    wchar_t * jsonstring;
    
    if (WinHttpSendRequest(hrequest2, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        if (WinHttpReceiveResponse(hrequest2, NULL)) {
            jsonstring = getWideResponseRaw(hrequest2, hconnect2, hsession2);
            return parseEpisodesJson(hwnd, resultid, jsonstring, episodes);
        }
    } else
        return 0;
}

int epnumConnection(HWND hwnd, wchar_t * resultid) {

    //MessageBox(NULL, L"epnumConnection", L"Info", MB_ICONINFORMATION);

    parseRequestText(strmatrix[1], resultid);

    HINTERNET hsession = WinHttpOpen(L"Dango/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hsession == NULL) {
        MessageBox(hwnd, "WinHttpOpen failed!", "Error", MB_ICONERROR);
        return 0;
    }

    DWORD dwprotocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
    WinHttpSetOption(hsession, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwprotocols, sizeof(dwprotocols));

    LPWSTR servernew = serverAddressInitializer(server);

    //Estabelece contacto com o domínio especificado
    HINTERNET hconnect = WinHttpConnect(hsession, servernew, (WORD)atoi(port), 0);
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
    
    wchar_t * jsonstring = NULL;
    const wchar_t * epnumkey = L"\"totalEpisodes\":";

    if (WinHttpSendRequest(hrequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        if (WinHttpReceiveResponse(hrequest, NULL)) {
            jsonstring = getWideResponseRaw(hrequest, hconnect, hsession);
            return getEpisodesNum(jsonstring, epnumkey);
        }
    } else
        return 0;
}

char * eplinkConnection(HWND hwnd, wchar_t * epid) {
    parseRequestText2(strmatrix[2], epid);

    HINTERNET hsession2 = WinHttpOpen(L"Dango/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hsession2 == NULL) {
        MessageBox(hwnd, "WinHttpOpen failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return 0;
    }

    DWORD dwprotocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
    WinHttpSetOption(hsession2, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwprotocols, sizeof(dwprotocols));
    
    LPWSTR servernew = serverAddressInitializer(server);

    HINTERNET hconnect2 = WinHttpConnect(hsession2, servernew, (WORD)atoi(port), 0);
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
            jsonstring = getCharResponseRaw(hrequest2, hconnect2, hsession2);
            return getLinkJson(hwnd, jsonstring);
        }   
    } else
        return 0;
}

animeinfo getInfoConnection(HWND hwnd, wchar_t * epid, animeinfo info) {

    //MessageBox(NULL, L"getInfoConnectionn", L"Info", MB_ICONINFORMATION);

    parseRequestText2(strmatrix[1], epid);
    animeinfo nullstruct;

    HINTERNET hsession2 = WinHttpOpen(L"Dango/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hsession2 == NULL) {
        MessageBox(hwnd, "WinHttpOpen failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return nullstruct;
    }

    DWORD dwprotocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
    WinHttpSetOption(hsession2, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwprotocols, sizeof(dwprotocols));

    LPWSTR servernew = serverAddressInitializer(server);
    
    HINTERNET hconnect2 = WinHttpConnect(hsession2, servernew, (WORD)atoi(port), 0);
    if (hconnect2 == NULL) {
        MessageBox(hwnd, "WinHttpConnect failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return nullstruct;
    }

    HINTERNET hrequest2 = WinHttpOpenRequest(hconnect2, L"GET", requestedquery2, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (hrequest2 == NULL) {
        MessageBox(hwnd, "WinHttpOpenRequest failed!", "Error", MB_ICONERROR);
        wprintf(L"\n %s | %lu", requestedquery2, GetLastError());
        WinHttpCloseHandle(hconnect2);
        WinHttpCloseHandle(hsession2);
        return nullstruct;
    }

    wchar_t * jsonstring;

    if (WinHttpSendRequest(hrequest2, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        if (WinHttpReceiveResponse(hrequest2, NULL)) {
            jsonstring = getWideResponseRaw(hrequest2, hconnect2, hsession2);
            return getShowInfo(jsonstring, info);
        }   
    } else
        return nullstruct;
}

int getTrendsConnection(HWND hwnd, trendinganimeinfo shows[]) {
    HINTERNET hsession2 = WinHttpOpen(L"Dango/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hsession2 == NULL) {
        MessageBox(hwnd, "WinHttpOpen failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return 0;
    }

    DWORD dwprotocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
    WinHttpSetOption(hsession2, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwprotocols, sizeof(dwprotocols));

    LPWSTR servernew = serverAddressInitializer(server);
    
    HINTERNET hconnect2 = WinHttpConnect(hsession2, servernew, (WORD)atoi(port), 0);
    if (hconnect2 == NULL) {
        MessageBox(hwnd, "WinHttpConnect failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return 0;
    }

    HINTERNET hrequest2 = WinHttpOpenRequest(hconnect2, L"GET", strmatrix[2], NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (hrequest2 == NULL) {
        MessageBox(hwnd, "WinHttpOpenRequest failed!", "Error", MB_ICONERROR);
        wprintf(L"\n %s | %lu", requestedquery2, GetLastError());
        WinHttpCloseHandle(hconnect2);
        WinHttpCloseHandle(hsession2);
        return 0;
    }

    wchar_t * jsonstring;

    if (WinHttpSendRequest(hrequest2, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        if (WinHttpReceiveResponse(hrequest2, NULL)) {
            jsonstring = getWideResponseRaw(hrequest2, hconnect2, hsession2);
            return getTrendingShows(jsonstring, shows);
        }   
    } else
        return 0;
}

wchar_t * imageConnection(HWND hwnd, wchar_t * imageurl, wchar_t * showid) {

    wchar_t * imageserver = getLinkDomain(imageurl);
    wchar_t * imagelocation = getLinkSubdomain(imageurl, imageserver);
    wchar_t * imageextension = getFileExtension(imageurl);

    printf("\n\n fez todos os images cosios");

    if (imageextension == NULL) {
        return NULL;
    }
    
    HINTERNET hsession2 = WinHttpOpen(L"Dango/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hsession2 == NULL) {
        MessageBox(hwnd, "WinHttpOpen failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return NULL;
    }

    DWORD dwprotocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
    WinHttpSetOption(hsession2, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwprotocols, sizeof(dwprotocols));

    LPWSTR servernew = serverAddressInitializer(imageserver);
    
    HINTERNET hconnect2 = WinHttpConnect(hsession2, servernew, (WORD)atoi(port), 0);
    if (hconnect2 == NULL) {
        MessageBox(hwnd, "WinHttpConnect failed!", "Error", MB_ICONERROR);
        printf("\n %lu", GetLastError());
        return NULL;
    }

    HINTERNET hrequest2 = WinHttpOpenRequest(hconnect2, L"GET", imagelocation, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (hrequest2 == NULL) {
        MessageBox(hwnd, "WinHttpOpenRequest failed!", "Error", MB_ICONERROR);
        wprintf(L"\n %s | %lu", requestedquery2, GetLastError());
        WinHttpCloseHandle(hconnect2);
        WinHttpCloseHandle(hsession2);
        return NULL;
    }

    wchar_t * jsonstring;

    if (WinHttpSendRequest(hrequest2, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        if (WinHttpReceiveResponse(hrequest2, NULL)) {
            return winHttpGetResponseBin(hrequest2, hconnect2, hsession2, createImagePath(showid, imageextension));
        } else {
            return NULL;
        }
    } else
        return NULL;
}
