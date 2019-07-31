#include "glovechanger.h"

c_glovechanger glove_changer;

static auto get_wearable_create_fn() -> create_client_class_fn {
	auto client_class = g_weebware.g_client->get_all_classes();

	for (client_class = g_weebware.g_client->get_all_classes();
		client_class; client_class = client_class->m_pNext) {

		if (client_class->m_ClassID == (int)class_ids::ceconwearable) {
			return client_class->m_pCreateFn;
		}
	}
	return NULL;
}


static auto make_glove(int entry, int serial) -> c_basecombat_weapon* {
	static auto create_wearable_fn = get_wearable_create_fn();
	create_wearable_fn(entry, serial);

	const auto glove = static_cast<c_basecombat_weapon*>(g_weebware.g_entlist->getcliententity(entry));
	assert(glove); {
		static auto set_abs_origin_addr = g_weebware.pattern_scan("client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");
		const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const std::array<float, 3>&)>(set_abs_origin_addr);
		static constexpr std::array<float, 3> new_pos = { 10000.f, 10000.f, 10000.f };
		set_abs_origin_fn(glove, new_pos);
	}
	return glove;
}

std::vector<const char*> c_glovechanger::set_glove_skin_array() {

	std::vector<const char*> default = { " " };
	std::vector<const char*> sport = { "Vice", "Bronze Morph", "Amphibious", "Omega", "Pandora's Box",  "Superconductor",  "Hedge Maze",  "Arid" };
	std::vector<const char*> hand_wraps = { "Cobalt Skulls", "Arboreal", "Overprint", "Duct Tape", "Slaughter", "Spruce DDPAT", "Badlands",  "Leather" };
	std::vector<const char*> specialist = { "Fade", "Crimson Web", "Mogul", "Buckshot", "Crimson Kimono", "Emerald Web", "Foundation", "Forest DDPAT" };
	std::vector<const char*> driver = { "King Snake", "Imperial Plaid", "Overtake", "Racing Green", "Crimson Weave", "Lunar Weave", "Diamondback", "Convoy" };
	std::vector<const char*> moto = { "Polygon", "POW!", "Turtle", "Transport", "Spearmint", "Eclipse", "Boom!", "Cool Mint" };
	std::vector<const char*> hydra = { "Case Hardened", "Emerald", "Rattler", "Mangrove", };
	std::vector<const char*> bloodhound = { "Charred", "Snakebite", "Bronzed", "Guerrilla" };

	switch (g_weebwarecfg.glove_model) {
	case 0:
		return default;
	case 1:
		return sport;
	case 2:
		return hand_wraps;
	case 3:
		return specialist;
	case 4:
		return driver;
	case 5:
		return moto;
	case 6:
		return hydra;
	case 7:
		return bloodhound;
	}

	return default;
}

bool c_glovechanger::apply_glove_model(c_basecombat_weapon* glove) noexcept {
	player_info info;
	g_weebware.g_engine->GetPlayerInfo(g_weebware.g_engine->get_local(), &info);
	*glove->get_accountid() = info.xuid_low;
	*reinterpret_cast<int*>(uintptr_t(glove) + 0x64) = -1;

	return true;
}

bool c_glovechanger::apply_glove_skin(c_basecombat_weapon* glove, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear) noexcept {
	*glove->m_iItemDefinitionIndexPtr() = item_definition_index;
	*glove->get_paint_kit() = paint_kit;
	glove->set_model_index(model_index);
	*glove->m_iEntityQuality() = entity_quality;
	*glove->get_fallbackwear() = fallback_wear;

	return true;
}

std::map<int, const char*> init_glove_indexes() {

	std::map<int, const char*> m;

	m.insert(std::make_pair(glove_studded_bloodhound, "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
	m.insert(std::make_pair(glove_sporty, "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
	m.insert(std::make_pair(glove_slick, "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
	m.insert(std::make_pair(glove_leather_wrap, "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
	m.insert(std::make_pair(glove_motorcycle, "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
	m.insert(std::make_pair(glove_specialist, "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));
	m.insert(std::make_pair(glove_hydra, "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl"));

	return m;
}

int get_glove_model() {
	switch (g_weebwarecfg.glove_model) {
	case 0:
		break;
	case 1:
		return glove_sporty;
	case 2:
		return glove_leather_wrap;
	case 3:
		return glove_specialist;
	case 4:
		return glove_slick;
	case 5:
		return glove_motorcycle;
	case 6:
		return glove_hydra;
	case 7:
		return glove_studded_bloodhound;
	}

	return glove_ct_side;
}

int get_skin() {

	switch (g_weebwarecfg.glove_model) {
	case 0:
		break;
	case 1:
		return glove_changer.sport_glove_skins[g_weebwarecfg.glove_skin];
	case 2:
		return glove_changer.handwrap_glove_skins[g_weebwarecfg.glove_skin];
	case 3:
		return glove_changer.specialist_glove_skins[g_weebwarecfg.glove_skin];
	case 4:
		return glove_changer.driver_glove_skins[g_weebwarecfg.glove_skin];
	case 5:
		return glove_changer.moto_glove_skins[g_weebwarecfg.glove_skin];
	case 6:
		return glove_changer.hydra_glove_skins[g_weebwarecfg.glove_skin];
	case 7:
		return glove_changer.bloodhound_glove_skins[g_weebwarecfg.glove_skin];
	}

	return 0;
}

void c_glovechanger::run() noexcept {

	if (!g_weebwarecfg.glove_model > 0)
		return;

	if (!g_weebware.g_engine->is_connected() && !g_weebware.g_engine->is_in_game())
		return;

	c_base_entity* local_player = reinterpret_cast<c_base_entity*>(g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local()));

	if (!local_player)
		return;

	std::map<int, const char*> glove_map = init_glove_indexes();

	uintptr_t* const wearables = local_player->get_wearables();

	if (!wearables)
		return;

	static uintptr_t glove_handle = uintptr_t(0);

	auto glove = reinterpret_cast<c_basecombat_weapon*>(g_weebware.g_entlist->getcliententityfromhandle(reinterpret_cast<HANDLE>(wearables[0])));

	if (!glove) {
		const auto our_glove = reinterpret_cast<c_basecombat_weapon*>(g_weebware.g_entlist->getcliententityfromhandle(reinterpret_cast<HANDLE>(glove_handle)));

		if (our_glove) {
			wearables[0] = glove_handle;
			glove = our_glove;
		}
	}


	if (!local_player || local_player->m_iHealth() <= 0) {
		if (glove) {
			glove->SetDestroyedOnRecreateEntities();
			glove->Release();
		}
		return;
	}

	if (!glove) {
		const auto entry = g_weebware.g_entlist->gethighestentityindex() + 1;
		const auto serial = rand() % 0x1000;
		glove = make_glove(entry, serial);
		wearables[0] = entry | serial << 16;
		glove_handle = wearables[0];
	}


	if (glove) {
		float g_wear = 0.15f;
		

		apply_glove_model(glove);

		set_glove_skin_array();

		apply_glove_skin(glove, get_glove_model(), get_skin(), g_weebware.g_model_info->getmodelindex(glove_map[get_glove_model()]), 3, g_wear);

		*glove->get_item_id_high() = -1;
		*glove->get_fallbackseed() = 0;
		*glove->fallback_stattrak() = -1;

		glove->PreDataUpdate(0);
	}
}
