#include "tools.h"
#include "minunit.h"

/*#include "libvlc/include/vlc/vlc.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//janelas
HWND hwndmain;
HWND hwndsearch;
HWND hwndinfo;
HWND hwndsettings;
HWND hwndvideo;z

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
extern wchar_t * videolink;

extern int x, y, c;
extern unsigned char * imgdata;

DWORD wversion, wmajorversion, wminorversion, wbuild;

int createSettingsWindow(HINSTANCE hinstance);
int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, PWSTR lpcmdline, int nshowcmd);*/

animeinfo test;

wchar_t * jsonresult = L"{\"id\":\"angel-beats\",\"title\":\"Angel Beats\",\"url\\:\"\"https://anitaku.so/category/angel-beats\",\"genres\":[\"Action\",\"Comedy\",\"Drama\",\"Supernatural\"],\"totalEpisodes\":15,\"image\":\"https://gogocdn.net/images/anime/angel-beats.jpg\",\"releaseDate\":\"2010\",\"description\":\"In a world ...";
wchar_t * jsonresult2 = L"{\"headers\":{\"Referer\":\"https://s3taku.com/embedplus?id=MTQ4Mg==&token=LH-xKtasFomVgpMf_KwvqQ&expires=1724543722\"},\"sources\":[{\"url\":\"https://www118.anzeat.pro/streamhls/20573f92bdb131fae79f44e00242dd51/ep.1.1709230302.360.m3u8\",\"isM3U8\":true,\"quality\":\"360p\"},{\"url\":\"https://www118.anzeat.pro/streamhls/20573f92bdb131fae79f44e00242dd51/ep.1.1709230302.480.m3u8\",\"isM3U8\":true,\"quality\":\"480p\"},{\"url\":\"https://www118.anzeat.pro/streamhls/20573f92bdb131fae79f44e00242dd51/ep.1.1709230302.720.m3u8\",\"isM3U8\":true,\"quality\":\"720p\"},{\"url\":\"https://www118.anzeat.pro/streamhls/20573f92bdb131fae79f44e00242dd51/ep.1.1709230302.1080.m3u8\",\"isM3U8\":true,\"quality\":\"1080p\"},{\"url\":\"https://www118.anzeat.pro/streamhls/20573f92bdb131fae79f44e00242dd51/ep.1.1709230302.m3u8\",\"isM3U8\":true,\"quality\":\"default\"},{\"url\":\"https://www118.anicdnstream.info/videos/hls/yzD4zs6LqpvJKAZ_dHg1PA/1724550923/1482/20573f92bdb131fae79f44e00242dd51/ep.1.1709230302.m3u8\",\"isM3U8\":true,\"quality\":\"backup\"}],\"download\":\"https://s3taku.com/download?id=MTQ4Mg==&token=LH-xKtasFomVgpMf_KwvqQ&expires=1724543722\"}";

wchar_t * controlstring5 = L"Action";
const wchar_t * testkey = L"\"genres\":[\"";

int controlint = 15;
const wchar_t * testkey2 = L"\"totalEpisodes\":";

HWND hwnd;

wchar_t * controlstring6 = L"https://www118.anzeat.pro/streamhls/20573f92bdb131fae79f44e00242dd51/ep.1.1709230302.360.m3u8";
char * controlstring7 = "https://www118.anzeat.pro/streamhls/20573f92bdb131fae79f44e00242dd51/ep.1.1709230302.360.m3u8";

void test_setup(void) {
	//Nothing 
}

void test_teardown(void) {
	//Nothing 
}

