#include "weebware_config.h"
#include "Header.h"

c_config_list g_config_list;

c_weebwarecfg g_weebwarecfg;
c_weebwareskinscfg g_weebwareskinscfg;
c_weebwareskins_save g_weebwarecfg_skins_but_donottouch;
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

void c_config_list::update_all_configs()
{
	config_names = get_all_files_names_within_folder(weebware_dir);
}

void write_donottouch_from_config()
{
	for (auto i = 0; i < 35; i++) {
		if (i < 10) {
			g_weebwarecfg_skins_but_donottouch.first[i] = g_weebwareskinscfg.skin_wheel[i];
		}
		else if (i < 20) {
			g_weebwarecfg_skins_but_donottouch.second[i] = g_weebwareskinscfg.skin_wheel[i];
		}
		else if (i < 30) {
			g_weebwarecfg_skins_but_donottouch.third[i] = g_weebwareskinscfg.skin_wheel[i];
		}
		else if (i < 35) {
			g_weebwarecfg_skins_but_donottouch.fourth[i] = g_weebwareskinscfg.skin_wheel[i];
		}
	}
}

void write_cfgs_from_donottouch_toreal()
{
	for (auto i = 0; i < 35; i++) {
		if (i < 10) {
			g_weebwareskinscfg.skin_wheel[i] = g_weebwarecfg_skins_but_donottouch.first[i];
		}
		else if (i < 20) {
			g_weebwareskinscfg.skin_wheel[i] = g_weebwarecfg_skins_but_donottouch.second[i];
		}
		else if (i < 30) {
			g_weebwareskinscfg.skin_wheel[i] = g_weebwarecfg_skins_but_donottouch.third[i];
		}
		else if (i < 35) {
			g_weebwareskinscfg.skin_wheel[i] = g_weebwarecfg_skins_but_donottouch.fourth[i];
		}
	}
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
	out.write((char*)(&g_weebwarecfg), sizeof(c_weebwarecfg));
	// Finish up
	out.close();
	write_donottouch_from_config();
	full_config.append("skins");
	std::stringstream skinfile;
	skinfile << full_config;
	std::ofstream out2(skinfile.str().c_str(), std::ofstream::out | std::ofstream::trunc);
	// Write to file
	out2.write((char*)(&g_weebwarecfg_skins_but_donottouch), sizeof(c_weebwareskins_save));
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
	out.write((char*)(&g_weebwarecfg), sizeof(c_weebwarecfg));
	// Finish up
	out.close();
	write_donottouch_from_config();
	full_config.append("skins");
	std::stringstream skinfile;
	skinfile << full_config;
	std::ofstream out2(skinfile.str().c_str(), std::ofstream::out | std::ofstream::trunc);
	// Write to file
	out2.write((char*)(&g_weebwarecfg_skins_but_donottouch), sizeof(c_weebwareskins_save));
	// Finish up
	out2.close();

	update_all_configs();
}

void c_config_list::load_weebware_config()
{
	// Build the file.
	std::string full_config = weebware_dir;
	full_config.append("\\");
	full_config.append(g_config_list.cur_load_name);
	std::stringstream configfile;
	configfile << full_config;
	std::ifstream infile;
	infile.open(configfile.str().c_str(), std::ios::in);
	while (infile.read((char*)&g_weebwarecfg, sizeof(c_weebwarecfg)));
	infile.close();

	full_config.append("skins");
	std::stringstream skinfile;
	skinfile << full_config;
	std::ifstream infile2;
	infile2.open(skinfile.str().c_str(), std::ios::in);
	while (infile2.read((char*)&g_weebwarecfg_skins_but_donottouch, sizeof(c_weebwareskins_save)));
	infile2.close();

	write_cfgs_from_donottouch_toreal();

	update_all_configs();
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
