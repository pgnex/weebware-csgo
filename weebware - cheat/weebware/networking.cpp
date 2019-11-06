#include "Header.h"

#define CURL_STATICLIB
#include "curl/curl.h"
#pragma comment (lib, "curl/libcurl_a.lib")

static int writer(char* data, size_t size, size_t nmemb, std::string* writerData) {
	if (writerData == NULL) return 0;
	writerData->append(data, size * nmemb);
	return size * nmemb;
}

std::string networking::post_request(std::string url, std::string post_data) {
	std::string content;
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = nullptr;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, ("weebware"));
		CURLcode code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		return content;
	}
	return "REQUESTFAILED";
}

void networking::download_file(std::string url, std::string path) {
	CURL* curl;
	FILE* fp;
	CURLcode res;
	curl = curl_easy_init();
	if (curl)
	{
		fp = fopen(path.c_str(), "wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
	}
}