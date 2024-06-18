#include "tools.h"

//TODO: meter todos os episódios na listbox de baixo X
//TODO: quando se clica no botão watch, meter o link do episódio no libvlc X
//TODO:: meter as legendas da coisa
//TODO:: fazer um menu toggable na janela do vídeo
//TODO:: melhorar o ecrã inicial (com conteúdo em trending e etc)

//FIXME: linha 223 do connections.c, segmentation fault depois de strstr com episodekey X

//BUG: alguns dos links não funcionam, mesmo funcionando no browser X
//BUG: quando reproduzimos um vídeo, e voltamos à janela para escolher outro episódio, a janela anterior não fecha 
//BUG: alguns resultados (até agora os de conteúdo único) não têm link associado


#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

const wchar_t CLASS_NAME[]      = L"Window Class";
const wchar_t SEARCH_CLASS[]    = L"SearchWndClass";
const wchar_t SETTINGS_CLASS[]  = L"SettingsWndClass";

//janelas
HWND hwndmain;
HWND hwndsearch;
HWND hwndsettings;

//controlos do home
extern HWND hbutton, hsearchbutton;            
extern HWND htext;
extern HWND hsearchlabel;              
extern HWND hsearchbox;  
extern HWND hsearchbutton;

extern HBRUSH hbrush;

//controlos dos resultados de pesquisa
extern HWND hshowlistbox, heplistbox;
extern HWND hwatchbutton;

//controlos da janela de vídeo
extern HWND hvidwindow;

//menu da janela
extern HMENU hmenu;
extern HMENU hsubmenusearch;  

//bool para a pesquisa estar vísivel ou não
boolean searchtoggled = FALSE;

extern const char* localfile;
extern result results[];
extern episode episodes[];

int number;

PAINTSTRUCT ps;

LRESULT CALLBACK WindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

LRESULT SearchWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

