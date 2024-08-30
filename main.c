#include "tools.h"
#include "libvlc/include/vlc/vlc.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//TODO: meter todos os episódios na listbox de baixo X
//TODO: quando se clica no botão watch, meter o link do episódio no libvlc X
//TODO:: meter as legendas da coisa
//TODO:: fazer um menu toggable na janela do vídeo
//TODO:: melhorar o ecrã inicial (com conteúdo em trending e etc)

//BUG: alguns dos links não funcionam, mesmo funcionando no browser X
//BUG: quando reproduzimos um vídeo, e voltamos à janela para escolher outro episódio, a janela anterior não fecha 
//BUG: alguns resultados (até agora os de conteúdo único) não têm link associado X

const wchar_t MAIN_CLASS[]          = L"MainWndClass";
const wchar_t SEARCH_CLASS[]        = L"SearchWndClass";
const wchar_t SETTINGS_CLASS[]      = L"idkwhyicantputanormalname";
const wchar_t VIDEO_CLASS[]         = L"VideoWndClass";
const wchar_t INFO_CLASS[]          = L"InfoWndClass";
const wchar_t EPISODE_CLASS[]        = L"EpisodeWndClass";

const wchar_t NETWORKTAB_CLASS[]    = L"NetTabWndClass";
const wchar_t PROVIDERTAB_CLASS[]   = L"ProviderTabWndClass";
const wchar_t LANGTAB_CLASS[]       = L"LangTabWndClass";

const wchar_t TEST_CLASS[]    = L"TCLASS";

//janelas
HWND hwndmain;
HWND hwndsearch;
HWND hwndinfo;
HWND hwndepisode;
HWND hwndsettings;
HWND hwndvideo;

//controlos do home
extern HWND hbutton, hsearchbutton;
extern HWND htext;
extern HWND hsearchlabel;              
extern HWND hsearchbox;  
extern HWND hsearchbutton;

extern HBRUSH hbrush;

//controlos das definições
extern HWND hokbutton, hcancelbutton, happlybutton;
extern HWND hproviderlist;
extern HWND htabtest;
extern HWND hwndnetworktab, hwndprovidertab, hwndlangtab;
extern HWND hprotocolcheck;

//controlos dos resultados de pesquisa
extern HWND hshowlistbox, heplistbox;
extern HWND hwatchbutton;

//controlos da janela de vídeo


//menu da janela
extern HMENU hmenu;
extern HMENU hsubmenusearch;  



//bool para a pesquisa estar vísivel ou não
boolean searchtoggled = FALSE;
boolean settingstoggled = FALSE;

extern const wchar_t * localfile;
extern result results[];
extern episode episodes[];
extern stream streams[];
extern trendinganimeinfo shows[];
extern animeinfo show;

extern HWND hproviderlist;

int number;

PAINTSTRUCT ps;

typedef struct stbInfo {
    unsigned char * imgdata;
    int width, height, planes;
} stbInfo;

extern wchar_t provider[32];
extern wchar_t lang[5];

//video stuhf
libvlc_instance_t* linst;
libvlc_media_player_t* mplay;
libvlc_media_t* media;
char * videolink;

extern int x, y, c;
extern unsigned char * imgdata;

DWORD wversion, wmajorversion, wminorversion, wbuild;

