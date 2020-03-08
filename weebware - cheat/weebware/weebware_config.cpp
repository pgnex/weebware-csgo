#include "weebware_config.h"
#include "Header.h"

#define CURL_STATICLIB
#include "curl/curl.h"
#pragma comment (lib, "curl/libcurl_a.lib")

c_config_list g_config_list;

c_weebwarecfg g_weebwarecfg;

#define weebware_dir "C:\\weebware\\cfgs"

// https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
std::vector<std::string> get_all_files_names_within_folder(std::string folder)
{
	std::vector<std::string> names;
	std::string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				// Make sure it is a .weebware
				if (std::string(fd.cFileName).find(".weebware") != std::string::npos && (std::string(fd.cFileName).find("data.weebware") == std::string::npos))
				{
					names.push_back(fd.cFileName);
				}
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}


void c_config_list::get_your_configs() {
	std::vector<std::string> info;
	std::string content = networking::get_request("https://weebware.net/api/cheat/config/own?name=" + g_weebware.g_user_name);

	g_config_list.your_configs_buffer = json::parse(content);

	for (const auto& name : g_config_list.your_configs_buffer) {
		info.push_back(name.at("title"));
	}
	config_browser_yours = info;
}

void c_config_list::get_favorited_configs() {
	std::vector<std::string> info;
	std::string content = networking::get_request("https://weebware.net/api/cheat/config/favourite/?name=" + g_weebware.g_user_name);
	g_config_list.favorite_configs_buffer = json::parse(content);

	for (const auto& name : g_config_list.favorite_configs_buffer) {
		info.push_back(name.at("title"));
	}
	config_browser_fav = info;
}

void c_config_list::load_config_from_memory(int index) {

	std::string content = networking::get_request("https://weebware.net/api/cheat/config/?config=" +std::to_string(index));
	json data = json::parse(content);
	g_weebwarecfg.load_cfg_mem(data);
	g_weebwarecfg.skinchanger_apply_nxt = 1;
}


void c_config_list::load_browser_config() {

	// nothing selected
	if (g_config_list.cur_secret == "")
		return;

	networking::download_file("https://api.weebware.net/configs/" + g_config_list.cur_secret + ".weebware", "c:/weebware/cfgs/" + g_config_list.cur_config_browser_name + (std::string)".weebware");
	load_weebware_config(g_config_list.cur_config_browser_name + ".weebware");
	std::string full_config = weebware_dir;
	full_config.append("\\");
	full_config.append(g_config_list.cur_config_browser_name);
	full_config.append(".weebware");
	remove(full_config.c_str());
	update_all_configs();

	g_weebwarecfg.skinchanger_apply_nxt = 1;
}

void c_config_list::update_all_configs() {
	config_names = get_all_files_names_within_folder(weebware_dir);
}

void c_config_list::save_weebware_config() {
	// Make sure we have a config for the first time.
	CreateDirectory(weebware_dir, NULL);

	// Build the file.
	std::string full_config = weebware_dir;
	full_config.append("\\");
	full_config.append(g_config_list.cur_save_name);
	full_config.append(".weebware");

	std::stringstream configfile;
	configfile << full_config;
	std::ofstream out(configfile.str().c_str(), std::ofstream::out | std::ofstream::trunc);
	// Write to file
	g_weebwarecfg.save_cfg(out);
	// Finish up
	out.close();
	update_all_configs();
}

void c_config_list::save_existing_weebware() {
	// Build the file.
	std::string full_config = weebware_dir;
	full_config.append("\\");
	full_config.append(g_config_list.cur_load_name);

	std::stringstream configfile;
	configfile << full_config;
	std::ofstream out(configfile.str().c_str(), std::ofstream::out | std::ofstream::trunc);
	// Write to file
	g_weebwarecfg.save_cfg(out);
	// Finish up
	out.close();
	update_all_configs();
}

bool file_exists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void c_config_list::load_weebware_config(std::string load_name) {
	// make sure something is selected
	if (load_name == "")
		return;

	// Build the file.
	std::string full_config = weebware_dir;
	full_config.append("\\");
	full_config.append(load_name);
	std::stringstream configfile;
	configfile << full_config;
	std::ifstream infile;
	infile.open(configfile.str().c_str(), std::ios::in);
	g_weebwarecfg.load_cfg(infile);
	infile.close();
}

void c_config_list::delete_weebware_config() {
	// Build the file.
	std::string full_config = weebware_dir;
	full_config.append("\\");
	full_config.append(g_config_list.cur_load_name);
	remove(full_config.c_str());
	update_all_configs();
}