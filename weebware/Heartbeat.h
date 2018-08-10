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


class c_heartbeat
{
public:

private:
	void send_beat(void);

	uintptr_t m_missed_beats;
};