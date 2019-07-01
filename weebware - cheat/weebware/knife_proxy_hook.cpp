#include "Header.h"
#include "shared.h"
#include "knife_proxy_hook.h"
#include <string>
#include <map>

c_knifehook knife_hook;

typedef void(*recv_var_proxy_fn)(const c_recv_proxy_data* p_data, void* p_struct, void* p_out);

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1

inline int RandomSequence(int low, int high) {
	return (rand() % (high - low + 1) + low);
}

struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};

template<class T>
static T* find_hud_element(const char* name) {
	static auto fn = *reinterpret_cast<DWORD * *>(g_weebware.pattern_scan("client_panorama.dll", ("B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08")) + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(g_weebware.pattern_scan("client_panorama.dll", ("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28")));
	return (T*)find_hud_element(fn, name);
}


void c_knifehook::force_update() {
	static auto fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(g_weebware.pattern_scan("client_panorama.dll", ("55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C")));

	auto element = find_hud_element<std::uintptr_t*>(("CCSGO_HudWeaponSelection"));

	auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);
	if (hud_weapons == nullptr)
		return;

	if (!*hud_weapons->get_weapon_count())
		return;

	for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
		i = fn(hud_weapons, i);

	(*g_weebware.g_client_state)->full_update();
}

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
recv_var_proxy_fn sequence_proxy_fn = nullptr;
recv_var_proxy_fn recv_model_index;


std::map<int, int> init_knife_indexes() {

	std::map<int, int> m;

	m.insert(std::make_pair(weapon_knife_t, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_default_t.mdl")));
	m.insert(std::make_pair(weapon_knife, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_default_ct.mdl")));
	m.insert(std::make_pair(weapon_knife_bayonet, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_bayonet.mdl")));
	m.insert(std::make_pair(weapon_knife_m9_bayonet, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_m9_bay.mdl")));
	m.insert(std::make_pair(weapon_knife_karambit, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_karam.mdl")));
	m.insert(std::make_pair(weapon_knife_survival_bowie, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_survival_bowie.mdl")));
	m.insert(std::make_pair(weapon_knife_butterfly, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_butterfly.mdl")));
	m.insert(std::make_pair(weapon_knife_falchion, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_falchion_advanced.mdl")));
	m.insert(std::make_pair(weapon_knife_flip, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_flip.mdl")));
	m.insert(std::make_pair(weapon_knife_gut, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_gut.mdl")));
	m.insert(std::make_pair(weapon_knife_tactical, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_tactical.mdl")));
	m.insert(std::make_pair(weapon_knife_push, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_push.mdl")));
	m.insert(std::make_pair(weapon_knife_gypsy_jackknife, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_gypsy_jackknife.mdl")));
	m.insert(std::make_pair(weapon_knife_stiletto, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_stiletto.mdl")));
	m.insert(std::make_pair(weapon_knife_widowmaker, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_widowmaker.mdl")));
	m.insert(std::make_pair(weapon_knife_ursus, g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_ursus.mdl")));

	return m;
}


bool is_knife_index(std::map<int, int> map, int knife_index) {
	for (auto& i : map) {
		if (i.second == knife_index) {
			return true;
		}
	}
	return false;
}

void hooked_recvproxy_viewmodel(c_recv_proxy_data* p_data, void* p_struct, void* p_out)noexcept {

	if (!g_weebwarecfg.skinchanger_enabled)
		return recv_model_index(p_data, p_struct, p_out);

	std::map<int, int> knife_map = init_knife_indexes();

	c_base_entity* local_player = reinterpret_cast<c_base_entity*>(g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local()));

	if (!local_player)
		return recv_model_index(p_data, p_struct, p_out);

	if (local_player->is_valid_player() && is_knife_index(knife_map, p_data->value.m_int))
		p_data->value.m_int = knife_map[g_weebwarecfg.selected_knife_index[0]];

	recv_model_index(p_data, p_struct, p_out);
}

void set_view_model_sequence(const c_recv_proxy_data* pDataConst, void* p_struct, void* p_out)noexcept {
	c_recv_proxy_data* p_data = const_cast<c_recv_proxy_data*>(pDataConst);
	c_viewmodel* player_view_model = static_cast<c_viewmodel*>(p_struct);

	if (player_view_model) {
		auto local_player = reinterpret_cast<c_base_entity*>(g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local()));
		c_base_entity* p_owner = static_cast<c_base_entity*>(g_weebware.g_entlist->getcliententity(player_view_model->m_hOwner() & 0xFFF));
		if (p_owner == local_player) {
			std::string sz_model = g_weebware.g_model_info->getmodelname(g_weebware.g_model_info->getmodel(*player_view_model->m_nModelIndex()));
			int m_nSequence = p_data->value.m_int;
			if (sz_model == "models/weapons/v_knife_butterfly.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
				}
			}
			else if (sz_model == "models/weapons/v_knife_falchion_advanced.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
					break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (sz_model == "models/weapons/v_knife_push.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
					break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
					break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (sz_model == "models/weapons/v_knife_survival_bowie.mdl") {
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1;
					break;
				default:
					m_nSequence--;
				}
			}
			else if (sz_model == "models/weapons/v_knife_ursus.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_stiletto.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(12, 13);
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_widowmaker.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(14, 15);
					break;
				}
			}
			p_data->value.m_int = m_nSequence;
		}
	}
	sequence_proxy_fn(p_data, p_struct, p_out);
}


void knife_animation_hook()noexcept {
	for (auto client_class = g_weebware.g_client->get_all_classes(); client_class; client_class = client_class->m_pNext) {

		if (!strcmp(client_class->m_networkedname, "CBaseViewModel")) {
			const auto table = reinterpret_cast<recv_table*>(client_class->m_precvtable);
			for (int nIndex = 0; nIndex < table->props_count; nIndex++) {
				recv_prop* pProp = &table->props[nIndex];
				if (!pProp || strcmp(pProp->prop_name, "m_nSequence"))
					continue;
				sequence_proxy_fn = static_cast<recv_var_proxy_fn>(pProp->proxy_fn);
				pProp->proxy_fn = static_cast<recv_var_proxy_fn>(set_view_model_sequence);
				break;
			}
			break;
		}
	}
}

void c_knifehook::knife_animation() noexcept {
	knife_animation_hook();

	auto client_class = g_weebware.g_client->get_all_classes();
	while (client_class) {
		const char* pszName = client_class->m_precvtable->table_name;
		if (!strcmp(pszName, "DT_SmokeGrenadeProjectile")) {
			for (int i = 0; i < client_class->m_precvtable->props_count; i++) {
				recv_prop* pProp = &(client_class->m_precvtable->props[i]);
				const char* name = pProp->prop_name;
				if (!strcmp(name, "m_bDidSmokeEffect")) {
				}
			}
		}
		else if (!strcmp(pszName, "DT_BaseViewModel")) {
			for (int i = 0; i < client_class->m_precvtable->props_count; i++) {
				recv_prop* pProp = &(client_class->m_precvtable->props[i]);
				const char* name = pProp->prop_name;
				if (!strcmp(name, "m_nModelIndex")) {
					recv_model_index = (recv_var_proxy_fn)pProp->proxy_fn;
					pProp->proxy_fn = (recv_var_proxy_fn)hooked_recvproxy_viewmodel;
				}
			}
		}
		client_class = client_class->m_pNext;
	}
}
