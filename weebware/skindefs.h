#pragma once
#ifndef SKINDEFS
#define SKINDEFS

class c_skinchanger {
public:
	struct gun_type
	{
		int id;
		std::string name;
	};

	struct skin_type
	{
		int id;
		std::string name;
	};

	struct knife_type
	{
		int weapon_index = 0;
		std::string mdl = "";
		std::string weapon_name = "";
	};

};

#endif
