#pragma once

#include "Header.h"

class c_draw_model_execute {
public:
	void player_chams(const modelrenderinfo_t& pInfo);
	void hand_chams(const modelrenderinfo_t& pInfo);

	enum custom_mats : int {
		disabled,
		plain,
		glow_cham = 2,
		flat,
		glass,
		crystal,
		gold,
		crystal_blue,
		max
	};

	std::vector<const char*> vistasmoke_mats =
	{
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};
}; 
extern c_draw_model_execute g_draw_model_execute;

namespace draw_model_execute {

	void player_chams(const modelrenderinfo_t& pInfo);
	void hand_chams(const modelrenderinfo_t& pInfo);

	enum custom_mats : int {
		disabled,
		plain,
		glow_cham = 2,
		flat,
		glass,
		crystal,
		gold,
		crystal_blue,
		max
	};

	namespace utils {
		imaterial* create_glow();
		imaterial* borrow_mat(custom_mats type);
		imaterial* create_default();
		void init_key_vals(KeyValues* keyValues, char* name);
		void load_from_buf(KeyValues* keyValues, char const* resourceName, const char* pBuffer, class IBaseFileSystem* pFileSystem, const char* pPathID, void* pUnknown, void* uu);
	}
}