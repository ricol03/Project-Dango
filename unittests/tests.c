#include "tools.h"
#include "minunit.h"

char* jsonresult = "{\"id\":\"clannad-418\",\"title\":\"Clannad\",\"malID\":2167,\"alID\":2167,\"japaneseTitle\":\"\",\"image\":\"...\",\"description\":\"...\",\"type\":\"TV\",\"url\":\"https://hianime.to/clannad-418\",\"recommendations\":[...],\"relatedAnime\":[...],\"subOrDub\":\"both\",\"hasSub\":true,\"hasDub\":true,\"totalEpisodes\":23,\"episodes\":[...]}";
const char* epnumkey = "\"totalEpisodes\":";

char* jsonsearch = "{\"currentPage\":1,\"hasNextPage\":false,\"totalPages\":1,\"results\":[{\"id\":\"kiss-x-sis-3800\",\"title\":\"Kiss x Sis\",\"url\":\"...\",\"duration\":\"24m\",\"japaneseTitle\":\"Kiss x Sis\",\"type\":\"OVA\",\"nsfw\":false,\"sub\":12,\"dub\":0,\"episodes\":12},{\"id\":\"kiss-x-sis-tv-4712\",\"title\":\"Kiss x Sis (TV)\",\"url\":\"...\",\"duration\":\"24m\",\"japaneseTitle\":\"Kiss x Sis (TV)\",\"type\":\"TV\",\"nsfw\":false,\"sub\":12,\"dub\":0,\"episodes\":12},{\"id\":\"itakiss-1398\",\"title\":\"ItaKiss\",\"url\":\"...\",\"image\":\"...\",\"duration\":\"24m\",\"japaneseTitle\":\"Itazura na Kiss\",\"type\":\"TV\",\"nsfw\":false,\"sub\":25,\"dub\":0,\"episodes\":25},{\"id\":\"engage-kiss-18082\",\"title\":\"Engage Kiss\",\"url\":\"...\",\"image\":\"...\",\"duration\":\"23m\",\"japaneseTitle\":\"Engage Kiss\",\"type\":\"TV\",\"nsfw\":false,\"sub\":13,\"dub\":13,\"episodes\":13},{\"id\":\"paradise-kiss-758\",\"title\":\"Paradise Kiss\",\"url\":\"...\",\"image\":\"...\",\"duration\":\"24m\",\"japaneseTitle\":\"Paradise Kiss\",\"type\":\"TV\",\"nsfw\":false,\"sub\":12,\"dub\":12,\"episodes\":12},{\"id\":\"sono-hanabira-ni-kuchizuke-wo-risa-x-miya-gekijou-6566\",\"title\":\"Sono Hanabira ni Kuchizuke wo: Risa x Miya Gekijou\",\"url\":\"...\",\"image\":\"...\",\"duration\":\"10m\",\"japaneseTitle\":\"Sono Hanabira ni Kuchizuke wo: Risa x Miya Gekijou\",\"type\":\"ONA\",\"nsfw\":false,\"sub\":10,\"dub\":0,\"episodes\":10}}";

