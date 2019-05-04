#pragma once
#include "Header.h"

//#define CURL_STATICLIB
//#include "c:/programmingbinarys/curl-master/include/curl/curl.h"
//#pragma comment (lib,"libcurl.lib")
//#pragma comment(lib,"wininet.lib")
//#pragma comment(lib,"Ws2_32.lib")
//#pragma comment(lib,"Wldap32.lib")
//#pragma comment (lib,"Gdiplus.lib")
//#pragma warning( disable : 4996 )


namespace auth
{
	std::string GetServerVariable(std::string key);
	__int64 GetEpochMS();
	__int64 GetEpochS();
	std::string base64_encode(const std::string &in);
	void reverseString(std::string &str);
	std::string base64_decode(const std::string &in);
};