#include "weebware_config.h"
#include "Header.h"

#define CURL_STATICLIB
#include "curl/curl.h"
#pragma comment (lib, "curl/libcurl_a.lib")

c_config_list g_config_list;

c_weebwarecfg g_weebwarecfg;
c_weebwareskinscfg g_weebwareskinscfg;
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

static int writer(char* data, size_t size, size_t nmemb, std::string* writerData) {
	if (writerData == NULL) return 0;
	writerData->append(data, size * nmemb);
	return size * nmemb;
}

std::string post_request(std::string url, std::string post_data) {
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
	}
	return content;
}

void download_config() {
	CURL* curl;
	FILE* fp;
	CURLcode res;
	curl = curl_easy_init();
	std::string path = "c:/weebware/cfgs/" + g_config_list.cur_config_browser_name + (std::string)".weebware";
	if (curl)
	{
		fp = fopen(path.c_str(), "wb");
		curl_easy_setopt(curl, CURLOPT_URL, "http://auth.weebware.net/configs/" + g_config_list.cur_secret + ".weebware");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
	}
}

void c_config_list::update_config_browser() {
	std::vector<std::string> info;
	std::string content = post_request("http://auth.weebware.net/configs/request.php", " ");
	g_config_list.config_browser_buffer = json::parse(content);
	for (const auto& name : g_config_list.config_browser_buffer.at("name")) {
		info.push_back(name);
	}
	config_browser_info = info;
}

void c_config_list::load_browser_config() {

	// nothing selected
	if (g_config_list.cur_secret == "")
		return;


	download_config();
	load_weebware_config(g_config_list.cur_config_browser_name + ".weebware");
	std::string full_config = weebware_dir;
	full_config.append("\\");
	full_config.append(g_config_list.cur_config_browser_name);
	full_config.append(".weebware");
	remove(full_config.c_str());
	update_all_configs();
}

void c_config_list::update_all_configs()
{
	config_names = get_all_files_names_within_folder(weebware_dir);
}

void c_config_list::save_weebware_config()
{
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
	full_config.append("skins");
	std::stringstream skinfile;
	skinfile << full_config;
	std::ofstream out2(skinfile.str().c_str(), std::ofstream::out | std::ofstream::trunc);
	// Write to file
	g_weebwareskinscfg.save_cfg(out2);
	// Finish up
	out2.close();

	update_all_configs();
}

void c_config_list::save_existing_weebware()
{
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
	full_config.append("skins");
	std::stringstream skinfile;
	skinfile << full_config;
	std::ofstream out2(skinfile.str().c_str(), std::ofstream::out | std::ofstream::trunc);
	// Write to file
	g_weebwareskinscfg.save_cfg(out2);
	// Finish up
	out2.close();

	update_all_configs();
}

bool file_exists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void c_config_list::load_weebware_config(std::string load_name)
{

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

	full_config.append("skins");
	if (file_exists(full_config)) {
		std::stringstream skinfile;
		skinfile << full_config;
		std::ifstream infile2;
		infile2.open(skinfile.str().c_str(), std::ios::in);
		g_weebwareskinscfg.load_cfg(infile2);
		infile2.close();
		update_all_configs();
	}
}

void c_config_list::delete_weebware_config()
{
	// Build the file.
	std::string full_config = weebware_dir;
	full_config.append("\\");
	full_config.append(g_config_list.cur_load_name);
	remove(full_config.c_str());
	full_config.append("skins");
	remove(full_config.c_str());
	update_all_configs();
}

void write_item(std::ostream& file, override_skin_style item)
{

}