#ifndef UNIT_TEST

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, PWSTR lpcmdline, int nshowcmd) {

    setlocale(LC_ALL, "");

    if (checkVersion())
        PostQuitMessage(-1);

    #pragma region MainWindow
    WNDCLASS mainwindowclass = { 0 };

    mainwindowclass.style            = CS_OWNDC;
    mainwindowclass.lpfnWndProc      = MainWindowProc;
    mainwindowclass.hInstance        = hinstance;
    mainwindowclass.lpszClassName    = (LPCSTR)MAIN_CLASS;

    RegisterClass(&mainwindowclass);

    hwndmain = CreateWindowEx(
        0,
        MAIN_CLASS,
        L"Project Dango",
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_ICONIC | WS_ACTIVECAPTION | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, MAINWINDOWWIDTH, MAINWINDOWHEIGHT,
        NULL,
        NULL,
        hinstance,
        NULL
    );
    
    if (hwndmain == NULL) {
        MessageBoxW(NULL, L"Unable to create Windows", 
                L"Error", MB_ICONERROR | MB_OK);
        return 0;
    } else
        ShowWindow(hwndmain, SW_SHOW);

    #pragma endregion

    #pragma region SearchWindow
    WNDCLASS searchwindowclass = { 0 };

    searchwindowclass.lpfnWndProc      = SearchWindowProc;
    searchwindowclass.hInstance        = hinstance;
    searchwindowclass.lpszClassName    = (LPCSTR)SEARCH_CLASS;
    searchwindowclass.hIcon            = NULL;

    RegisterClass(&searchwindowclass);

    hwndsearch = CreateWindowEx(
        0,
        SEARCH_CLASS,
        L"Search",
        WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, SEARCHWINDOWWIDTH, SEARCHWINDOWHEIGHT,
        NULL,
        NULL,
        hinstance,
        NULL
    );
    
    if (hwndsearch == NULL) {
        MessageBoxW(NULL, L"Unable to create search window", 
                    L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    #pragma endregion

    #pragma region SettingsWindow
    WNDCLASS settingswindowclass = { 0 };

    settingswindowclass.lpfnWndProc     = SettingsWndProc; 
    settingswindowclass.hInstance       = hinstance;
    settingswindowclass.lpszClassName   = SETTINGS_CLASS;
    settingswindowclass.hIcon           = NULL;

    RegisterClass(&settingswindowclass);

    #pragma endregion
    
    #pragma region SettingsTabs
    //network tab
    WNDCLASS networktabclass = { 0 };

    //networktabclass.style            = CS_OWNDC;
    networktabclass.lpfnWndProc      = NetworkTabProc;
    networktabclass.hInstance        = hinstance;
    networktabclass.lpszClassName    = (LPCSTR)NETWORKTAB_CLASS;
    //networktabclass.hbrBackground    = (HBRUSH)RGB(255, 0, 0);

    if(!RegisterClass(&networktabclass)) {
        MessageBox(NULL, "Falhou o do network!", "Erro", MB_ICONERROR);
        printf("Erro: %lu", GetLastError());
    }

    //provider tab
    WNDCLASS providertabclass = { 0 };

    providertabclass.style            = CS_OWNDC;
    providertabclass.lpfnWndProc      = ProviderTabProc;
    providertabclass.hInstance        = hinstance;
    providertabclass.lpszClassName    = (LPCSTR)PROVIDERTAB_CLASS;

    RegisterClass(&providertabclass);

    //language tab
    WNDCLASS languagetabclass = { 0 };

    languagetabclass.style            = CS_OWNDC;
    languagetabclass.lpfnWndProc      = LangTabProc;
    languagetabclass.hInstance        = hinstance;
    languagetabclass.lpszClassName    = (LPCSTR)LANGTAB_CLASS;

    RegisterClass(&languagetabclass);

    #pragma endregion

    settingstoggled = createSettingsWindow(hinstance);

    #pragma region VideoWindow

    WNDCLASS videowindowclass = { 0 };

    videowindowclass.style            = CS_OWNDC;
    videowindowclass.lpfnWndProc      = VideoWindowProc;
    videowindowclass.hInstance        = hinstance;
    videowindowclass.lpszClassName    = (LPCSTR)VIDEO_CLASS;

    RegisterClass(&videowindowclass);

    //createVideoWindow(hinstance);

    //createVideoWindow(hinstance);

    #pragma endregion

    #pragma region InfoWindow
    WNDCLASS infowindowclass = { 0 };

    infowindowclass.lpfnWndProc     = InfoWindowProc; 
    infowindowclass.hInstance       = hinstance;
    infowindowclass.lpszClassName   = TEST_CLASS;
    infowindowclass.hIcon           = NULL;

    RegisterClass(&infowindowclass);

    #pragma endregion

    #pragma region EpisodeWindow
    WNDCLASS episodewindowclass = { 0 };

    episodewindowclass.lpfnWndProc     = EpisodeWindowProc; 
    episodewindowclass.hInstance       = hinstance;
    episodewindowclass.lpszClassName   = EPISODE_CLASS;
    episodewindowclass.hIcon           = NULL;

    RegisterClass(&episodewindowclass);

    #pragma endregion

    MSG msg = { 0 };

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    } 

    return 0;
}

#endif

int checkVersion() {
    wversion = GetVersion();

    wmajorversion = (DWORD)(LOBYTE(LOWORD(wversion)));
    wminorversion = (DWORD)(HIBYTE(LOWORD(wversion)));

    if (wmajorversion >= 5 && wmajorversion < 11) {
        return 0;
    } else if (wmajorversion < 5) {
        MessageBox(NULL, L"This program only runs on Windows 2000 and later.", L"Error", MB_ICONERROR);
        return 1;
    } else {
        MessageBox(NULL, L"Unknown Windows version. Aborting.", L"Error", MB_ICONERROR);
        return 1;
    }
}

int createSettingsWindow(HINSTANCE hinstance) {
    if (settingstoggled) {
        return 1;
    } else {
        hwndsettings = CreateWindowEx(
            0,
            SETTINGS_CLASS,
            L"Settings",
            WS_CAPTION,
            CW_USEDEFAULT, CW_USEDEFAULT, SETTINGSWINDOWWIDTH, SETTINGSWINDOWHEIGHT,
            NULL,
            NULL,
            hinstance,
            NULL
        );

        if (hwndsettings == NULL) {
            MessageBoxW(NULL, L"Unable to create settings window",
                        L"Error", MB_ICONERROR | MB_OK);
            return 0;
        } else {
            createNetworkTab();
            createProviderTab();
            createLanguageTab();

            return 1;
        }        
    }
}

int createNetworkTab() {
    RECT rect;
    GetClientRect(htabtest, &rect);

    hwndnetworktab = CreateWindowEx(
        0,
        (LPCSTR)NETWORKTAB_CLASS,
        NULL,
        WS_CHILDWINDOW | WS_VISIBLE,
        rect.left+5, rect.top+33, rect.right - rect.left - 15, rect.bottom - rect.top - 60,
        //0, 0, 100, 100,
        htabtest,
        (HMENU)989,
        //(HINSTANCE)GetWindowLong(htabtest, GWL_HINSTANCE),
        GetModuleHandle(NULL),
        //hinstance,
        NULL
    );
    
    if (hwndnetworktab == NULL) {
        MessageBox(NULL, "Não funcionou (network tab)", "Error", MB_ICONERROR);
        printf("Erro: %lu", GetLastError());
        //printf("%s", NETWORKTAB_CLASS);
    }
}

int createProviderTab() {
    RECT rect;
    GetClientRect(htabtest, &rect);

    hwndprovidertab = CreateWindowEx(
        0,
        (LPCSTR)PROVIDERTAB_CLASS,
        NULL,
        WS_CHILDWINDOW,
        rect.left + 5, rect.top+33, rect.right - rect.left - 15, rect.bottom - rect.top - 60,
        //0, 0, 100, 100,
        htabtest,
        (HMENU)988,
        (HINSTANCE)GetWindowLong(htabtest, GWL_HINSTANCE),
        //hinstance,
        NULL
    );
    
    if (hwndprovidertab == NULL) {
        MessageBox(NULL, "Não funcionou (provider tab)", "Error", MB_ICONERROR);
        printf("Erro: %lu", GetLastError());
        //printf("%s", NETWORKTAB_CLASS);
    }
}

int createLanguageTab() {
    RECT rect;
    GetClientRect(htabtest, &rect);

    hwndlangtab = CreateWindowEx(
        0,
        (LPCSTR)LANGTAB_CLASS,
        NULL,
        WS_CHILDWINDOW,
        rect.left + 5, rect.top+33, rect.right - rect.left - 15, rect.bottom - rect.top - 60,
        //0, 0, 100, 100,
        htabtest,
        (HMENU)987,
        (HINSTANCE)GetWindowLong(htabtest, GWL_HINSTANCE),
        //hinstance,
        NULL
    );
    
    if (hwndlangtab == NULL) {
        MessageBox(NULL, "Não funcionou (language tab)", "Error", MB_ICONERROR);
        printf("Erro: %lu", GetLastError());
        //printf("%s", NETWORKTAB_CLASS);
    }
}

int createVideoWindow(HINSTANCE hinstance) {

    //TODO: MAKE A PROPER WINDOW TITLE
    //TCHAR * windowtitle = strcat(windowtitle, "a");

    hwndvideo = CreateWindowEx(
        0,
        VIDEO_CLASS,
        L"Video",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        NULL,
        NULL,
        hinstance,
        NULL
    );

    if (hwndvideo == NULL) {
        MessageBoxW(NULL, L"Unable to create Windows", 
                L"Error", MB_ICONERROR | MB_OK);
        return 0;
    } 
}

int createInfoWindow(HINSTANCE hinstance) {
    hwndinfo = CreateWindowEx(
        0,
        TEST_CLASS,
        L"Info",
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_ICONIC | WS_ACTIVECAPTION,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL,
        NULL,
        hinstance,
        NULL
    );

    if (hwndinfo == NULL) {
        MessageBoxW(NULL, L"Unable to create Windows", 
                L"Error", MB_ICONERROR | MB_OK);
        return 0;
    } 
}

int createEpisodeWindow(HINSTANCE hinstance) {
    hwndepisode = CreateWindowEx(
        0,
        EPISODE_CLASS,
        L"Episode selection",
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_ICONIC | WS_ACTIVECAPTION | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL,
        NULL,
        hinstance,
        NULL
    );

    if (hwndepisode == NULL) {
        MessageBoxW(NULL, L"Unable to create Windows", 
                L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }
    
}

LRESULT CALLBACK MainWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    
    switch (message) {
        case WM_CREATE: {
            createUtils();
            readSettings();
            homeWindow(hwnd);
            /*stbInfo teste;

            teste.imgdata = stbi_load(TESTIMAGE, teste.width, teste.height, teste.planes, 0);

            if (teste.imgdata) {
                MessageBox(NULL, "Funcionou", "Info", MB_ICONINFORMATION);
            } else {
                MessageBox(NULL, "Não funcionou", "Error", MB_ICONERROR);
            }*/
        }
        return 0;

        case WM_COMMAND: {
            switch (LOWORD(wparam)) {

                case ID_TEST: {
                    createVideoWindow(GetModuleHandle(NULL));

                    ShowWindow(hwndvideo, SW_SHOW);
                    UpdateWindow(hwndvideo);

                    SetForegroundWindow(hwndvideo);
                }

                #pragma region won
                case IDM_FILE_HOME:
                    homeWindow(hwnd);
                    break;

                case IDM_FILE_CLOSE:
                    PostQuitMessage(0);
                    break;
                
                case IDM_SEARCH_SEARCHBOX: {
                    if (searchtoggled == TRUE) {
                        searchtoggled = hideSearch();
                        break;
                    }

                    if (searchtoggled == FALSE) {
                        searchtoggled = showSearch();
                        break;
                    }
                    
                    break;
                }

                case IDM_SETTINGS_SETTINGS: {
                    settingstoggled = showSettings();
                    break;
                }

                case IDW_MAIN_BUTTON_SEARCH: {
                    searchtoggled = showSearch();
                    break;
                }
         
                case IDW_SEARCH_BUTTON_SELECT: {
                    //TODO: put this logic on the eventual 'select' button
                    memset(&episodes, 0, sizeof(episode));
                    memset(&show, 0, sizeof(animeinfo));
                    int item = (int)SendMessage(hshowlistbox, LB_GETCURSEL, 0, 0);
                    int i = (int)SendMessage(hshowlistbox, LB_GETITEMDATA, item, 0);
                    MessageBox(hwnd, results[i].id, L"Info", MB_ICONINFORMATION);

                    wcscpy(show.id, results[i].id);
                    wcscpy(show.title, results[i].title);
                    show.isempty = TRUE;

                    show = getInfoConnection(hwnd, results[i].id, show);

                    if (show.isempty == FALSE) {
                        createInfoWindow(GetModuleHandle(NULL));

                        //show info window
                        ShowWindow(hwndinfo, SW_SHOW);
                        UpdateWindow(hwndinfo);
                    } 
                }
                return 0;

                default:
                    break;
            }
            
            return 0;
        
        }

        case WM_PAINT: {
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_3DFACE+1));

            EndPaint(hwnd, &ps);
        }
            return 0;

        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, message, wparam, lparam);
}

