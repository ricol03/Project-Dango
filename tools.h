

#define NO_DSHOW_STRSAFE

#ifdef _WIN32_WINNT_WIN2K
//#include "connections_prexp.h"
//#include <wininet.h>
#else
#include "connections.h"
#include <winhttp.h>
#endif


#ifndef TOOLS_H_
#define TOOLS_H_



//system headers
#include <stdio.h>
//#include <winsock2.h>
#include <Windows.h>
#include <locale.h>
#include <wingdi.h>
#include <string.h>
//#include <dshow.h>
//#include <mfapi.h>
//#include <mfidl.h>
//#include <mferror.h>
#include <tchar.h>
#include <commctrl.h>
#include <process.h>
#include <shlobj.h>

//external headers
#include "mut.h"

//project headers
#include "const.h"
#include "main.h"
#include "wmcreate.h"

#include "video.h"
#include "json.h"
#include "file.h"
#include "lang_en.h"
#include "provider.h"

#endif
