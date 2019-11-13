#pragma once

#include <iostream>


namespace networking {

	std::string post_request(std::string url, std::string post_data);
	std::string get_request(std::string url);
	void download_file(std::string url, std::string path);
}