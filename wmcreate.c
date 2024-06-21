#include "tools.h"

extern HWND hwndmain;
extern HWND hwndsearch;
extern HWND hwndsettings;

//janela principal
HWND hbutton;            
HWND htext;              
HBITMAP hbitmap;
HWND hsearchpanel;
HWND hsearchlabel;         
HWND hsearchbox; 
HWND hsearchbutton, hcancelbutton;  
HWND settingsbutton;

//utilidades
HFONT htitlefont;
HFONT hfont;
HBRUSH hbrush;

//pesquisa
HWND hshowlistbox;
HWND hwatchbutton;
HWND heplistbox;

//settings
HWND hokbutton, hcancelbutton;
HWND hproviderlist;

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

extern const char provider[32];

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
    
    if (provider == "Windows 95 ") {
        HWND htestbutton = CreateWindow(
            TEXT("BUTTON"),
            TEXT("TEST"),
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            250, 200, 120, 30,
            hwnd,
            ID_TEST,
            GetModuleHandle(NULL),
            NULL
        );
    } else {
        //MessageBox(NULL, provider, "Error", MB_ICONERROR);
    }
    
    

    htext = CreateWindow(
        TEXT("STATIC"),
        TEXT("Trending Content"),
        WS_VISIBLE | WS_CHILD,
        25, 25, 450, 200,
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
        400, 60, 120, 30,
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
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
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
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );

    hsearchbutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Search"),
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
        200, 90, 70, 20,
        hwnd,
        IDW_SEARCH_BUTTON_SEARCH,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
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
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );

    return 1;

}

int settingsWindow(HWND hwnd) {
    hokbutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("OK"),
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
        200, 90, 70, 20,
        hwnd,
        IDW_SETTINGS_BUTTON_OK,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );

    hcancelbutton = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Cancel"),
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE,
        140, 90, 70, 20,
        hwnd,
        IDW_SETTINGS_BUTTON_CANCEL,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );

    hproviderlist = CreateWindow(
        TEXT("COMBOBOX"),
        NULL,
        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        25, 25, 150, 40,
        hwnd,
        500,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );

    SendMessage(hproviderlist, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)provider);
    SendMessage(hproviderlist, CB_SETCURSEL, (WPARAM)0, (LPARAM)provider);
    
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
