#include "tools.h"
#include "libvlc/include/vlc/vlc.h"

extern PAINTSTRUCT ps;

libvlc_instance_t* linst;
libvlc_media_player_t* mplay;
libvlc_media_t* media;

HDC hdc;

char * videolink;

/*LRESULT CALLBACK videoWindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

HWND videoWindowMain(HWND hwnd, HINSTANCE hinstance) {
    const wchar_t VIDEO_CLASS[] = L"Video Window";

    WNDCLASS videowindowclass = { 0 };

    videowindowclass.lpfnWndProc      = videoWindowProc;
    videowindowclass.hInstance        = hinstance;
    videowindowclass.lpszClassName    = (LPCSTR)VIDEO_CLASS;

    RegisterClass(&videowindowclass);

    hwndvideo = CreateWindowEx(
        0,
        VIDEO_CLASS,
        "Video",
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_ICONIC | WS_ACTIVECAPTION | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        NULL,
        NULL,
        hinstance,
        NULL 
    );

    ShowWindow(hwndvideo, SW_SHOW);
    UpdateWindow(hwndvideo);

    MSG msg = { 0 };

    if (hwndvideo == NULL) {
        MessageBox(hwnd, "Couldn't create video window", "Error", MB_ICONERROR);
        return 0;
    }
    
    while (GetMessage(&msg, hwndvideo, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    

    return hwndvideo;
}

LRESULT CALLBACK videoWindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    
    switch (message) {
        case WM_CREATE: {
            

            //printf("\n\n\n\n\n\n\nprint do videolink: %s", videolink);
            linst = libvlc_new(0, NULL);

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

                /*MSG msg;
                while (GetMessage(&msg, NULL, 0, 0)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }*/

                /*getchar();
                printf("char");*/

                /*libvlc_media_player_stop(mplay);
                printf("Ele fez esta parte do stop");
                libvlc_media_player_release(mplay);
                printf("Ele fez esta parte do release 1");

                libvlc_release(linst);
                printf("Ele fez esta parte do release 2");

                return 0;
            } else {
                MessageBox(hwnd, "No instance could be created", "Error", MB_ICONERROR);
                return -1;
            }
            AppendMenu(GetSystemMenu(hwnd, FALSE), MF_SEPARATOR, 0, NULL);
            AppendMenu(GetSystemMenu(hwnd, FALSE), MF_STRING, IDW_VIDEO_SYS_TOGGLE, "Control menu");
        }
        break;
        
        case WM_SYSCOMMAND:
            switch (wparam) {
                case IDW_VIDEO_SYS_TOGGLE:
                    MessageBox(hwnd, "Test", "Test", MB_ICONEXCLAMATION);
                    break;
                
                default:
                    break;
            }

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            
            EndPaint(hwnd, &ps);
	        
            break;

        case WM_DESTROY:
            printf("este man saiu me daqui");
            libvlc_media_player_release(mplay);
            libvlc_release(linst);
            _endthread();
            printf("destruiu tudo");
            break;

        default:
            break;
    }

    return DefWindowProc(hwnd, message, wparam, lparam);
}*/

int initializeLink(HWND hwnd, char * link) {
    //strcpy(videolink, link);

    printf("\n\nvideolink: %s", videolink);

    return 1;
}

int videoWindow(HWND hwnd, char * link) {

    printf("\n\n\n\n\neste é o link saboroso do coisinho: %s", link);

    linst = libvlc_new(0, NULL);

    if (linst != NULL) {
        media = libvlc_media_new_location(linst, link);
        printf("este é o link saboroso do coisinho: %s", link);

        mplay = libvlc_media_player_new_from_media(media);
        printf("Ele fez esta parte do new from media");

        libvlc_media_release(media);
        printf("Ele fez esta parte do media release");

        libvlc_media_player_set_hwnd(mplay, hwnd);
        printf("Ele fez esta parte do hwnd");

        libvlc_media_player_play(mplay);
        printf("Ele fez esta parte do play");

        /*MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }*/

        /*getchar();
        printf("char");*/

        /*libvlc_media_player_stop(mplay);
        printf("Ele fez esta parte do stop");
        libvlc_media_player_release(mplay);
        printf("Ele fez esta parte do release 1");

        libvlc_release(linst);
        printf("Ele fez esta parte do release 2");*/

        return 0;
    }
    
    else {
        MessageBox(hwnd, "No instance could be created", "Error", MB_ICONERROR);
        return -1;
    }

}