char * searchquery = "clannad";
char* controlstring = "{\"currentPage\":null,\"hasNextPage\":false,\"totalPages\":null,\"results\":[{\"id\":\"clannad-the-movie-2553\",\"title\":\"Clannad: The Movie\",\"url\":\"https://hianime.to/clannad-the-movie-2553?ref=search\",\"image\":\"https://cdn.noitatnemucod.net/thumbnail/300x400/100/146185b19062e463b04c0b0f88fcbfdb.jpg\",\"duration\":\"93m\",\"japaneseTitle\":\"Clannad Movie\",\"type\":\"Movie\",\"nsfw\":false,\"sub\":1,\"dub\":1,\"episodes\":0},{\"id\":\"clannad-418\",\"title\":\"Clannad\",\"url\":\"https://hianime.to/clannad-418?ref=search\",\"image\":\"https://cdn.noitatnemucod.net/thumbnail/300x400/100/033c07868e7fe2c69d216db13dd3a886.jpg\",\"duration\":\"24m\",\"japaneseTitle\":\"Clannad\",\"type\":\"TV\",\"nsfw\":false,\"sub\":23,\"dub\":23,\"episodes\":23},{\"id\":\"clannad-another-world-tomoyo-chapter-516\",\"title\":\"Clannad: Another World, Tomoyo Chapter\",\"url\":\"https://hianime.to/clannad-another-world-tomoyo-chapter-516?ref=search\",\"image\":\"https://cdn.noitatnemucod.net/thumbnail/300x400/100/205f891c94de306383c57f7664cc944d.jpg\",\"duration\":\"25m\",\"japaneseTitle\":\"Clannad: Mou Hitotsu no Sekai, Tomoyo-hen\",\"type\":\"Special\",\"nsfw\":false,\"sub\":1,\"dub\":1,\"episodes\":0},{\"id\":\"clannad-after-story-another-world-kyou-chapter-690\",\"title\":\"Clannad ~After Story~: Another World, Kyou Chapter\",\"url\":\"https://hianime.to/clannad-after-story-another-world-kyou-chapter-690?ref=search\",\"image\":\"https://cdn.noitatnemucod.net/thumbnail/300x400/100/b64746bb3104a3c3f874f3e7a50ceac5.jpg\",\"duration\":\"24m\",\"japaneseTitle\":\"Clannad: After Story - Mou Hitotsu no Sekai, Kyou-hen\",\"type\":\"Special\",\"nsfw\":false,\"sub\":1,\"dub\":1,\"episodes\":0},{\"id\":\"clannad-after-story-15\",\"title\":\"Clannad ~After Story~\",\"url\":\"https://hianime.to/clannad-after-story-15?ref=search\",\"image\":\"https://cdn.noitatnemucod.net/thumbnail/300x400/100/04e65044c0d9637367efe0cddf7dd205.jpg\",\"duration\":\"24m\",\"japaneseTitle\":\"Clannad: After Story\",\"type\":\"TV\",\"nsfw\":false,\"sub\":24,\"dub\":24,\"episodes\":24}]}";

char * testuri = "/anime/zoro/info?id=";
char * showid = "clannad-418";

char * episodeid = "clannad-418$episode$9184$both";

char * controlstring2 = "/anime/zoro/info?id=clannad-418";
char * controlstring3 = "https://hianime.to/clannad-418";

const char* testkey = "\"title\":\"";
char * controlstring4 = "Clannad\",\"malID\":2167,\"alID\":2167,\"japaneseTitle\":\"\",\"image\":\"...\",\"description\":\"...\",\"type\":\"TV\",\"url\":\"https://hianime.to/clannad-418\",\"recommendations\":[...],\"relatedAnime\":[...],\"subOrDub\":\"both\",\"hasSub\":true,\"hasDub\":true,\"totalEpisodes\":23,\"episodes\":[...]}";

result testresults[8];
episode testepisodes[100];
HWND testhwnd;

HINTERNET hsession, hconnect, hrequest;
DWORD dwprotocols;
extern LPSTR requestedquery;

void test_setup(void) {
	/* Nothing */
}

void test_teardown(void) {
	/* Nothing */
}

MU_TEST(t_getepisodesnum) {
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
}

MU_TEST(t_getjsonid) {
	connections result = getjsonid(jsonresult, testkey, testresults[0].title, 0);
	mu_assert_string_eq(controlstring4, result.value);
}

MU_TEST(t_winhttpgetresponse) {
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
}

MU_TEST_SUITE(test_suite) {
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

	MU_RUN_TEST(t_getepisodesnum);
	MU_RUN_TEST(t_parseresultsjson);
	MU_RUN_TEST(t_searchconnection);
	MU_RUN_TEST(t_tparserequesttext);
	MU_RUN_TEST(t_episodesconnection);
	MU_RUN_TEST(t_epnumconnection);
	MU_RUN_TEST(t_teplinkconnection);
	MU_RUN_TEST(t_getlinkjson);
	MU_RUN_TEST(t_getjsonid);
	MU_RUN_TEST(t_winhttpgetresponse);
}

int main(int argc, char const *argv[]) {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}