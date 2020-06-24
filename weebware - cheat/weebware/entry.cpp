#include "Header.h"
#include "shared.h"
#include "netvars.h"
#include "drawing.h"
#include "hook_funcs.h"
#include "events.h"
#include "knife_proxy_hook.h"

#define WEEBWARE_RELEASE 0

GameEvents g_events;
c_weebware g_weebware;
create_interface retrieve_interface( LPCSTR module_name );

unsigned __stdcall entry_thread( void* v_arg )
{
	// run our thread
	g_weebware.setup_thread( );

	// close thread
	_endthreadex( 0 );

	return 0;
}
bool c_weebware::init_interfaces( )
{

	while (!(g_weebware.h_window = FindWindowW(L"Valve001", NULL))) { Sleep(250); }

	while (!GetModuleHandleA("serverbrowser.dll")) { Sleep(250); }


	engine_fact = retrieve_interface( "engine.dll" );
	client_fact = retrieve_interface( "client.dll" );
	surface_fact = retrieve_interface( "vguimatsurface.dll" );
	vgui2_fact = retrieve_interface( "vgui2.dll" );
	mat_system_fact = retrieve_interface( "materialsystem.dll" );
	vstd_lib_fact = retrieve_interface( "vstdlib.dll" );
	steam_fact = retrieve_interface( "steamclient.dll" );
	cache_fact = retrieve_interface( "datacache.dll" );
	phys_fact = retrieve_interface( "vphysics.dll" );
	input_fact = retrieve_interface( "inputsystem.dll" );
	match_fact = retrieve_interface("matchmaking.dll");

	networking::curl_init();

#if WEEBWARE_RELEASE
	g_user_name = auth::GetServerVariable( auth::base64_decode( "ZG9n" ).c_str( ) );
	g_engine = reinterpret_cast<c_engine_client*>(engine_fact( auth::GetServerVariable( auth::base64_decode( "cmF0" ) ).c_str( ), NULL ));
	g_client = reinterpret_cast<i_base_client*>(client_fact( auth::GetServerVariable( auth::base64_decode( "Y2F0" ) ).c_str( ), NULL ));
#else
	g_engine = reinterpret_cast<c_engine_client*>(engine_fact( "VEngineClient014", NULL ));
	g_client = reinterpret_cast<i_base_client*>(client_fact( "VClient018", NULL ));
#endif

	g_client_mode = **(unsigned long***)(pattern_scan( "client.dll", "8B 0D ? ? ? ? FF 75 08 8B 01 FF 50 64 BA" ) + 0x2);
	g_entlist = reinterpret_cast<c_entity_list*>(client_fact( "VClientEntityList003", NULL ));
	g_panel = reinterpret_cast<c_panel*>(vgui2_fact( "VGUI_Panel009", NULL ));
	g_matchframework = reinterpret_cast<c_match_framework*>(match_fact("MATCHFRAMEWORK_001", NULL));
	g_surface = reinterpret_cast<c_surface*>(surface_fact( "VGUI_Surface031", NULL ));
	g_direct_x = **reinterpret_cast<IDirect3DDevice9***>(pattern_scan( "shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 0x1);
	g_glow_obj_manager = *reinterpret_cast<CGlowObjectManager**>(pattern_scan( "client.dll", "0F 11 05 ? ? ? ? 83 C8 01" ) + 3);
	g_input_system = reinterpret_cast<c_input_system*>(input_fact( "InputSystemVersion001", NULL ));
	g_model_info = reinterpret_cast<iv_model_info*>(engine_fact( "VModelInfoClient004", NULL ));
	g_engine_trace = reinterpret_cast<i_engine_trace*>(engine_fact( "EngineTraceClient004", NULL ));
	g_debug_overlay = reinterpret_cast<c_debug_overlay*>(engine_fact( "VDebugOverlay004", NULL ));
	g_global_vars = *reinterpret_cast<c_global_vars**>(((*(PDWORD*)g_client)[0]) + 0x1B);
	g_global_vars = reinterpret_cast<c_global_vars*>(*(PDWORD)g_global_vars);
	g_render_view = reinterpret_cast<c_render_view*>(engine_fact( "VEngineRenderView014", NULL ));
	g_mat_sys = reinterpret_cast<c_mat_system*>(mat_system_fact( "VMaterialSystem080", NULL ));
	g_client_state = *reinterpret_cast<c_clientstate***>(getvfunc<std::uintptr_t>( g_engine, 12 ) + 0x10);
	g_model_render = reinterpret_cast<c_model_render*>(engine_fact( "VEngineModel016", NULL ));
	g_mdlcache = reinterpret_cast<IMDLCache*>(cache_fact( "MDLCache004", NULL ));
	g_NetworkContainer = reinterpret_cast<CNetworkStringTableContainer*>(engine_fact( "VEngineClientStringTable001", NULL ));
	g_beams = *reinterpret_cast<IViewRenderBeams**>(pattern_scan( "client.dll", "8D 04 24 50 A1 ? ? ? ? B9" ) + 5);
	g_animoffset = *(uintptr_t*)((uintptr_t)pattern_scan( "client.dll", "8B 8E ? ? ? ? F3 0F 10 48 04 E8 ? ? ? ? E9" ) + 0x2);
	g_present_address = pattern_scan("gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 DB") + 0x2;
	g_reset_address = pattern_scan( "gameoverlayrenderer.dll", "C7 45 ? ? ? ? ? FF 15 ? ? ? ? 8B F8" ) + 9;
	// wtf is the past address kite >? new ? (C7 45 ? ? ? ? ? FF 15 ? ? ? ? 8B F8)
	g_game_movement = reinterpret_cast<c_gamemovement*>(client_fact( "GameMovement001", NULL ));
	g_prediction = reinterpret_cast<c_prediction*>(client_fact( "VClientPrediction001", NULL ));
	g_move_helper = **(c_move_helper***)(pattern_scan( "client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) + 0x2);
	g_convars = reinterpret_cast<c_iconvar*>(vstd_lib_fact( "VEngineCvar007", NULL ));
	g_effects = reinterpret_cast<i_effects*>(client_fact( "IEffects001", NULL ));
	g_game_events = reinterpret_cast<i_game_event_manager*>(engine_fact( "GAMEEVENTSMANAGER002", NULL ));
	g_enginesound = reinterpret_cast<uintptr_t*>(engine_fact( "IEngineSoundClient003", NULL ));
	g_input = *(c_input**)(pattern_scan( "client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10" ) + 0x1);
	g_surface_props = reinterpret_cast<c_phys_surface_props*>(phys_fact( "VPhysicsSurfaceProps001", NULL ));
	g_render = *reinterpret_cast<CRender**>(pattern_scan("engine.dll", "56 57 0F 57 C0 C7 05 ? ? ? ? ? ? ? ? 51") + 0x7);
	g_weapon_system = *(IWeaponSystem**)(pattern_scan("client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0") + 0x2);
	
	// lets make sure models are installed to display on menu
	models_installed = check_models_installed( );

	// Load our meme database for our p netvars
	netvar_manager::_instance( )->create_database( );

	g_weebwarecfg = c_weebwarecfg( );

	init_fonts( );

	g_config_list.update_all_configs( );
#if WEEBWARE_RELEASE
	g_config_list.get_favorited_configs();
	g_config_list.get_your_configs();
#endif

	// initialise 
	for ( auto i = 0; i < 35; i++ ) {
		g_weebwarecfg.skin_wheel[i] = override_skin_style( );
		g_weebwarecfg.skin_wheel[i].weapon_id = i;
	}

	g_skin_list = create_skin_list( );
	g_gun_list = create_gun_list( );
	g_knife_list = create_knife_list( );


	g_events.init( );

	srand( time( 0 ) );

	return true;
}

void c_weebware::init_fonts( ) {

	static bool init = false;

	if (!init) {
		tahoma_font = g_weebware.g_surface->create_font();
		tahoma_font_large = g_weebware.g_surface->create_font();
		indicator_font_XD = g_weebware.g_surface->create_font();
		g_weebware.g_surface->setfontglyphset(tahoma_font, "Tahoma", 11, 300, 0, 0, fontflag_antialias | fontflag_dropshadow);
		g_weebware.g_surface->setfontglyphset(tahoma_font_large, "Tahoma", 16, 400, 0, 0, fontflag_antialias | fontflag_dropshadow);
		g_weebware.g_surface->setfontglyphset(indicator_font_XD, "Tahoma", 48, 400, 0, 0, fontflag_antialias | fontflag_dropshadow | fontflag_outline);
		init = true;
	} else {
		g_weebware.g_surface->setfontglyphset(tahoma_font, "Tahoma", 11, 300, 0, 0, fontflag_antialias | fontflag_dropshadow);
		g_weebware.g_surface->setfontglyphset(tahoma_font_large, "Tahoma", 16, 400, 0, 0, fontflag_antialias | fontflag_dropshadow);
		g_weebware.g_surface->setfontglyphset(indicator_font_XD, "Tahoma", 48, 400, 0, 0, fontflag_antialias | fontflag_dropshadow | fontflag_outline);
	}
}

void c_weebware::setup_thread( ) {

#if !WEEBWARE_RELEASE
	setup_debug_window( );
#endif

	if ( init_interfaces( ) ) {
		//netvar_manager::_instance()->dump("latest.txt");
		g_hooking.hook_all_functions( );
	}

	while (true) {
		if (g_vars.g_unload.get() != 0.0f)
			break;

		if (g_vars.g_initfont.get() == 0.0f)
			continue;

		init_fonts();

		Sleep(250);
	}

	g_hooking.unhook_all_functions( );
	printf( "\nUnloading\n" );
	FreeConsole( );
	Sleep( 1500 ); // allow thread finishing time
	FreeLibraryAndExitThread( h_module, 0 );
}

void c_weebware::setup_debug_window( ) {
	AllocConsole( );
	freopen( "CONIN$", "r", stdin );
	freopen( "CONOUT$", "w", stdout );
	freopen( "CONOUT$", "w", stderr );
	SetConsoleTitle( "weebware debug console" );
}

std::vector<c_skinchanger::gun_type> c_weebware::create_gun_list( ) {
	std::ifstream skin_file( "C://weebware//dependencies//guns.txt" );
	std::string cur_line = "";
	std::vector<c_skinchanger::gun_type> gun_list;
	while ( std::getline( skin_file, cur_line ) )
	{
		std::string skin_name = strstr( cur_line.c_str( ), ":" );
		auto name_length = skin_name.length( );
		skin_name = skin_name.substr( 2 );
		auto id_len = cur_line.length( ) - name_length;
		auto id = cur_line.substr( 0, id_len );
		auto iId = std::stoi( id );
		gun_list.push_back( { iId, skin_name } );
	}
	return gun_list;
}

std::vector<c_skinchanger::skin_type> c_weebware::create_skin_list( ) {
	std::ifstream skin_file( "C://weebware//dependencies//skins.txt" );
	std::string cur_line = "";
	std::vector<c_skinchanger::skin_type> skin_list;
	while ( std::getline( skin_file, cur_line ) )
	{
		std::string skin_name = strstr( cur_line.c_str( ), ":" );
		auto name_length = skin_name.length( );
		skin_name = skin_name.substr( 2 );
		auto id_len = cur_line.length( ) - name_length;
		auto id = cur_line.substr( 0, id_len );
		auto iId = std::stoi( id );
		skin_list.push_back( { iId, skin_name } );
	}
	return skin_list;
}

std::vector<c_skinchanger::knife_type> c_weebware::create_knife_list() {
	std::vector<c_skinchanger::knife_type> tmp;

	auto knife_template = [](std::string mdl_file, std::string weapon_name, int defindex) {
		c_skinchanger::knife_type tmp;
		tmp.mdl = mdl_file;
		tmp.weapon_name = weapon_name;
		tmp.weapon_index = defindex;
		return tmp;
	};

	tmp.push_back(knife_template("models/weapons/v_knife_default_t.mdl", "Terrorist Knife", weapon_knife_t));
	tmp.push_back(knife_template("models/weapons/v_knife_default_ct.mdl", "Counter-Terrorist Knife", weapon_knife));
	tmp.push_back(knife_template("models/weapons/v_knife_flip.mdl", "Flip", weapon_knife_flip));
	tmp.push_back(knife_template("models/weapons/v_knife_gut.mdl", "Gut", weapon_knife_gut));
	tmp.push_back(knife_template("models/weapons/v_knife_karam.mdl", "Karambit", weapon_knife_karambit));
	tmp.push_back(knife_template("models/weapons/v_knife_m9_bay.mdl", "M9 Bayonet", weapon_knife_m9_bayonet));
	tmp.push_back(knife_template("models/weapons/v_knife_bayonet.mdl", "Bayonet", weapon_knife_bayonet));
	tmp.push_back(knife_template("models/weapons/v_knife_tactical.mdl", "Huntsman", weapon_knife_tactical));
	tmp.push_back(knife_template("models/weapons/v_knife_falchion_advanced.mdl", "Falchion", weapon_knife_falchion));
	tmp.push_back(knife_template("models/weapons/v_knife_stiletto.mdl", "Stiletto", weapon_knife_stiletto));
	tmp.push_back(knife_template("models/weapons/v_knife_ursus.mdl", "Ursus", weapon_knife_ursus));
	tmp.push_back(knife_template("models/weapons/v_knife_gypsy_jackknife.mdl", "Navaja", weapon_knife_gypsy_jackknife));
	tmp.push_back(knife_template("models/weapons/v_knife_widowmaker.mdl", "Talon", weapon_knife_widowmaker));
	tmp.push_back(knife_template("models/weapons/v_knife_butterfly.mdl", "Butterfly", weapon_knife_butterfly));
	tmp.push_back(knife_template("models/weapons/v_knife_push.mdl", "Shadow Daggers", weapon_knife_push));
	tmp.push_back(knife_template("models/weapons/v_knife_survival_bowie.mdl", "Bowie", weapon_knife_survival_bowie));
	tmp.push_back(knife_template("models/weapons/v_knife_css.mdl", "Classic Knife", weapon_knife_css));
	tmp.push_back(knife_template("models/weapons/v_knife_cord.mdl", "Paracord", weapon_knife_cord));
	tmp.push_back(knife_template("models/weapons/v_knife_canis.mdl", "Nomad", weapon_knife_canis));
	tmp.push_back(knife_template("models/weapons/v_knife_skeleton.mdl", "Skeleton", weapon_knife_skeleton));
	tmp.push_back(knife_template("models/weapons/v_knife_outdoor.mdl", "Survival", weapon_knife_outdoor));

	return tmp;
}

bool c_weebware::check_models_installed( ) {
	std::string path = std::filesystem::current_path( ).string( );
	return (
		std::filesystem::exists( path + "/csgo/models/player/custom_player/caleon1/reinakousaka/reina_blue.mdl" ) &&
		std::filesystem::exists( path + "/csgo/models/player/custom_player/voikanaa/mirainikki/gasaiyono.mdl" ) &&
		std::filesystem::exists( path + "/csgo/models/player/custom_player/caleon1/reinakousaka/reina_red.mdl" ) &&
		std::filesystem::exists( path + "/csgo/models/player/custom_player/bbs_93x_net_2016/kimono_luka/update_2016_08_05/kimono_luka.mdl" ) &&
		std::filesystem::exists( path + "/csgo/models/player/custom_player/monsterko/inori_yuzuriha/inori.mdl" )
		);
}

DWORD c_weebware::GetCallOffset(DWORD offset)
{
	auto call = *(DWORD*)((DWORD)offset + 1);
	return (DWORD)((DWORD)offset + call + 5);
}

DWORD c_weebware::pattern_scan_from_call(const char* module, const char* signature, DWORD offset)
{
	auto pat_offset = (DWORD)g_weebware.pattern_scan(module, signature);
	return GetCallOffset(pat_offset) + offset;
}

// credits: learn_more
uintptr_t c_weebware::pattern_scan( const char* model_name, const char* ida_sig ) {

#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

	MODULEINFO modinfo;
	GetModuleInformation( GetCurrentProcess( ), GetModuleHandleA( model_name ), &modinfo, sizeof( MODULEINFO ) );
	uintptr_t start_address = (uintptr_t)modinfo.lpBaseOfDll;
	uintptr_t endAddress = start_address + modinfo.SizeOfImage;
	const char* pat = ida_sig;
	uintptr_t first_match = 0;
	for ( uintptr_t _cur = start_address; _cur < endAddress; _cur++ )
	{
		if ( !*pat ) return first_match;
		if ( *(PBYTE)pat == '\?' || *(BYTE*)_cur == getByte( pat ) )
		{
			if ( !first_match ) first_match = _cur;
			if ( !pat[2] ) return first_match;
			if ( *(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?' ) pat += 3;
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

create_interface retrieve_interface( LPCSTR module_name ) {
	return reinterpret_cast<create_interface>(GetProcAddress( GetModuleHandle( module_name ), "CreateInterface" ));
}

bool c_base_entity::trace_from_smoke( Vector src ) {
	typedef bool(*td_LineGoesThroughSmoke)(float, float, float, float, float, float, short);

	Vector dst = *this->m_vecOrigin( );
	static td_LineGoesThroughSmoke line_through_smoke = reinterpret_cast<td_LineGoesThroughSmoke>(g_weebware.pattern_scan( "client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0" ));

	if ( !line_through_smoke )
		return false;

	return line_through_smoke( src.x, src.y, src.z, dst.x, dst.y, dst.z, 1 );
}