LRESULT CALLBACK VideoWindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    
    switch (message) {
        case WM_CREATE: {
    
        }
        return 0;

        case WM_SHOWWINDOW: {
            linst = libvlc_new(0, NULL);



            //size_t a = wcstombs(NULL, videolink, 0);

            /*char * charlink = (char *)malloc(wcslen(videolink) * sizeof(char));
            ZeroMemory(charlink, sizeof(charlink));*/

            /*videolink[wcslen(videolink)] = L"\0";

            wcstombs(charlink, videolink, wcslen(videolink));  */

            if (videolink != NULL) {
                //WideCharToMultiByte(CP_UTF8, 0, videolink, -1, charlink, wcslen(videolink) * sizeof(char), 0, 0);

                printf("\nlink: %ls", videolink);
                //printf("\nlink novo: %s\n\n", charlink);

                if (linst != NULL) {
                    media = libvlc_media_new_location(linst, videolink);
                    printf("este é o link saboroso do coisinho: %s", videolink);

                    mplay = libvlc_media_player_new_from_media(media);
                    printf("Ele fez esta parte do new from media");

                    libvlc_media_release(media);
                    printf("Ele fez esta parte do media release");

                    libvlc_media_player_set_hwnd(mplay, hwnd);
                    printf("Ele fez esta parte do hwnd");

                    libvlc_media_player_play(mplay);
                    printf("Ele fez esta parte do play");

                    return 0;
                } else {
                    MessageBox(hwnd, L"No instance could be created", L"Error", MB_ICONERROR);
                    return -1;
                }
                AppendMenu(GetSystemMenu(hwnd, FALSE), MF_SEPARATOR, 0, NULL);
                //AppendMenu(GetSystemMenu(hwnd, FALSE), MF_STRING, IDW_VIDEO_SYS_TOGGLE, "Control menu");
            } else {
                MessageBox(NULL, L"An error occured and the video cannot be played.", L"Error", MB_ICONERROR);
            }

            
        }
        return 0;

        /*case WM_SYSCOMMAND: {
            switch (wparam) {
                case IDW_VIDEO_SYS_TOGGLE:
                    MessageBox(hwnd, "Test", "Test", MB_ICONEXCLAMATION);
                    break;
                
                default:
                    break;
            }
            return 0;
        }*/

        case WM_PAINT: {
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_3DFACE+1));

            EndPaint(hwnd, &ps);
        }
        return 0;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY:
            libvlc_media_player_stop(mplay);
            libvlc_media_player_release(mplay);
            libvlc_release(linst);
            return 0;

        default:
            return DefWindowProc(hwnd, message, wparam, lparam);
    }

    
}

