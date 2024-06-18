#define NO_DSHOW_STRSAFE

//system headers
#include <stdio.h>
#include <winsock2.h>
#include <winhttp.h>
#include <Windows.h>
#include <wingdi.h>
#include <string.h>
#include <dshow.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>
#include <tchar.h>

#include <process.h>

//project headers
#include "const.h"
#include "main.h"
#include "wmcreate.h"
#include "connections.h"
#include "video.h"
#include "json.h"

//minunit header
//#include "minunit.h"