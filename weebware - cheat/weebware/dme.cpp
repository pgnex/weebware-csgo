#include "Header.h"
#include "shared.h"
#include "dme.h"
#include "hook_funcs.h"
#include <iostream>

c_draw_model_execute g_dme;

void hooks::hook_functions::draw_model_execute(void* thisptr, void* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld) {

	if (!ctx)
		return;

	// if its not a player or arms nothing needed
	const char* model_name = g_weebware.g_model_info->getmodelname(pInfo.pModel);
	if (!(strstr(model_name, "models/player")) && !(strstr(model_name, "arms")))
		return;

	g_dme.player_chams(pInfo);

}
void init_key_vals(KeyValues* keyValues, char* name)
{
	static DWORD keyval_addr = 0;

	if (!keyval_addr)keyval_addr = g_weebware.pattern_scan("client.dll", "68 ?? ?? ?? ?? 8B C8 E8 ?? ?? ?? ?? 89 45 FC EB 07 C7 45 ?? ?? ?? ?? ?? 8B 03 56*") + 7;

	static DWORD dwFunction = 0;

	if (!dwFunction) dwFunction = keyval_addr + *reinterpret_cast<PDWORD_PTR>(keyval_addr + 1) + 5;

	if (!dwFunction) {
		return;
	}
	__asm
	{
		push name
		mov ecx, keyValues
		call dwFunction
	}
}

void load_from_buf(KeyValues* keyValues, char const* resourceName, const char* pBuffer, class IBaseFileSystem* pFileSystem, const char* pPathID, void* pUnknown, void* uu)
{
	static  DWORD dwFunction = 0;

	if (!dwFunction) dwFunction = g_weebware.pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89");

	if (!dwFunction) {
		return;
	}

	__asm
	{
		push uu
		push pUnknown
		push pPathID
		push pFileSystem
		push pBuffer
		push resourceName
		mov ecx, keyValues
		call dwFunction
	}
}

imaterial* create_default() {

	static const char material[] =
	{
		"\"VertexLitGeneric\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$flat\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$nofog\" \"1\"\
		\n\t\"$ignorez\" \"0\"\
		\n\t\"$znearer\" \"0\"\
        \n}\n"
	};

	KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
	init_key_vals(keyValues, "VertexLitGeneric");
	load_from_buf(keyValues, "default.vmt", material, 0, 0, 0, 0);
	imaterial* created_mat = g_weebware.g_mat_sys->create_mat("default.vmt", keyValues);

	return created_mat;
}

imaterial* create_glow() {

	std::stringstream s;

	s << "\"VertexLitGeneric\" {\n\n\t";
	s << "\"basetexture\" \"vgui/white\"\n\t";
	s << "\"$additive\" \"0\"\n\t";
	s << "\"$envmap\" \"models/effects/cube_white\"\n\t";
	s << "\"$envmaptint\"" << " \"[" << 1 << " " << 1 << " " << 1 << "]\"\n\t";
	s << "\"$envmapfresnel\" \"1\"\n\t";
	s << "\"$envmapfresnelminmaxexp\" \"[0 1 2]\"\n\t";
	s << "\"$alpha\" \"0.8\"\n";
	s << "}";


	KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
	init_key_vals(keyValues, "VertexLitGeneric");
	load_from_buf(keyValues, "glow.vmt", s.str().c_str(), 0, 0, 0, 0);
	imaterial* created_mat = g_weebware.g_mat_sys->create_mat("glow.vmt", keyValues);

	return created_mat;
}

void c_draw_model_execute::player_chams(const modelrenderinfo_t& pInfo) {

	// if model isnt a player no point
	const char* model_name = g_weebware.g_model_info->getmodelname(pInfo.pModel);
	if (!(strstr(model_name, "models/player")))
		return;

	// get local
	c_base_entity* local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());
	if (!local)
		return;

	c_base_entity* model_ent = g_weebware.g_entlist->getcliententity(pInfo.entity_index);
	if (!model_ent || !model_ent->is_valid_player())
		return;

	// setting up mats
	static bool init = false;
	static imaterial* mat_list[custom_mats::max];
	if (!init) {
		mat_list[custom_mats::plain] = create_glow();
		init = true;
	}


	float col_blend[4] = {200.f, 0.f, 255.f, 0.8f };
	g_weebware.g_model_render->forcedmaterialoverride(mat_list[custom_mats::plain]);
	g_weebware.g_render_view->SetBlend(1.f);
	g_weebware.g_render_view->SetColorModulation(col_blend);
	mat_list[custom_mats::plain]->setmaterialvarflag(material_var_ignorez, true);
}


void c_draw_model_execute::hand_chams(const modelrenderinfo_t& pInfo) {

}