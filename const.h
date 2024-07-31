/*
    ORDER OF NUMBERS:
    - 1x - 2x:      menus
    - 1xx:          labels
    - 2xx:          buttons
    - 3xx:          listboxes
    - 4xx:          textboxes (edit)
    - 5xx:          checkboxes
    - 6xx:          radio buttons
    - 7xx:          comboboxes
    - 7xx:          toggles
    - 9xx:          test


*/
#include <Windows.h>

#ifndef CONST_H_
#define CONST_H_

#define SETTINGSFILENAME "settings.bin"
#define TESTIMAGE "test.jpg"


#define MAINWINDOWHEIGHT 500
#define MAINWINDOWWIDTH 600

#define SEARCHWINDOWHEIGHT 200
#define SEARCHWINDOWWIDTH 400

#define SETTINGSWINDOWHEIGHT 250
#define SETTINGSWINDOWWIDTH 400

#define IDM_FILE_HOME                        10
#define IDM_FILE_CLOSE                       11

/*-------------------------------*/

#define IDM_SEARCH_SEARCHBOX                 12

/*-------------------------------*/

#define IDM_SETTINGS_SETTINGS                13

/*-------------------------------*/

#define IDM_ABOUT_HELP                       14
#define IDM_ABOUT_ABOUT                      15

/*-------------------------------*/

#define IDW_MAIN_LABEL_TITLE                100
#define IDW_MAIN_LABEL_UNAVAILABLETRENDING  101

#define IDW_PROVIDER_LABEL_WARNING          120


#define IDW_MAIN_BUTTON_SEARCH              200
#define IDW_SEARCH_BUTTON_WATCH             201
#define IDW_SEARCH_BUTTON_SEARCH            202
#define IDW_SEARCH_BUTTON_CANCEL            203
#define IDW_SETTINGS_BUTTON_APPLY           204

#define IDW_SETTINGS_BUTTON_OK              210
#define IDW_SETTINGS_BUTTON_CANCEL          211

#define IDW_SEARCH_LISTBOX_SHOW             303
#define IDW_SEARCH_LISTBOX_EPISODELIST      304

#define IDW_SEARCH_EDIT_SEARCH              402
#define IDW_NETWORK_EDIT_PORT               444

#define IDW_NETWORK_CHECKBOX_PORT           500

#define IDW_NETWORK_RADIO_HTTP              600
#define IDW_NETWORK_RADIO_HTTPS             601



#define IDW_SETTINGS_COMBOBOX_PROVIDERLIST  700
#define IDW_SETTINGS_COMBOBOX_LANGLIST      701

#define IDW_VIDEO_SYS_TOGGLE                800


#define ID_TEST 9999



#endif