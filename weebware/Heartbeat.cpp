#include "Heartbeat.h"

/*
size_t write_callback(void *contents, size_t size, size_t nmemb, std::string *s)
{
	size_t newLength = size * nmemb;
	size_t oldLength = s->size();

	try
	{
		s->resize(oldLength + newLength);
	}
	catch (...)
	{
		// Fuck life. I crashed :C
		return 0;
	}

	std::copy((char*)contents, (char*)contents + newLength, s->begin() + oldLength);

	return size * nmemb;
}

#define User "client/heartbeat(compatable: sadu21bd12d0812dhn012903iu120fn1308f9nw308fh103fn;);"

std::string send_request(std::string strAddress, std::string strPost)
{
	CURL *curl = curl_easy_init();
	std::string strResponse = "";
	if (curl)
	{
		curl_easy_reset(curl);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, User);
		curl_easy_setopt(curl, CURLOPT_URL, strAddress.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strResponse);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		return strResponse;
	}
	else
	{
		return ("Network Failure");
	}
}


struct beat
{
	long long m_lastbeat;

};

void c_heartbeat::send_beat(void)
{
	// failed.
	++m_missed_beats;
}

void crash_game(void)
{
	uintptr_t* random_memory = reinterpret_cast<uintptr_t*>(g_entry.pattern_scan("engine.dll", "66 3B CB 0F 84 ? ? ? ? 8B 57 04 0F B7 C1 56 8D 04 40 C1 E0 03 89 45 FC 66 39 1C 02"));
	*random_memory = 0;
}*/