LRESULT CALLBACK SearchWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_CREATE:
            searchWindow(hwnd);
            return 0;

        case WM_COMMAND: {
            switch (LOWORD(wparam)) {
                case IDW_SEARCH_BUTTON_SEARCH: {
                    wchar_t searchquery[256];
                    GetWindowText(hsearchbox, searchquery, sizeof(searchquery));

                    MessageBox(NULL, searchquery, L"Info", MB_ICONINFORMATION);

                    if (searchquery != NULL) {
                        ShowWindow(hwnd, SW_HIDE);
                        searchtoggled = hideSearch();
                        if (searchResults(hwndmain, searchquery) == 0) {
                            homeWindow(hwndmain);
                        }
                    }
                    break;
		        } 

                case IDW_SEARCH_BUTTON_CANCEL: {
                    searchtoggled = hideSearch();
                    break;
		        } 
            
                default:
                    break;
            }
            return 0;
        }  
            
        case WM_PAINT: {
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_3DFACE+1));

            EndPaint(hwnd, &ps);
        
            return 0;
        }

        case WM_CLOSE:
        case WM_DESTROY:
            searchtoggled = hideSearch();
            return 0;

        default:
            return DefWindowProc(hwnd, message, wparam, lparam);
    }
}

LRESULT CALLBACK SettingsWndProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_CREATE:
            settingsWindow(hwnd);
            return 0; 

        case WM_COMMAND: {
            switch (LOWORD(wparam)) {
                case IDW_SETTINGS_BUTTON_OK: {
                    saveSettings();
                    settingstoggled = hideSettings();
                    homeWindow(hwndmain); //refreshing the window
                    break;
                }

                case IDW_SETTINGS_BUTTON_CANCEL:
                    settingstoggled = hideSettings();
                    break;

                case IDW_SETTINGS_BUTTON_APPLY:
                    saveSettings();
                    break;

                default:
                    break;
            }
        }
        return 0;

        case WM_NOTIFY: {
            LPNMHDR a = (LPNMHDR)lparam;
            switch (a->code) {
                case TCN_SELCHANGE: {
                    switch (TabCtrl_GetCurSel(htabtest)) {
                        case 0:
                            showNetworkTab();
                            break;
                        
                        case 1:
                            showProviderTab();
                            break;

                        case 2:
                            showLanguageTab();
                            break;
                        
                        default:
                            break;
                    }
                    
                    
                    
                    
                    break;
                }
                default:
                    break;
            }
        }
        return 0;
            
        case WM_PAINT: {
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_3DFACE+1));

            EndPaint(hwnd, &ps);
        }
        return 0;

        case WM_CLOSE:
        case WM_DESTROY:
            settingstoggled = hideSettings();
            return 0;

        default:
            return DefWindowProc(hwnd, message, wparam, lparam);
    }
}

