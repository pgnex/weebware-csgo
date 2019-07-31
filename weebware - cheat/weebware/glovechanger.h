#pragma once
#include "Header.h"
class c_glovechanger {
public:
	bool apply_glove_model(c_basecombat_weapon* glove) noexcept;
	bool apply_glove_skin(c_basecombat_weapon* glove, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear) noexcept;
	void run() noexcept;

	std::vector<const char*> set_glove_skin_array();
	std::map<int, int> sport_glove_skins = { {0, 10048}, {1, 10046}, {2, 10045}, {3, 10047}, {4, 10037}, {5, 10018}, {6, 10038}, {7, 10019} };
	std::map<int, int> handwrap_glove_skins = { {0, 10053}, {1, 10056}, {2, 10054}, {3, 10055}, {4, 10021}, {5, 10010}, {6, 10036}, {7, 10009} };
	std::map<int, int> specialist_glove_skins = { {0, 10063}, {1, 10061}, {2, 10064}, {3, 10062}, {4, 10033}, {5, 10034}, {6, 10035}, {7, 10030} };
	std::map<int, int> driver_glove_skins = { {0, 10041}, {1, 10042}, {2, 10043}, {3, 10044}, {4, 10016}, {5, 10013}, {6, 10040}, {7, 10015} };
	std::map<int, int> moto_glove_skins = { {0, 10052}, {1, 10049}, {2, 10050}, {3, 10051}, {4, 10026}, {5, 10024}, {6, 10027}, {7, 10028} };
	std::map<int, int> hydra_glove_skins = { {0, 10060}, {1, 10057}, {2, 10059}, {3, 10058} };
	std::map<int, int> bloodhound_glove_skins = { {0, 10006}, {1, 10007}, {2, 10008}, {3, 10039} };

};
extern c_glovechanger glove_changer;