/*MU_TEST(t_getepisodesnum) {
	int result = getepisodesnum(jsonresult, epnumkey);
	mu_assert_int_eq(23, result);
}

MU_TEST(t_parseresultsjson) {
	int result = parseresultsjson(jsonsearch, testresults);
	mu_assert_int_eq(6, result);
}

MU_TEST(t_searchconnection) {
	int result = searchConnection(testhwnd, searchquery, testresults);
	mu_assert_int_eq(5, result);
}

//função reajustada para retornar um long pointer string
MU_TEST(t_tparserequesttext) {
	LPSTR result = TparseRequestText(testuri, showid);
	mu_assert_string_eq(controlstring2, result);
}

MU_TEST(t_episodesconnection) {
	int result = episodesConnection(testhwnd, showid, testepisodes);
	mu_assert_int_eq(23, result);
}

MU_TEST(t_epnumconnection) {
	int result = epnumConnection(testhwnd, showid);
	mu_assert_int_eq(23, result);
}

//função reajustada devido ao hash do url não ser estático
MU_TEST(t_teplinkconnection) {
	int check = 0;
	char * result = TeplinkConnection(testhwnd, episodeid);

	if (result != NULL)
		if (strstr(result, "{\"sources\":[{\"url\":") != NULL)
			check = 1;

	mu_assert_int_eq(1, check);
}

MU_TEST(t_getlinkjson) {
	char * result = getlinkjson(testhwnd, jsonresult);
	mu_assert_string_eq(controlstring3, result);
}*/

MU_TEST(t_getgenres) {
	getGenres(jsonresult, testkey, test.genres);
	int check = wcscmp(controlstring5, test.genres[0]);
	mu_assert_int_eq(0, check);
}

MU_TEST(t_getlinkjson) {
	wchar_t * a = getLinkJson(hwnd, jsonresult2);
	int check = wcscmp(controlstring6, a);
	mu_assert_int_eq(0, check);
}

MU_TEST(t_getepisodesnum) {
	int check = getEpisodesNum(jsonresult, testkey2);
	//int check = wcscmp(controlstring6, a);
	mu_assert_int_eq(15, check);
}

MU_TEST(t_convertwidetomulti) {
	wchar_t * a = getLinkJson(hwnd, jsonresult2);
	//int check = wcscmp(controlstring6, a);

	printf("\n\na: %ls\\", a);

	char * test = (char *)malloc(wcslen(controlstring6) * sizeof(char));
	int check = convertWideToMulti(a, test);

	printf("\n\ntest string: %s\\", test);

	int aux = strcmp(test, controlstring7);
	printf("aux: %d", aux);

	if (aux) {
		check = 0;
	} else {
		check = 1;
	}
	
	//int check = wcscmp(controlstring6, a);
	mu_assert_int_eq(1, check);
}

/*MU_TEST(t_winhttpgetresponse) {
	int check;
	char * result = NULL;

	parseRequestText("/anime/zoro/", "Clannad");
	hsession = WinHttpOpen(L"Miru-win32/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (hsession == NULL)
		check = 0;
	else {
		dwprotocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;
		WinHttpSetOption(hsession, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwprotocols, sizeof(dwprotocols));
		hconnect = WinHttpConnect(hsession, L"consumet-one-sigma.vercel.app", INTERNET_DEFAULT_PORT, 0);
		if (hconnect == NULL)
			check = 0;
		else {
			hrequest = WinHttpOpenRequest(hconnect, L"GET", requestedquery, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
			if (hrequest == NULL)
				check = 0;
			else {
				if (WinHttpSendRequest(hrequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
					if (WinHttpReceiveResponse(hrequest, NULL))
						result = winHttpGetResponse(hrequest, hconnect, hsession);
					else
						check = 0;
				else
					check = 0;
			}
		}
	}
	
	if (result != NULL)
		check = 1;

	mu_assert_int_eq(1, check);
}*/

MU_TEST_SUITE(test_suite) {
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

	/*MU_RUN_TEST(t_getepisodesnum);
	MU_RUN_TEST(t_parseresultsjson);
	MU_RUN_TEST(t_searchconnection);
	MU_RUN_TEST(t_tparserequesttext);
	MU_RUN_TEST(t_episodesconnection);
	MU_RUN_TEST(t_epnumconnection);
	MU_RUN_TEST(t_teplinkconnection);
	MU_RUN_TEST(t_getlinkjson);*/
	MU_RUN_TEST(t_getgenres);
	MU_RUN_TEST(t_getlinkjson);
	MU_RUN_TEST(t_getepisodesnum);
	MU_RUN_TEST(t_convertwidetomulti);
	//MU_RUN_TEST(t_winhttpgetresponse);
}

int wmain(int argc, char const *argv[]) {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}

/*TESTSUITE_BEGIN

TEST_BEGIN("t_getgenres")
	getGenres(jsonresult, testkey, test.genres);
	CHECK(wcscmp(controlstring5, test.genres[0]));
TEST_END

TESTSUITE_END*/