LRESULT CALLBACK NetworkTabProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

    HDC hdc;

    switch (message) {
        case WM_CREATE: {
            networkTab(hwnd);
            break;
        }
        return 0;

        case WM_COMMAND: {
            switch (LOWORD(wparam)) {
                case IDW_NETWORK_CHECKBOX_PORT: {
                    switch (HIWORD(wparam)) {
                        case BN_CLICKED: {
                            if (SendDlgItemMessage(hwnd, IDW_NETWORK_CHECKBOX_PORT, BM_GETCHECK, 0, 0) == 0) {
                                SendDlgItemMessage(hwnd, IDW_NETWORK_CHECKBOX_PORT, BM_SETCHECK, 1, 0);
                                EnableWindow(GetDlgItem(hwnd, IDW_NETWORK_EDIT_PORT), FALSE);
                            } else {
                                SendDlgItemMessage(hwnd, IDW_NETWORK_CHECKBOX_PORT, BM_SETCHECK, 0, 0);
                                EnableWindow(GetDlgItem(hwnd, IDW_NETWORK_EDIT_PORT), TRUE);
                            }
                            break;
                        }
                        default:
                            break;
                        
                    }
                    break;
                }
                
                case IDW_NETWORK_RADIO_HTTP: {
                    switch(HIWORD(lparam)) {
                        case BN_CLICKED: {
                            if (SendDlgItemMessage(hwnd, IDW_NETWORK_RADIO_HTTP, BM_GETCHECK, 0, 0) == 0) {
                                SendDlgItemMessage(hwnd, IDW_NETWORK_RADIO_HTTP, BM_SETCHECK, 1, 0);
                                SendDlgItemMessage(hwnd, IDW_NETWORK_RADIO_HTTPS, BM_SETCHECK, 0, 0);
                            }
                        }  
                    }
                    break;
                }

                case IDW_NETWORK_RADIO_HTTPS: {
                    switch(HIWORD(lparam)) {
                        case BN_CLICKED: {
                            if (SendDlgItemMessage(hwnd, IDW_NETWORK_RADIO_HTTPS, BM_GETCHECK, 0, 0) == 0) {
                                SendDlgItemMessage(hwnd, IDW_NETWORK_RADIO_HTTPS, BM_SETCHECK, 1, 0);
                                SendDlgItemMessage(hwnd, IDW_NETWORK_RADIO_HTTP, BM_SETCHECK, 0, 0);
                            }
                        }  
                    }
                    break;
                }

                /*case IDW_NETWORK_CHECKBOX_PORT: {
                    switch (HIWORD(lparam))
                    {
                    case BN_CLICKED:
                        if (SendDlgItemMessage(hwnd, IDW_NETWORK_CHECKBOX_PORT, BM_GETCHECK, 0, 0) == 0) {

                        } else {

                        }
                        break;
                    
                    default:
                        break;
                    }
                }*/
                
                default:
                    break;
            }
        }
        
        case WM_PAINT: {
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) ((GetSysColor(COLOR_WINDOW)+1)));

            EndPaint(hwnd, &ps);
        }
        return 0;

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORDLG:
            EnumChildWindows(hwnd, SetBackgroundColorProc, (LPARAM)hdc);

        return 0;

        default:
            return DefWindowProc(hwnd, message, wparam, lparam);
    }
}

