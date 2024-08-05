#include "tools.h"

extern HWND hwndmain;
extern HWND hwndsearch;
extern HWND hwndsettings;

char providers[6][16] = {
    PROVIDER1, PROVIDER2, "", "", "", ""
};

//main window
HWND hbutton;            
HWND htext;              
HBITMAP hbitmap;
HWND hsearchpanel;
HWND hsearchlabel;         
HWND hsearchbox; 
HWND hsearchbutton, hcancelbutton;  
HWND settingsbutton;
HWND htestbutton, hunavailabletext;

//utils
HFONT htitlefont;
HFONT hfont;
HBRUSH hbrush;

//search
HWND hlisttitle;
HWND hshowlistbox;
HWND hselectbutton;
HWND hwatchbutton;
HWND heplistbox;

//info
HWND hdescriptiontext;

//settings
HWND hokbutton, hcancelbutton, happlybutton;
HWND htabtest;
HWND hwndnetworktab, hwndprovidertab, hwndlangtab;

//network tab child windows
HWND hserverlabel, hprotocollabel, hportlabel;
HWND hserverbox, hportbox;
HWND hhttpsradio, hhttpradio;
HWND hprotocolcheck;

//provider tab child windows
HWND hproviderlabel;
HWND hproviderlist;
HWND hwarningnotice;

//language tab child windows
HWND hlanglabel, hlanglist;

//menu
HMENU hmenu;             
HMENU hsubmenufile;      
HMENU hsubmenusearch;
HMENU hsubmenusettings; 
HMENU hsubmenuabout; 

const char* localfile;

result results[100];
episode episodes[100];
stream streams[8];
trendinganimeinfo shows[12];
animeinfo show;

extern char provider[32];
extern char server[32];
extern char protocol[6];
extern char port[6];

void createUtils() {
    htitlefont = CreateFont(
        32,
        0,
        0,
        0,
        FW_BOLD,
        0, 0, 0, 
        DEFAULT_CHARSET, 
        OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, 
        ANTIALIASED_QUALITY, 
        FF_DONTCARE,
        TEXT("Arial")
    );

    hfont = CreateFont(
        16,
        0,
        0,
        0,
        FW_NORMAL,
        0, 0, 0, 
        DEFAULT_CHARSET, 
        OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, 
        ANTIALIASED_QUALITY, 
        FF_DONTCARE,
        TEXT("Arial")
    );

    hbrush = CreateSolidBrush(RGB(255, 255, 0));

    /*OSVERSIONINFO winvista;

    winvista.dwMajorVersion = 6;
    winvista.dwMinorVersion = 0;

    int test = VerifyVersionInfo(&winvista, VER_MAJORVERSION | VER_MINORVERSION, VER_EQUAL);

    if (test)
        MessageBox(NULL, "Windows 7", "Info", MB_ICONINFORMATION);
    else
        MessageBox(NULL, "Other Windows version", "Info", MB_ICONINFORMATION);*/
    

}

BOOL CALLBACK enumVisibleChildWindowsProc(HWND hwnd, LPARAM lparam) {
    if (IsWindowVisible(hwnd))
        DestroyWindow(hwnd);
    
    return TRUE;
}

void destroyVisibleChildWindows(HWND hwndparent) {
    EnumChildWindows(hwndparent, enumVisibleChildWindowsProc, 0);
}

