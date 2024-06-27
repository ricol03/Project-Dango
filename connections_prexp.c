#include <Windows.h>
#include <wininet.h>
#include "connections_prexp.h"

int testconnection(HWND hwnd) {
    HINTERNET hsession = InternetOpen(L"Dango/1.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, INTERNET_FLAG_ASYNC);
    if (hsession == NULL) {
        MessageBox(NULL, "Could not open an usable session", "Error", MB_ICONERROR);
        return 0;
    }

    HINTERNET hconnect = InternetConnect(hsession, "consumet-one-sigma.vercel.app", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
    if (hconnect == NULL) {
        MessageBox(NULL, "Could not make a successful connection", "Error", MB_ICONERROR);
        return 0;
    }

    HINTERNET hrequest = HttpOpenRequest(hconnect, "GET", "/anime", NULL, NULL, NULL, NULL, NULL);
    if (hrequest == NULL) {
        MessageBox(NULL, "Could not make an usable request", "Error", MB_ICONERROR);
        return 0;
    }

    if (HttpSendRequest(hrequest, NULL, 0, NULL, 0)) {
        printf("deu");
    }
    
    
}