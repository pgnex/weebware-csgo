#include "Header.h"
#include "shared.h"
#include "netvars.h"
#include "drawing.h"
#include "hook_funcs.h"
c_weebware g_weebware;

unsigned __stdcall entry_thread(void* v_arg)
{
	g_weebware.setup_thread(); // run our thread

	_endthreadex(0); // close thread

	return 0; // return 0.
}

create_interface retrieve_interface(LPCSTR module_name);

bool c_weebware::init_interfaces()
{
	Sleep(5000);

	while (!(g_weebware.h_window = FindWindowA("Valve001", NULL)))
		Sleep(200);

	engine_fact = retrieve_interface("engine.dll");

	if (GetModuleHandleA("client.dll"))
		client_fact = retrieve_interface("client.dll");
	else
		client_fact = retrieve_interface("client_panorama.dll");

	surface_fact = retrieve_interface("vguimatsurface.dll");
	vgui2_fact = retrieve_interface("vgui2.dll");
	mat_system_fact = retrieve_interface("materialsystem.dll");
	vstd_lib_fact = retrieve_interface("vstdlib.dll");
	steam_fact = retrieve_interface("steamclient.dll");
	cache_fact = retrieve_interface("datacache.dll");
	phys_fact = retrieve_interface("vphysics.dll");
	input_fact = retrieve_interface("inputsystem.dll");

	// Load Our Interfaces.	

	// g_engine = reinterpret_cast<c_engine_client*>(engine_fact("VEngineClient014", NULL));
	g_engine = reinterpret_cast<c_engine_client*>(engine_fact(auth::GetServerVariable(auth::base64_decode("cmF0")).c_str(), NULL));

	g_client = reinterpret_cast<i_base_client*>(client_fact("VClient018", NULL));
	// g_client = reinterpret_cast<i_base_client*>(client_fact(auth::GetServerVariable(auth::base64_decode("Y2F0")).c_str(), NULL));

	//g_client_mode = *(unsigned long**)((*(uintptr_t**)g_client)[10] + 0x5);
	// Im not a paster lol. 
	g_client_mode = **(unsigned long***)(pattern_scan("client.dll", "55 8B EC 8B 0D ? ? ? ? 8B 01 5D FF 60 30") + 0x5);

	g_entlist = reinterpret_cast<c_entity_list*>(client_fact("VClientEntityList003", NULL));

	g_panel = reinterpret_cast<c_panel*>(vgui2_fact("VGUI_Panel009", NULL));

	g_surface = reinterpret_cast<c_surface*>(surface_fact("VGUI_Surface031", NULL));

	g_direct_x = **reinterpret_cast<IDirect3DDevice9***>(pattern_scan("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 0x1);

	g_input_system = reinterpret_cast<c_input_system*>(input_fact("InputSystemVersion001", NULL));

	g_model_info = reinterpret_cast<iv_model_info*>(engine_fact("VModelInfoClient004", NULL));

	g_engine_trace = reinterpret_cast<i_engine_trace*>(engine_fact("EngineTraceClient004", NULL));

	g_debug_overlay = reinterpret_cast<c_debug_overlay*>(engine_fact("VDebugOverlay004", NULL));

	g_global_vars = *reinterpret_cast<c_global_vars**>(((*(PDWORD*)g_client)[0]) + 0x1B);
	g_global_vars = reinterpret_cast<c_global_vars*>(*(PDWORD)g_global_vars);

	g_render_view = reinterpret_cast<c_render_view*>(engine_fact("VEngineRenderView014", NULL));

	g_mat_sys = reinterpret_cast<c_mat_system*>(mat_system_fact("VMaterialSystem080", NULL));

	g_model_render = reinterpret_cast<c_model_render*>(engine_fact("VEngineModel016", NULL));

	g_present_address = pattern_scan("gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 DB") + 0x2;

	//int __stdcall reset_replacement(int a1, int a2)
	// This is the first reset func
	g_reset_address = pattern_scan("gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 FF 78 18 85 F6 74 14 FF 75 E0 8B CE 53 FF 75 EC E8 ? ? ? ? 8A 45 E1 88 46 4D") + 0x2;

	g_game_movement = reinterpret_cast<c_gamemovement*>(client_fact("GameMovement001", NULL));

	g_prediction = reinterpret_cast<c_prediction*>(client_fact("VClientPrediction001", NULL));

	g_move_helper = **(c_move_helper***)(pattern_scan("client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 0x2);

	g_convars = reinterpret_cast<c_iconvar*>(vstd_lib_fact("VEngineCvar007", NULL));

	g_effects = reinterpret_cast<i_effects*>(client_fact("IEffects001", NULL));

	// Load our meme database for our p netvars
	netvar_manager::_instance()->create_database();

	// Load fonts.
	g_draw.GetDevice(g_direct_x);

	init_fonts();

	g_config_list.update_all_configs();

#pragma region load_skins
	// Initialise 
	for (auto i = 0; i < 100; i++) {
		g_weebwarecfg.skin_wheel[i] = override_skin_style();
		g_weebwarecfg.skin_wheel[i].weapon_id = i;
	}

	g_skin_list = create_skin_list();

	g_gun_list = create_gun_list();

	g_knife_list = create_knife_list();
#pragma endregion

	srand(time(0));

	return true;
}

void c_weebware::init_fonts()
{
	tahoma_font = g_weebware.g_surface->create_font();
	g_weebware.g_surface->setfontglyphset(tahoma_font, "Tahoma", 13, 300, 0, 0, fontflag_antialias | fontflag_dropshadow | fontflag_outline);
	g_draw.AddFont("Verdana", 12, true, false);
	g_draw.AddFont("Verdana", 11, true, false);
	g_draw.AddFont("Verdana", 18, true, false);
	g_draw.AddFont("Verdana", 17, true, false);
}

void c_weebware::setup_thread()
{
#define debug 0

#if debug
	setup_debug_window();
#endif

	if (init_interfaces())
	{
#if 0
		netvar_manager::_instance()->dump("latest.txt");
#endif 

		//if (g_weebware.g_engine->get_engine_build() != 13644)
		//{
		//	goto label_exit;
		//}

		g_hooking.hook_all_functions();
	}


	while (g_vars.g_unload.get() == 0.0f)
	{
#if 0
		g_config.Process();
#endif
		Sleep(250);
	}
	g_hooking.unhook_all_functions();

label_exit:

	printf("\nUnloading\n");

	FreeConsole();

	Sleep(1500); // allow thread finishing time

	FreeLibraryAndExitThread(h_module, 0);
}

void c_weebware::setup_debug_window()
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	SetConsoleTitle("weebware Cheat Console");
}

std::vector<c_skinchanger::gun_type> c_weebware::create_gun_list()
{
	std::ifstream skin_file("C://weebware//dependencies//guns.txt");
	std::string cur_line = "";
	std::vector<c_skinchanger::gun_type> gun_list;
	while (std::getline(skin_file, cur_line))
	{
		std::string skin_name = strstr(cur_line.c_str(), ":");
		auto name_length = skin_name.length();
		skin_name = skin_name.substr(2);
		auto id_len = cur_line.length() - name_length;
		auto id = cur_line.substr(0, id_len);
		auto iId = std::stoi(id);
		gun_list.push_back({ iId, skin_name });
	}
	return gun_list;
}

std::vector<c_skinchanger::skin_type> c_weebware::create_skin_list()
{
	std::ifstream skin_file("C://weebware//dependencies//skins.txt");
	std::string cur_line = "";
	std::vector<c_skinchanger::skin_type> skin_list;
	while (std::getline(skin_file, cur_line))
	{
		std::string skin_name = strstr(cur_line.c_str(), ":");
		auto name_length = skin_name.length();
		skin_name = skin_name.substr(2);
		auto id_len = cur_line.length() - name_length;
		auto id = cur_line.substr(0, id_len);
		auto iId = std::stoi(id);
		skin_list.push_back({ iId, skin_name });
	}
	return skin_list;
}

std::vector<c_skinchanger::knife_type> c_weebware::create_knife_list()
{
	std::vector<c_skinchanger::knife_type> tmp;

	auto knife_template = [](std::string mdl_file, std::string weapon_name, int defindex) {
		c_skinchanger::knife_type tmp;
		tmp.mdl = mdl_file;
		tmp.weapon_name = weapon_name;
		tmp.weapon_index = defindex;
		return tmp;
	};

	tmp.push_back(knife_template("models/weapons/v_knife_default_t.mdl", "Terrorist Knife", 59));
	tmp.push_back(knife_template("models/weapons/v_knife_default_ct.mdl", "Counter-Terrorist Knife", 42));
	tmp.push_back(knife_template("models/weapons/v_knife_butterfly.mdl", "Butterfly", 515));
	tmp.push_back(knife_template("models/weapons/v_knife_flip.mdl", "Flip", 505));
	tmp.push_back(knife_template("models/weapons/v_knife_gut.mdl", "Gut", 506));
	tmp.push_back(knife_template("models/weapons/v_knife_karam.mdl", "Karambit", 507));
	tmp.push_back(knife_template("models/weapons/v_knife_m9_bay.mdl", "M9 Bayonet", 508));
	tmp.push_back(knife_template("models/weapons/v_knife_tactical.mdl", "Huntsman", 509));
	tmp.push_back(knife_template("models/weapons/v_knife_falchion_advanced.mdl", "Falchion", 512));
	tmp.push_back(knife_template("models/weapons/v_knife_push.mdl", "Shadow Daggers", 516));
	tmp.push_back(knife_template("models/weapons/v_knife_survival_bowie.mdl", "Bowie", 514));
	tmp.push_back(knife_template("models/weapons/v_knife_stiletto.mdl", "Stiletto", 522));
	tmp.push_back(knife_template("models/weapons/v_knife_ursus.mdl", "Ursus", 519));
	tmp.push_back(knife_template("models/weapons/v_knife_gypsy_jackknife.mdl", "Navaja", 520));
	tmp.push_back(knife_template("models/weapons/v_knife_widowmaker.mdl", "Talon", 523));
	return tmp;
}
// paste wtf am i meant to write huh?
uint64_t c_weebware::pattern_scan(const char* model_name, const char* ida_sig)
{
	const char* cModule = model_name;
	if (strstr(model_name, "client.dll") || strstr(model_name, "client_panorama.dll")) {
		if (!GetModuleHandleA("client.dll")) {
			cModule = "client_panorama.dll";
		}
	}
	//CREDITS: learn_more
#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

	MODULEINFO modinfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(cModule), &modinfo, sizeof(MODULEINFO));
	DWORD start_address = (DWORD)modinfo.lpBaseOfDll;
	DWORD endAddress = start_address + modinfo.SizeOfImage;
	const char* pat = ida_sig;
	DWORD first_match = 0;
	for (DWORD _cur = start_address; _cur < endAddress; _cur++)
	{
		if (!*pat) return first_match;
		if (*(PBYTE)pat == '\?' || *(BYTE*)_cur == getByte(pat))
		{
			if (!first_match) first_match = _cur;
			if (!pat[2]) return first_match;
			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
			else pat += 2; //one ?
		}
		else
		{
			pat = ida_sig;
			first_match = 0;
		}
	}
	return NULL;
}

create_interface retrieve_interface(LPCSTR module_name)
{
	return reinterpret_cast<create_interface>(GetProcAddress(GetModuleHandle(module_name), "CreateInterface"));
}

bool c_base_entity::trace_from_smoke(Vector src)
{
	typedef bool(*td_LineGoesThroughSmoke)(float, float, float, float, float, float, short);

	Vector dst = *this->m_Origin();
	static td_LineGoesThroughSmoke line_through_smoke = reinterpret_cast<td_LineGoesThroughSmoke>(g_weebware.pattern_scan("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"));

	if (!line_through_smoke)
		return false;

	return line_through_smoke(src.x, src.y, src.z, dst.x, dst.y, dst.z, 1);
}
