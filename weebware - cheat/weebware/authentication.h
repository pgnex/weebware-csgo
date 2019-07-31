#pragma once
#include "Header.h"

namespace auth
{
	std::string GetServerVariable(std::string key);
	__int64 GetEpochMS();
	__int64 GetEpochS();
	std::string base64_encode(const std::string &in);
	void reverseString(std::string &str);
	std::string base64_decode(const std::string &in);
};