int homeWindow(HWND hwnd) {
    destroyVisibleChildWindows(hwnd);

    if (!strcmp(provider, PROVIDER1)) {
        HWND htesttext = CreateWindow(
            WC_STATIC, 
            TEXT("Under construction"),
            WS_VISIBLE | WS_CHILD,
            250, 150, 450, 200,
            hwnd, 
            (HMENU)998,
            GetModuleHandle(NULL),
            NULL
        );

        htestbutton = CreateWindow(
            TEXT("BUTTON"),
            TEXT("TEST"),
            WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
            250, 200, 120, 30,
            hwnd,
            (HMENU)ID_TEST,
            GetModuleHandle(NULL),
            NULL
        );
    } else if (!strcmp(provider, PROVIDER2)) {
        hunavailabletext = CreateWindow(
            TEXT("STATIC"), 
            IDT_UNAVAILABLETRENDING,
            WS_VISIBLE | WS_CHILD,
            100, 100, 450, 200,
            hwnd, 
            (HMENU)IDW_MAIN_LABEL_UNAVAILABLETRENDING,
            GetModuleHandle(NULL),
            NULL
        );
    }
        
    htext = CreateWindow(
        TEXT("STATIC"),
        TEXT("Trending Content"),
        WS_VISIBLE | WS_CHILD,
        25, 25, 450, 50,
        hwnd,
        (HMENU)1,
        GetModuleHandle(NULL),
        NULL
    );

    SendMessage(htext, WM_SETFONT, (WPARAM)htitlefont, (LPARAM)NULL);

    hbutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Search anime"),
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        400, 25, 120, 30,
        hwnd,
        (HMENU)IDW_MAIN_BUTTON_SEARCH,
        GetModuleHandle(NULL),
        NULL
    );

    SendMessage(hbutton, WM_SETFONT, (WPARAM)hfont, (LPARAM)NULL);

    hmenu = CreateMenu();
    hsubmenufile        = CreatePopupMenu();
    hsubmenusearch      = CreatePopupMenu();
    hsubmenusettings    = CreatePopupMenu();
    hsubmenuabout       = CreatePopupMenu();

    AppendMenu(hsubmenufile, MF_STRING, IDM_FILE_HOME, "Home");
    AppendMenu(hsubmenufile, MF_SEPARATOR, 0, NULL);
    AppendMenu(hsubmenufile, MF_STRING, IDM_FILE_CLOSE, "Exit");
    AppendMenu(hsubmenusearch, MF_STRING, IDM_SEARCH_SEARCHBOX, "Search box...");
    AppendMenu(hsubmenusettings, MF_STRING, IDM_SETTINGS_SETTINGS, "Settings");
    AppendMenu(hsubmenuabout, MF_STRING, IDM_ABOUT_HELP, "Ajuda");
    AppendMenu(hsubmenuabout, MF_SEPARATOR, 0, NULL);
    AppendMenu(hsubmenuabout, MF_STRING, IDM_ABOUT_ABOUT, "Acerca de");
    AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT_PTR)hsubmenufile, "File");
    AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT_PTR)hsubmenusearch, "Search");
    AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT_PTR)hsubmenusettings, "Settings");
    AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT_PTR)hsubmenuabout, "About");

    SetMenu(hwnd, hmenu);
}