LRESULT CALLBACK ProviderTabProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    
    HDC hdc;
    
    switch (message) {
        case WM_CREATE: {
            providerTab(hwnd);
            break;
        }
        return 0;

        case WM_COMMAND: {
            if (HIWORD(wparam) == CBN_SELCHANGE) {
                int index = SendMessage((HWND)lparam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                SendMessage((HWND)lparam, (UINT)CB_GETLBTEXT, (WPARAM)index, (LPARAM)provider);

                printf("deu isto zeca: %s", provider);

                updateNotice();
                
                break;
            }
        }
        
        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) ((GetSysColor(COLOR_WINDOW)+1)));

            EndPaint(hwnd, &ps);
        }
        return 0;

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORDLG:
            EnumChildWindows(hwnd, SetBackgroundColorProc, (LPARAM)hdc);

        return 0;

        default:
            return DefWindowProc(hwnd, message, wparam, lparam);
    }
}

LRESULT CALLBACK LangTabProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

    HDC hdc;

    switch (message) {
        case WM_CREATE: {
            languageTab(hwnd);
            EnumChildWindows(hwnd, SetBackgroundColorProc, (LPARAM)hdc);
            break;
        }
        return 0;

        /*case WM_COMMAND: {
            if (HIWORD(wparam) == CBN_SELCHANGE) {
                int index = SendMessage((HWND)lparam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                SendMessage((HWND)lparam, (UINT)CB_GETLBTEXT, (WPARAM)index, (LPARAM)lang);

                printf("deu isto zeca: %s", provider);

                updateNotice();
                
                break;
            }
        }*/
        
        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) ((GetSysColor(COLOR_WINDOW)+1)));

            EndPaint(hwnd, &ps);
        }
        return 0;

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORDLG:
            EnumChildWindows(hwnd, SetBackgroundColorProc, (LPARAM)hdc);

        return 0;

        default:
            return DefWindowProc(hwnd, message, wparam, lparam);
    }
}

