#include "tools.h"

extern const wchar_t NETWORKTAB_CLASS[];// = L"Classe";

extern HWND hwndmain;
extern HWND hwndsearch;
extern HWND hwndsettings;

char providers[6][16] = {
    PROVIDER1, PROVIDER2, "", "", "", ""
};

//janela principal
HWND hbutton;            
HWND htext;              
HBITMAP hbitmap;
HWND hsearchpanel;
HWND hsearchlabel;         
HWND hsearchbox; 
HWND hsearchbutton, hcancelbutton;  
HWND settingsbutton;
HWND htestbutton, hunavailabletext;

//utilidades
HFONT htitlefont;
HFONT hfont;
HBRUSH hbrush;

//pesquisa
HWND hshowlistbox;
HWND hwatchbutton;
HWND heplistbox;

//settings
HWND hokbutton, hcancelbutton, happlybutton;
HWND hproviderlist;
HWND htabtest;
HWND hwndnetworktab, hwndprovidertab, hwndlangtab;

//menu
HMENU hmenu;             
HMENU hsubmenufile;      
HMENU hsubmenusearch;
HMENU hsubmenusettings; 
HMENU hsubmenuabout; 

const char* localfile;

result results[100];
episode episodes[100];
trendinganimeinfo shows[12];

extern char provider[32];

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
            TEXT("STATIC"), 
            TEXT("Under construction"),
            WS_VISIBLE | WS_CHILD,
            250, 150, 450, 200,
            hwnd, 
            998,
            GetModuleHandle(NULL),
            NULL
        );

        htestbutton = CreateWindow(
            TEXT("BUTTON"),
            TEXT("TEST"),
            WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
            250, 200, 120, 30,
            hwnd,
            ID_TEST,
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
            IDW_MAIN_LABEL_UNAVAILABLETRENDING,
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

    SendMessage(htext, WM_SETFONT, htitlefont, NULL);

    hbutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Search anime"),
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        400, 25, 120, 30,
        hwnd,
        IDW_MAIN_BUTTON_SEARCH,
        GetModuleHandle(NULL),
        NULL
    );

    SendMessage(hbutton, WM_SETFONT, hfont, NULL);

    hmenu = CreateMenu();
    hsubmenufile        = CreatePopupMenu();
    hsubmenusearch      = CreatePopupMenu();
    hsubmenusettings    = CreatePopupMenu();
    hsubmenuabout       = CreatePopupMenu();

    AppendMenu(hsubmenufile, MF_STRING, IDM_FILE_HOME, "Home");
    AppendMenu(hsubmenufile, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hsubmenufile, MF_STRING, IDM_FILE_CLOSE, "Exit");
    AppendMenu(hsubmenusearch, MF_STRING, IDM_SEARCH_SEARCHBOX, "Search box...");
    AppendMenu(hsubmenusettings, MF_STRING, IDM_SETTINGS_SETTINGS, "Settings");
    AppendMenu(hsubmenuabout, MF_STRING, IDM_ABOUT_HELP, "Ajuda");
    AppendMenu(hsubmenuabout, MF_SEPARATOR, NULL, NULL);
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
    
    SendMessage(hsearchlabel, WM_SETFONT, hfont, NULL);

    hsearchbox = CreateWindow(
        TEXT("EDIT"),
        NULL,
        WS_TABSTOP | WS_CHILD | WS_BORDER | WS_EX_CLIENTEDGE | ES_WANTRETURN | WS_VISIBLE,
        130, 10, 250, 20,
        hwnd,
        IDW_SEARCH_EDIT_SEARCH,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hsearchbutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Search"),
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
        200, 90, 70, 20,
        hwnd,
        IDW_SEARCH_BUTTON_SEARCH,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    SendMessage(hsearchbutton, WM_SETFONT, hfont, NULL);

    hcancelbutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Cancel"),
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
        120, 90, 70, 20,
        hwnd,
        IDW_SEARCH_BUTTON_CANCEL,
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
    printf("\n\n\n\n\nNúmero size: %d", resultsize);

    if (resultsize != 0) {
        MessageBox(hwnd, "Working", "Info", MB_ICONEXCLAMATION);
    } else {
        MessageBox(hwnd, "No results found or error connecting", "Error", MB_ICONERROR);
        return 0;
    }

    //showSearch();
        
    hshowlistbox = CreateWindow(
        TEXT("LISTBOX"),
        NULL,
        WS_CHILD | WS_BORDER | LBS_STANDARD,
        10, 50, 330, 140,
        hwnd,
        IDW_SEARCH_LISTBOX_SHOW,
        GetModuleHandle(NULL),
        NULL
    );

    hwatchbutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Watch"),
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        350, 50, 120, 30,
        hwnd,
        IDW_SEARCH_BUTTON_WATCH,
        GetModuleHandle(NULL),
        NULL
    );

    heplistbox = CreateWindow(
        TEXT("LISTBOX"),
        NULL,
        WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_STANDARD,
        10, 250, 330, 140,
        hwnd,
        IDW_SEARCH_LISTBOX_EPISODELIST,
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

    ShowWindow(heplistbox, SW_SHOWDEFAULT);
    UpdateWindow(heplistbox);
    
}

BOOL showSettings() {
    ShowWindow(hwndsettings, SW_SHOW);
    UpdateWindow(hwndsettings);
    return TRUE;
}

BOOL hideSettings() {
    ShowWindow(hwndsettings, SW_HIDE);
    UpdateWindow(hwndsettings);
    return FALSE;
}


int settingsWindow(HWND hwnd) {

    htabtest = CreateWindowEx(
        0,
        WC_TABCONTROL,
        "",
        WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
        7, 0, 380, 190,
        hwnd,
        (HMENU)990,
        GetModuleHandle(NULL),
        //hinstance,
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
        TEXT("BUTTON"),
        TEXT("OK"),
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
        155, 195, 70, 20,
        hwnd,
        IDW_SETTINGS_BUTTON_OK,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    hcancelbutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Cancel"),
        WS_TABSTOP | WS_CHILD | WS_VISIBLE,
        235, 195, 70, 20,
        hwnd,
        IDW_SETTINGS_BUTTON_CANCEL,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    happlybutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Apply"),
        WS_TABSTOP | WS_CHILD | WS_VISIBLE,
        315, 195, 70, 20,
        hwnd,
        IDW_SETTINGS_BUTTON_APPLY,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    
    
    
    
    
}

int networkTab(HWND hwnd) {
    hproviderlist = CreateWindowEx(
        0,
        WC_COMBOBOX,
        NULL,
        CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        25, 25, 150, 200,
        hwnd,
        IDW_SETTINGS_COMBOBOX_PROVIDERLIST,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL
    );

    SendMessage(hproviderlist, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)providers[0]);
    SendMessage(hproviderlist, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)providers[1]);

    /*for (int i = 0; i < 6; i++) {
        SendMessage(hproviderlist, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)providers[i]);
        printf("%s\n", providers[i]);
        
        if (!strcmp(providers[i], provider)) 
            SendMessage(hproviderlist, CB_SETCURSEL, (WPARAM)i, (LPARAM)0);
    }*/
}
