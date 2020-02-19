#pragma once
#include "Header.h"

class KeyValues
{
public:
	char _pad[0x20];//csgo, for css its a diff size

	KeyValues* FindKey(const char* keyName, bool bCreate = false);
	void set_string(const char* keyName, const char* value);



};