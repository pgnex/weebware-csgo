#include "Heartbeat.h"

#define CURL_STATICLIB
#include "curl/curl.h"
#pragma comment (lib, "curl/libcurl_a.lib")


using namespace std::chrono;
__int64 auth::GetEpochMS() {
	__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return now;
}

__int64 auth::GetEpochS() {
	__int64 now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return now;
}


static int writer(char *data, size_t size, size_t nmemb, std::string *writerData) {
	if (writerData == NULL) return 0;
	writerData->append(data, size*nmemb);
	return size * nmemb;
}


std::string auth::GetServerVariable(std::string key) {
	int timestamp = GetEpochS();
	std::string content;
	curl_global_init(CURL_GLOBAL_ALL);
	CURL *curl = nullptr;
	curl = curl_easy_init();
	if (curl) {
		std::string requestData = ("key=") + key;
		curl_easy_setopt(curl, CURLOPT_URL, base64_decode("aHR0cHM6Ly9hdXRoLndlZWJ3YXJlLm5ldC93ZWVid2FyZXgvaXBfbG9nLnBocA=="));
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestData.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, ("weebware"));
		CURLcode code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	// std::cin.get(); crashes for some reason? why was this ever there.
	if (content == (base64_decode("Q09OTkVDVElPTiBFWENFUFRJT04=").c_str()) || content == (base64_decode("T0ZGTElORQ==").c_str()) || content == (base64_decode("Tk8gTE9HSU4gRk9VTkQ=").c_str())) {
		std::string exceptionError = "=4ibvl2czV2cgUmchdnYlV2dgIXdvlHIm9GI5RXaydWZ05WagUGa0BSemlmclZHIvRHIlxmYh5WV";
		reverseString(exceptionError);
		MessageBox(NULL, (base64_decode(exceptionError).c_str()), ("Error"), MB_ICONWARNING);
		exit(EXIT_SUCCESS);
	}
	if (content == base64_decode("U0VTU0lPTiBFWFBJUkVE").c_str()) {
		std::string test = "==gLkVmcpBHelBychhGIu9WazNXZzBSZyF2diVWZ3Bic19WW";
		reverseString(test);
		MessageBox(NULL, (base64_decode(test).c_str()), ("Error"), MB_ICONWARNING);
		exit(EXIT_SUCCESS);
	}
	reverseString(content);;
	content = base64_decode(content);
	reverseString(content);
	std::stringstream originalContentStream(content);
	std::string piece;
	content = "";
	while (std::getline(originalContentStream, piece, ',')) {
		int charValue = atoi(piece.c_str()) / 8;
		char c = charValue;
		content += c;
	}
	OutputDebugString(content.c_str());
	std::vector<std::string> pieces;
	std::stringstream decodedContentStream(content);
	content = "";
	while (std::getline(decodedContentStream, piece, '|'))
		pieces.push_back(piece);
	int serverTimestamp = atoi(pieces.at(1).c_str());
	if (serverTimestamp < timestamp - 30) {
		// it took us more than 30 seconds to receive and parse that data? :thinking:
		pieces.clear();
		decodedContentStream.str(std::string());
		std::string gayNigger = "=4ibvl2czV2cgUmchdnYlV2dgIXdvlHIm9GI5RXaydWZ05WagUGa0BSemlmclZHIvRHIlxmYh5WV";
		reverseString(gayNigger);
		MessageBox(NULL, (base64_decode(gayNigger).c_str()), ("Error"), MB_ICONWARNING);
		exit(EXIT_SUCCESS);
	}
	content = pieces.at(0);
	return content;
}

static void auth::reverseString(std::string &str) {
	int n = str.length();
	for (int i = 0; i < n / 2; i++)
		std::swap(str[i], str[n - i - 1]);
}

static std::string auth::base64_encode(const std::string &in) {

	std::string out;
	int val = 0, valb = -6;
	for (unsigned char c : in) {
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) {
			out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
	while (out.size() % 4) out.push_back('=');
	return out;
}

std::string auth::base64_decode(const std::string &in) {

	std::string out;

	std::vector<int> T(256, -1);
	for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

	int val = 0, valb = -8;
	for (unsigned char c : in) {
		if (T[c] == -1) break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0) {
			out.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return out;
}