LRESULT CALLBACK InfoWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_CREATE:
            infoWindow(hwnd);
            return 0;
        case WM_COMMAND: {
            switch (LOWORD(wparam)) {
                case IDW_INFO_BUTTON_RETURN:
                    DestroyWindow(hwnd);
                    break;

                case IDW_INFO_BUTTON_WATCH:
                    memset(&episodes, 0, sizeof(episode));
                    /*int item = (int)SendMessage(hshowlistbox, LB_GETCURSEL, 0, 0);
                    int i = (int)SendMessage(hshowlistbox, LB_GETITEMDATA, item, 0);
                    MessageBox(hwnd, results[i].id, "Info", MB_ICONINFORMATION);*/


                    printf("\n\nepisodescoiso\n");
                    number = episodesConnection(hwnd, show.id, episodes);

                    for (int i = 0; i < number; i++) {
                        printf("\n%s", episodes[i].id);
                    }

                    createEpisodeWindow(GetModuleHandle(NULL));
                    break;
            
                default:
                    break;
            }
            return 0;
        }  
            
        case WM_PAINT: {
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_3DFACE+1));

            BITMAPINFO bmap;
            HBITMAP hbitmap;

            void * dibdata;
            //HDC newhdc = GetDC(NULL);

            ZeroMemory(&bmap, sizeof(bmap));
            bmap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmap.bmiHeader.biWidth = x;
            bmap.bmiHeader.biHeight = -y;
            //tem de ser sempre 1!!!
            bmap.bmiHeader.biPlanes = 1;
            bmap.bmiHeader.biBitCount = 32;
            bmap.bmiHeader.biCompression = BI_RGB;

            /*float aspectratio = (float)bmap.bmiHeader.biWidth / -(float)bmap.bmiHeader.biHeight;

            printf("\n\n\naspect ratio: %f", aspectratio);

            int scaledwidth = 225;
            int scaledheight = (int)(225 / aspectratio);

            if (scaledheight > 350) {
                scaledheight = 350;
                scaledwidth = (int)(350 * aspectratio);
            }

            printf("\nscaled height: %d", scaledheight);
            printf("\nscaled width: %d", scaledwidth);

            hbitmap = CreateDIBSection(hdc, &bmap, DIB_RGB_COLORS, &dibdata, NULL, 0);

            //hbitmap = CreateBitmap(x, y, c, 3, a);

            //ReleaseDC(NULL, newhdc);
            
            if (hbitmap) {

                

                
                


                RECT rc;
                GetWindowRect(hwnd, &rc);

                int windowwidth = rc.right - rc.left;
                int windowheight = rc.bottom - rc.top;

                HDC hdcmem = CreateCompatibleDC(hdc);
                HBITMAP hbm = (HBITMAP)SelectObject(hdcmem, hbitmap);

                BITMAP bm;
                GetObject(hbitmap, sizeof(bm), &bm);

                StretchDIBits(hdc, 0, 0, windowwidth, windowheight,
                                   0, 0, bm.bmWidth, bm.bmHeight,
                                   dibdata, &bmap, DIB_RGB_COLORS, SRCCOPY);
                StretchBlt(hdc, 30, 75, scaledwidth, scaledheight, hdcmem, 0, 0, bmap.bmiHeader.biWidth, bmap.bmiHeader.biHeight, SRCCOPY);

                SelectObject(hdcmem, hbm);
                DeleteDC(hdcmem);
            } else {
                MessageBox(NULL, "morreu", "error", MB_ICONERROR);
            }*/

            float aspectratio = (float)x / (float)y;

            int scaledwidth = 225; // Target width
            int scaledheight = (int)(scaledwidth / aspectratio);

            if (scaledheight > 350) {
                scaledheight = 350;
                scaledwidth = (int)(scaledheight * aspectratio);
            }

            SetStretchBltMode(hdc, HALFTONE);

            hbitmap = CreateDIBSection(hdc, &bmap, DIB_RGB_COLORS, &dibdata, NULL, 0);

            if (hbitmap) {
                unsigned char * data = (unsigned char *)imgdata;
                unsigned char * row1 = (unsigned char *)dibdata;

                for (int i = 0; i < y; i++) {
                    unsigned char * pixel = (unsigned char *)row1;
                    unsigned char * pixel2 = (unsigned char *)data;
                    for (int j = 0; j < x; j++) {
                        unsigned char r = pixel2[0];
                        unsigned char g = pixel2[1];
                        unsigned char b = pixel2[2];
                        unsigned char a = pixel2[3];

                        pixel[0] = b;
                        pixel[1] = g;
                        pixel[2] = r;
                        pixel[3] = a;

                        pixel += 4;
                        pixel2 += 4;
                    } 

                    row1 += x*4;
                    data += x*4;
                }

                RECT rc;
                GetClientRect(hwnd, &rc);

                HDC hdcmem = CreateCompatibleDC(hdc);
                HBITMAP hbmold = (HBITMAP)SelectObject(hdcmem, hbitmap);

                StretchBlt(hdc, 30, 75, scaledwidth, scaledheight,
                   hdcmem, 0, 0, x, y, SRCCOPY);

                SelectObject(hdcmem, hbmold);
                DeleteDC(hdcmem);
                DeleteObject(hbitmap);
                
            } else {
                MessageBox(hwnd, L"Failed to create DIB section", L"Error", MB_ICONERROR);
            }

            EndPaint(hwnd, &ps);
            DeleteObject(hdc);

            return 0;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            
            return 0;

        default:
            return DefWindowProc(hwnd, message, wparam, lparam);
    }
}