int searchWindow(HWND hwnd) {
    hsearchlabel = CreateWindow(
        TEXT("STATIC"),
        TEXT("Search here: "),
        WS_CHILD | WS_VISIBLE,
        10, 10, 100, 20,
        hwnd,
        (HMENU)1,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    if (hsearchlabel == NULL) {
        MessageBox(hwnd, "Olha o erro", "Erro", MB_ICONERROR);
        printf("Erro do label: %lu", GetLastError());
    }
    
    SendMessage(hsearchlabel, WM_SETFONT, (WPARAM)hfont, (LPARAM)NULL);

    hsearchbox = CreateWindow(
        TEXT("EDIT"),
        NULL,
        WS_TABSTOP | WS_CHILD | WS_BORDER | WS_EX_CLIENTEDGE | ES_WANTRETURN | WS_VISIBLE,
        130, 10, 250, 20,
        hwnd,
        (HMENU)IDW_SEARCH_EDIT_SEARCH,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hsearchbutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Search"),
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
        200, 90, 70, 20,
        hwnd,
        (HMENU)IDW_SEARCH_BUTTON_SEARCH,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    SendMessage(hsearchbutton, WM_SETFONT, (WPARAM)hfont, (LPARAM)NULL);

    hcancelbutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Cancel"),
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
        120, 90, 70, 20,
        hwnd,
        (HMENU)IDW_SEARCH_BUTTON_CANCEL,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    return 1;
}

BOOL hideSearch() {
    ShowWindow(hwndsearch, SW_HIDE);
    UpdateWindow(hwndsearch);
    CheckMenuItem(hmenu, IDM_SEARCH_SEARCHBOX, MF_UNCHECKED);
    return FALSE;
}

BOOL showSearch() {
    ShowWindow(hwndsearch, SW_SHOW);
    UpdateWindow(hwndsearch);
    CheckMenuItem(hmenu, IDM_SEARCH_SEARCHBOX, MF_CHECKED);
    return TRUE;
}

int searchResults(HWND hwnd, char * query) {
    destroyVisibleChildWindows(hwnd);

    //faz a conexão à api para fazer a pesquisa, e devolve o número de resultados
    int resultsize = searchConnection(hwnd, query, results);

    if (resultsize != 0) {
        MessageBox(hwnd, "Working", "Info", MB_ICONEXCLAMATION);
    } else {
        MessageBox(hwnd, "No results found or error connecting", "Error", MB_ICONERROR);
        return 0;
    }
    
    hlisttitle = CreateWindow(
        WC_STATIC,
        "Search Results",
        WS_VISIBLE | WS_CHILD,
        25, 25, 350, 50,
        hwnd,
        (HMENU)1010,
        GetModuleHandle(NULL),
        NULL
    );

    SendMessage(hlisttitle, WM_SETFONT, (WPARAM)htitlefont, (LPARAM)NULL);
        
    hshowlistbox = CreateWindow(
        TEXT("LISTBOX"),
        NULL,
        WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_STANDARD,
        25, 75, 545, 350,
        hwnd,
        (HMENU)IDW_SEARCH_LISTBOX_SHOW,
        GetModuleHandle(NULL),
        NULL
    );

    //TODO: REALLOCATE THE WATCH BUTTON
    hwatchbutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Watch"),
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
        350, 50, 120, 30,
        hwnd,
        (HMENU)IDW_SEARCH_BUTTON_WATCH,
        GetModuleHandle(NULL),
        NULL
    );

    hselectbutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Select"),
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        450, 25, 120, 30,
        hwnd,
        (HMENU)IDW_SEARCH_BUTTON_SELECT,
        GetModuleHandle(NULL),
        NULL
    );

    //TODO: REALLOCATE THE EP LIST
    heplistbox = CreateWindow(
        TEXT("LISTBOX"),
        NULL,
        WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_STANDARD,
        10, 250, 330, 140,
        hwnd,
        (HMENU)IDW_SEARCH_LISTBOX_EPISODELIST,
        GetModuleHandle(NULL),
        NULL
    );

    if (hshowlistbox == NULL) {
        MessageBox(NULL, "List box creation failed!", "Error", MB_ICONERROR);
        printf("Error in listbox: %lu", GetLastError());
        return 1;
    }

    for (int i = 0; i < resultsize; i++) {
        int position = (int)SendMessage(hshowlistbox, LB_INSERTSTRING, 0, (LPARAM)results[i].title);
        SendMessage(hshowlistbox, LB_SETITEMDATA, position, (LPARAM)i);
        printf("\n%s", results[i].id);
    }
    
    ShowWindow(hshowlistbox, SW_SHOWDEFAULT);
    UpdateWindow(hshowlistbox);

    if (heplistbox == NULL) {
        MessageBox(NULL, "List box creation failed!", "Error", MB_ICONERROR);
        printf("Error in listbox: %lu", GetLastError());
        return 1;
    }
}