/*#include <windows.h>
//#include <vlc/vlc.h>
#include <stdio.h>
#include <stdlib.h>

HWND hwndvideo;
PAINTSTRUCT ps;

libvlc_instance_t* linst;
libvlc_media_player_t* mplay;
libvlc_media_t* media;

HDC hdc;

char * videolink = NULL;

LRESULT videoWindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
HWND videoWindowMain(HWND hwnd, HINSTANCE hinstance);
int videoWindow(HWND hwnd, char * link);
int initializeLink(HWND hwnd, char * link);

LRESULT videoWindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_CREATE:
            printf("Creating video window...\n");
            videoWindow(hwnd, videolink);
            AppendMenu(GetSystemMenu(hwnd, FALSE), MF_SEPARATOR, 0, NULL);
            AppendMenu(GetSystemMenu(hwnd, FALSE), MF_STRING, 1, "Control menu");
            break;
        
        case WM_SYSCOMMAND:
            if (wparam == 1) {
                MessageBox(hwnd, "Test", "Test", MB_ICONEXCLAMATION);
            }
            break;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            break;

        case WM_DESTROY:
            printf("Destroying video window...\n");
            libvlc_media_player_stop(mplay);
            libvlc_media_player_release(mplay);
            libvlc_release(linst);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, message, wparam, lparam);
    }
    return 0;
}

HWND videoWindowMain(HWND hwnd, HINSTANCE hinstance) {
    const wchar_t CLASS_NAME[] = L"Video Window";

    WNDCLASS wc = { 0 };
    wc.hInstance = hinstance;
    wc.lpfnWndProc = videoWindowProc;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    hwndvideo = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Video",
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        NULL,
        NULL,
        hinstance,
        NULL 
    );

    if (hwndvideo == NULL) {
        MessageBox(hwnd, "Couldn't create video window", "Error", MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwndvideo, SW_SHOW);
    UpdateWindow(hwndvideo);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return hwndvideo;
}

int initializeLink(HWND hwnd, char * link) {
    videolink = malloc(512);
    if (videolink == NULL) {
        MessageBox(hwnd, "Memory allocation failed", "Error", MB_ICONERROR);
        return -1;
    }
    strncpy(videolink, link, 511);
    videolink[511] = '\0';
    return 1;
}

int videoWindow(HWND hwnd, char * link) {
    printf("Initializing libVLC...\n");

    linst = libvlc_new(0, NULL);
    if (linst == NULL) {
        MessageBox(hwnd, "LibVLC initialization failed", "Error", MB_ICONERROR);
        return -1;
    }

    media = libvlc_media_new_location(linst, link);
    if (media == NULL) {
        MessageBox(hwnd, "Media loading failed", "Error", MB_ICONERROR);
        libvlc_release(linst);
        return -1;
    }

    mplay = libvlc_media_player_new_from_media(media);
    libvlc_media_release(media);
    if (mplay == NULL) {
        MessageBox(hwnd, "Media player creation failed", "Error", MB_ICONERROR);
        libvlc_release(linst);
        return -1;
    }

    libvlc_media_player_set_hwnd(mplay, hwnd);
    if (libvlc_media_player_play(mplay) == -1) {
        MessageBox(hwnd, "Failed to play media", "Error", MB_ICONERROR);
        libvlc_media_player_release(mplay);
        libvlc_release(linst);
        return -1;
    }

    printf("Video is playing...\n");
    return 0;
}*/