LRESULT CALLBACK EpisodeWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_CREATE:
            episodeWindow(hwnd);
            return 0;
        case WM_COMMAND: {
            switch (LOWORD(wparam)) {
                case IDW_EPISODE_BUTTON_WATCH: {
                    int item = (int)SendMessage(heplistbox, LB_GETCURSEL, 0, 0);
                    int i = (int)SendMessage(heplistbox, LB_GETITEMDATA, item, 0);

                    MessageBox(NULL, episodes[i].id, L"Info", MB_ICONASTERISK);

                    wchar_t * link = eplinkConnection(hwnd, episodes[i].id);

                    printf("\n\nlink do command: %ls\\", link);
                    //getchar();

                    if (link == NULL) {
                        MessageBox(NULL, L"An error occured, please try again later.", L"Error", MB_ICONERROR);
                    } else { 
                        printf("\n\n\ndevolveu este link zeca: %ls", link);

                        /*if (!wcscmp(provider, PROVIDER1)) {z
                            getQualitiesJson(link, streams);
                            wcscpy(videolink, streams[1].link);
                        } */

                        

                        videolink = (char *)malloc(wcslen(link) * sizeof(char));
                        printf("\n\nsizeof: %d", (wcslen(link) * sizeof(char)));
                        //wcscpy(videolink, link);

                        if (convertWideToMulti(link, videolink)) {
                            MessageBox(NULL, L"Deu com firmeza", L"Info", MB_ICONINFORMATION);
                        }


                        //videolink = link;

                        createVideoWindow(GetModuleHandle(NULL));
                        ShowWindow(hwndvideo, SW_SHOW);
                        UpdateWindow(hwndvideo);

                        SetForegroundWindow(hwndvideo);
                    }
                }
                return 0;

                case IDW_SEARCH_LISTBOX_EPISODELIST: {
                    switch (HIWORD(wparam)) {
                        case LBN_SELCHANGE: {
                            
                            int item = (int)SendMessage(heplistbox, LB_GETCURSEL, 0, 0);
                            int i = (int)SendMessage(heplistbox, LB_GETITEMDATA, item, 0);
                            MessageBox(hwnd, episodes[i].id, L"Info", MB_ICONINFORMATION);

                            

                            //int number = episodesconnection(hwnd, episodes[i].id, episodes);

                            /*for (int i = 0; i < number; i++) {
                                printf("\n%s", episodes[i].title);
                            }
                             
                            SendMessage(heplistbox, LB_RESETCONTENT, 0, 0);
                            //SendMessage(heplistbox, LB_INITSTORAGE, (WPARAM)(int)number, (LPARAM)(sizeof(wchar_t)*sizeof(episodes->title)));
                            printf("\n\n\n\n Isto deu meu amigo, olha: %s", episodes[0].title);
                            for (int i = 0; i < number; i++) {
                                printf("%s", episodes[i].title);
                                printf("\n\n\nerro do capeta: %lu", GetLastError());
                                int position = (int)SendMessage(heplistbox, LB_INSERTSTRING, 0, (LPARAM)episodes[i].title);
                                if (position == LB_ERR || position == LB_ERRSPACE) {
                                    printf("\n\n\nerro do capeta: %lu", GetLastError());
                                    break;
                                } else {
                                    printf("\n\nfez com sucesso");
                                }
                                SendMessage(heplistbox, LB_SETITEMDATA, position, (LPARAM)i);
                                
                            }*/
                        }          
                        return 0;
                        
                    
                    }
                    break;
                }
                
            
                default:
                    break;
            }
            return 0;
        }  
            
        case WM_PAINT: {
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_3DFACE+1));
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            
            return 0;

        default:
            return DefWindowProc(hwnd, message, wparam, lparam);
    }
}

BOOL CALLBACK SetFontProc(HWND hwnd, LPARAM lparam) {
    SendMessage(hwnd, WM_SETFONT, 0, lparam);
    return TRUE;
}

BOOL CALLBACK SetBackgroundColorProc(HWND hwnd, LPARAM lparam) {
    SetBkColor((HDC)lparam, GetSysColor(COLOR_WINDOW));
    return TRUE;
}

void initializeTheming() { 
    InitCommonControls();
    EnumChildWindows(hwndmain, (WNDENUMPROC)SetFontProc, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));
    EnumChildWindows(hwndsettings, (WNDENUMPROC)SetFontProc, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));
    EnumChildWindows(hwndsearch, (WNDENUMPROC)SetFontProc, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));
}

void showNetworkTab() {
    ShowWindow(hwndnetworktab, SW_SHOW);
    ShowWindow(hwndprovidertab, SW_HIDE);
    ShowWindow(hwndlangtab, SW_HIDE);
    UpdateWindow(hwndnetworktab);
    UpdateWindow(hwndprovidertab);
    UpdateWindow(hwndlangtab);
}

void showProviderTab() {
    ShowWindow(hwndnetworktab, SW_HIDE);
    ShowWindow(hwndprovidertab, SW_SHOW);
    ShowWindow(hwndlangtab, SW_HIDE);
    UpdateWindow(hwndnetworktab);
    UpdateWindow(hwndprovidertab);
    UpdateWindow(hwndlangtab);
}

void showLanguageTab() {
    ShowWindow(hwndnetworktab, SW_HIDE);
    ShowWindow(hwndprovidertab, SW_HIDE);
    ShowWindow(hwndlangtab, SW_SHOW);
    UpdateWindow(hwndnetworktab);
    UpdateWindow(hwndprovidertab);
    UpdateWindow(hwndlangtab);
}