int infoWindow(HWND hwnd) {
    MessageBox(hwnd, show.title, "Info", MB_ICONINFORMATION);

    HWND hinfotitletext = CreateWindow(
        WC_STATIC, 
        "devia dar",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        250, 150, 450, 200,
        hwnd,
        (HMENU)1098,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hdescriptiontext = CreateWindow(
        WC_STATIC, 
        show.description,
        WS_VISIBLE | WS_CHILD,
        250, 150, 450, 200,
        hwnd,
        (HMENU)1099,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    HWND hokbuttons = CreateWindow(
        WC_BUTTON,
        TEXT("OK"),
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
        155, 192, 70, 25,
        hwnd,
        (HMENU)11111,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );
}

BOOL showSettings() {
    createSettingsWindow(GetModuleHandle(NULL));
    ShowWindow(hwndsettings, SW_SHOW);
    UpdateWindow(hwndsettings);
    return TRUE;
}

BOOL hideSettings() {
    DestroyWindow(hwndsettings);
    //ShowWindow(hwndsettings, SW_HIDE);
    //UpdateWindow(hwndsettings);
    return FALSE;
}

int settingsWindow(HWND hwnd) {
    htabtest = CreateWindowEx(
        WS_EX_TRANSPARENT,
        WC_TABCONTROL,
        "",
        WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
        5, 5, 380, 180,
        hwnd,
        (HMENU)990,
        GetModuleHandle(NULL),
        NULL
    );

    if (htabtest == NULL) {
        MessageBox(NULL, "Não funcionou", "Error", MB_ICONERROR);
        printf("Erro: %lu", GetLastError());
    }

    INITCOMMONCONTROLSEX icex;
    TCITEM tcnetwork, tcprovider, tclang;

    tcnetwork.mask = TCIF_TEXT;
    tcnetwork.pszText = "Network";

    tcprovider.mask = TCIF_TEXT;
    tcprovider.pszText = "Provider";

    tclang.mask = TCIF_TEXT;
    tclang.pszText = "Language";

    icex.dwICC = ICC_TAB_CLASSES;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);

    InitCommonControlsEx(&icex);

    TabCtrl_InsertItem(htabtest, 0, &tcnetwork);
    TabCtrl_InsertItem(htabtest, 1, &tcprovider);
    TabCtrl_InsertItem(htabtest, 2, &tclang);
    
    hokbutton = CreateWindow(
        WC_BUTTON,
        TEXT("OK"),
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
        155, 192, 70, 25,
        hwnd,
        (HMENU)IDW_SETTINGS_BUTTON_OK,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hcancelbutton = CreateWindow(
        WC_BUTTON,
        TEXT("Cancel"),
        WS_TABSTOP | WS_CHILD | WS_VISIBLE,
        235, 192, 70, 25,
        hwnd,
        (HMENU)IDW_SETTINGS_BUTTON_CANCEL,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    happlybutton = CreateWindow(
        WC_BUTTON,
        TEXT("Apply"),
        WS_TABSTOP | WS_CHILD | WS_VISIBLE,
        315, 192, 70, 25,
        hwnd,
        (HMENU)IDW_SETTINGS_BUTTON_APPLY,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );
}

int networkTab(HWND hwnd) {
    hserverlabel = CreateWindow(
        WC_STATIC,
        TEXT("Server address (IP/URL): "),
        WS_CHILD | WS_VISIBLE,
        10, 10, 100, 40,
        hwnd,
        (HMENU)949,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hserverbox = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WC_EDIT,
        NULL,
        WS_TABSTOP | WS_CHILD | ES_WANTRETURN | WS_VISIBLE,
        125, 20, 225, 20,
        hwnd,
        (HMENU)950,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    if (server != NULL)
        SetWindowText(hserverbox, server);
    
    hportlabel = CreateWindow(
        WC_STATIC,
        TEXT("Protocol: "),
        WS_CHILD | WS_VISIBLE,
        10, 60, 100, 20,
        hwnd,
        (HMENU)946,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hhttpradio = CreateWindow(
        WC_BUTTON,
        TEXT("HTTP"),
        BS_AUTORADIOBUTTON | WS_GROUP | WS_CHILD | WS_VISIBLE,
        125, 60, 70, 20,
        hwnd,
        (HMENU)IDW_NETWORK_RADIO_HTTP,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hhttpsradio = CreateWindow(
        WC_BUTTON,
        TEXT("HTTPS"),
        BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
        205, 60, 70, 20,
        hwnd,
        (HMENU)IDW_NETWORK_RADIO_HTTPS,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hportlabel = CreateWindow(
        WC_STATIC,
        TEXT("Port: "),
        WS_CHILD | WS_VISIBLE,
        10, 100, 100, 20,
        hwnd,
        (HMENU)948,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hportbox = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WC_EDIT,
        NULL,
        WS_TABSTOP | WS_CHILD | ES_WANTRETURN | WS_VISIBLE,
        125, 100, 55, 20,
        hwnd,
        (HMENU)IDW_NETWORK_EDIT_PORT,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hprotocolcheck = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Protocol's default"),
        BS_CHECKBOX | WS_CHILD | WS_VISIBLE,
        195, 100, 135, 20,
        hwnd,
        (HMENU)IDW_NETWORK_CHECKBOX_PORT,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    printf("chegou aqui");

    //sets the enabled radio button
    if (!strcmp(protocol, "HTTP")) {
        SendDlgItemMessage(hwnd, IDW_NETWORK_RADIO_HTTP, BM_SETCHECK, 1, 0);
        SendDlgItemMessage(hwnd, IDW_NETWORK_RADIO_HTTPS, BM_SETCHECK, 0, 0);
        printf("e aqui");
    } else if (!strcmp(protocol, "HTTPS")) {
        SendDlgItemMessage(hwnd, IDW_NETWORK_RADIO_HTTPS, BM_SETCHECK, 1, 0);
        SendDlgItemMessage(hwnd, IDW_NETWORK_RADIO_HTTP, BM_SETCHECK, 0, 0);
        printf("e aqui2");
    }

    if ((!strcmp(port, "443") && !strcmp(protocol, "HTTPS")) || (!strcmp(port, "80") && !strcmp(protocol, "HTTP"))) {
        SendDlgItemMessage(hwnd, IDW_NETWORK_CHECKBOX_PORT, BM_SETCHECK, 1, 0);
        SetWindowText(hportbox, port);
        EnableWindow(GetDlgItem(hwnd, IDW_NETWORK_EDIT_PORT), FALSE);
    } else
        SetWindowText(hportbox, port);
    

    printf("saiu daqui");
}

int providerTab(HWND hwnd) {
    destroyVisibleChildWindows(hwnd);

    hproviderlabel = CreateWindow(
        WC_STATIC,
        TEXT("Provider: "),
        WS_CHILD | WS_VISIBLE,
        10, 25, 100, 20,
        hwnd,
        (HMENU)948,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hproviderlist = CreateWindowEx(
        0,
        WC_COMBOBOX,
        NULL,
        CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        125, 20, 150, 200,
        hwnd,
        (HMENU)IDW_SETTINGS_COMBOBOX_PROVIDERLIST,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hwarningnotice = CreateWindow(
        WC_STATIC,
        IDT_WARNINGNOTRENDING,
        WS_CHILD,
        10, 80, 400, 125,
        hwnd,
        (HMENU)IDW_PROVIDER_LABEL_WARNING,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    /*SendMessage(hproviderlist, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)providers[0]);
    SendMessage(hproviderlist, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)providers[1]);*/

    for (int i = 0; i < 2; i++) {
        SendMessage(hproviderlist, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)providers[i]);
        printf("%s\n", providers[i]);
        
        if (!strcmp(providers[i], provider)) 
            SendMessage(hproviderlist, CB_SETCURSEL, (WPARAM)i, (LPARAM)0);
    }

    updateNotice();
}

int updateNotice() {   
    if (!strcmp(PROVIDER2, provider)) {
        ShowWindow(hwarningnotice, SW_SHOW);
        //UpdateWindow(htabtest);
    } else {
        ShowWindow(hwarningnotice, SW_HIDE);
        //UpdateWindow(htabtest);
    }
}

int languageTab(HWND hwnd) {
    hlanglabel = CreateWindow(
        WC_STATIC,
        TEXT("Language: "),
        WS_CHILD | WS_VISIBLE,
        10, 25, 100, 20,
        hwnd,
        (HMENU)948,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hlanglist = CreateWindowEx(
        0,
        WC_COMBOBOX,
        NULL,
        CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        125, 20, 150, 200,
        hwnd,
        (HMENU)IDW_SETTINGS_COMBOBOX_LANGLIST,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );
}