LRESULT SettingsWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, PSTR lpcmdline, int nshowcmd) {

    WNDCLASS wc = { 0 };

    wc.lpfnWndProc      = WindowProc;
    wc.hInstance        = hinstance;
    wc.lpszClassName    = (LPCSTR)CLASS_NAME;

    RegisterClass(&wc);

    hwndmain = CreateWindowEx(
        0,
        CLASS_NAME,
        "Windows",
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_ICONIC | WS_ACTIVECAPTION | WS_VISIBLE,
        MAINWINDOWWIDTH, MAINWINDOWHEIGHT, MAINWINDOWWIDTH, MAINWINDOWHEIGHT,
        NULL,
        NULL,
        hinstance,
        NULL
    );

    ShowWindow(hwndmain, SW_SHOW);
    UpdateWindow(hwndmain);
    
    if (hwndmain == NULL) {
        MessageBoxW(NULL, L"Unable to create Windows", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }
    else
        ShowWindow(hwndmain, SW_SHOW);

    WNDCLASS wc2 = { 0 };

    wc2.lpfnWndProc      = SearchWindowProc;
    wc2.hInstance        = hinstance;
    wc2.lpszClassName    = (LPCSTR)SEARCH_CLASS;

    RegisterClass(&wc2);

    hwndsearch = CreateWindowEx(
        0,
        SEARCH_CLASS,
        "Search",
        WS_CAPTION | WS_SYSMENU | WS_ACTIVECAPTION | WS_VISIBLE,
        SEARCHWINDOWWIDTH, SEARCHWINDOWHEIGHT, SEARCHWINDOWWIDTH, SEARCHWINDOWHEIGHT,
        NULL,
        NULL,
        hinstance,
        NULL
    );
    
    if (hwndsearch == NULL) {
        MessageBoxW(NULL, L"Unable to create search window", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    WNDCLASS wc3 = { 0 };

    wc3.lpfnWndProc      = SettingsWindowProc;
    wc3.hInstance        = hinstance;
    wc3.lpszClassName    = (LPCSTR)SETTINGS_CLASS;

    RegisterClass(&wc3);

    hwndsettings = CreateWindowEx(
        0,
        SETTINGS_CLASS,
        "Settings",
        WS_CAPTION | WS_SYSMENU | WS_ACTIVECAPTION | WS_VISIBLE,
        SETTINGSWINDOWWIDTH, SETTINGSWINDOWHEIGHT, SETTINGSWINDOWWIDTH, SETTINGSWINDOWHEIGHT,
        NULL,
        NULL,
        hinstance,
        NULL
    );
    
    if (hwndsettings == NULL) {
        MessageBoxW(NULL, L"Unable to create settings window", L"Error", MB_ICONERROR | MB_OK);
        return 0;
    }
    
    MSG msg = { 0 };

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }  

    return 0;
}

LRESULT CALLBACK WindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    
    switch (message) {
        case WM_CREATE:
            createUtils();
            homeWindow(hwnd);
            
            return 0;

        case WM_COMMAND:
            switch (LOWORD(wparam)) {
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
                    MessageBox(hwnd, "Foi clicado", "Info", MB_ICONEXCLAMATION | MB_OK);
                    break;
                }
                 
                case 201: {
                    int item = (int)SendMessage(heplistbox, LB_GETCURSEL, 0, 0);
                    int i = (int)SendMessage(heplistbox, LB_GETITEMDATA, item, 0);

                    char * link = eplinkConnection(hwnd, episodes[i].id);
                    printf("\n\n\ndevolveu este link zeca: %s", link);

                    initializeLink(hwnd, link);
                    _beginthread(videoWindowWrapper, 0, NULL);
                    //printf("e saiu do videowindow");
                    break;
                }   
                    

              
                case 303: {
                    switch (HIWORD(wparam)) {
                        case LBN_SELCHANGE: {
                            memset(&episodes, 0, sizeof(episode));
                            int item = (int)SendMessage(hshowlistbox, LB_GETCURSEL, 0, 0);
                            int i = (int)SendMessage(hshowlistbox, LB_GETITEMDATA, item, 0);
                            MessageBox(hwnd, results[i].id, "Info", MB_ICONINFORMATION);

                            number = episodesConnection(hwnd, results[i].id, episodes);

                            for (int i = 0; i < number; i++) {
                                printf("\n%s", episodes[i].id);
                            }
                             
                            SendMessage(heplistbox, LB_RESETCONTENT, 0, 0);
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
                                
                            }
                        }
                                           
                            return 0;
                        
                        default:
                            break;
                    }
                    break;
                }

                case 304: {
                    switch (HIWORD(wparam)) {
                        case LBN_SELCHANGE: {
                            
                            int item = (int)SendMessage(heplistbox, LB_GETCURSEL, 0, 0);
                            int i = (int)SendMessage(heplistbox, LB_GETITEMDATA, item, 0);
                            MessageBox(hwnd, episodes[i].id, "Info", MB_ICONINFORMATION);

                            

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
                        
                        default:
                            break;
                    }
                    break;
                }


                default:    
                    break;
                }
            
            return 0;

        case WM_PAINT: {
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_3DFACE+1));

            EndPaint(hwnd, &ps);
        }
            return 0;

        case WM_DESTROY:
            
            return 0;
    }
    return DefWindowProc(hwnd, message, wparam, lparam);
}

void videoWindowWrapper(void* data) {
    videoWindowMain(hvidwindow, GetModuleHandle(NULL));
}

LRESULT SearchWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_CREATE:
            searchWindow(hwnd);
            return 0;

        case WM_COMMAND: {
            switch (LOWORD(wparam)) {
                case 202: {
                    char searchquery[256];
                    GetWindowText(hsearchbox, searchquery, sizeof(searchquery));
                    if (searchquery != NULL) {
                        //ShowWindow(hwnd, SW_HIDE);
                        searchtoggled = hideSearch();
                        if (searchResults(hwndmain, searchquery) == 0) {
                            homeWindow(hwndmain);
                        }
                    }
                    break;
		        } 

                case 203: {
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

LRESULT SettingsWindowProc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        /*case WM_CREATE:
            searchWindow(hwnd);
            return 0;

        case WM_COMMAND: {
            switch (LOWORD(wparam)) {
                case 202: {
                    char searchquery[256];
                    GetWindowText(hsearchbox, searchquery, sizeof(searchquery));
                    if (searchquery != NULL) {
                        //ShowWindow(hwnd, SW_HIDE);
                        searchtoggled = hideSearch();
                        if (searchResults(hwndmain, searchquery) == 0) {
                            homeWindow(hwndmain);
                        }
                    }
                    break;
		        } 

                case 203: {
                    searchtoggled = hideSearch();
                    break;
		        } 
            
                default:
                    break;
            }
            return 0;
        }  */
            
        case WM_PAINT: {
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_3DFACE+1));

            EndPaint(hwnd, &ps);
        
            return 0;
        }

        case WM_CLOSE:
        case WM_DESTROY:
            settingstoggled = hideSettings();
            return 0;

        default:
            return DefWindowProc(hwnd, message, wparam, lparam);
    }
}