#pragma once
#ifndef SHARED
#define SHARED

#include "Header.h"
#include "IViewRenderBeams.h"

typedef void* (__cdecl* create_interface)(const char*, int*);

class c_weebware
{
public:
	std::string g_user_name = "";
	void setup_thread();
	uintptr_t pattern_scan(const char* szModule, const char* szSignature);
	void init_fonts();
	bool models_installed = false;
	bool check_models_installed();
	bool round_end = false;
	bool send_packet = true;
	ImFont* pFont[3];
	QAngle real_angle;


	HMODULE h_module;
	c_engine_client* g_engine;
	c_entity_list* g_entlist;
	i_base_client* g_client;
	c_panel* g_panel;
	c_surface* g_surface;
	unsigned long* g_client_mode;
	IDirect3DDevice9* g_direct_x;
	c_input_system* g_input_system;
	c_input* g_input;
	c_phys_surface_props* g_surface_props;
	iv_model_info* g_model_info;
	i_engine_trace* g_engine_trace;
	c_debug_overlay* g_debug_overlay;
	c_global_vars* g_global_vars;
	c_render_view* g_render_view;
	unsigned long g_present_address;
	unsigned long g_reset_address;
	c_gamemovement* g_game_movement;
	c_prediction* g_prediction;
	c_move_helper* g_move_helper;
	c_iconvar* g_convars;
	i_effects* g_effects;
	c_mat_system* g_mat_sys;
	c_model_render* g_model_render;
	c_clientstate** g_client_state;
	i_game_event_manager* g_game_events;
	CGlowObjectManager* g_glow_obj_manager;
	uintptr_t* g_enginesound;
	IMDLCache* g_mdlcache;
	IViewRenderBeams* g_beams;
	uintptr_t g_animoffset;
	CNetworkStringTableContainer* g_NetworkContainer;
	CRender* g_render;
	HWND h_window;
	WNDPROC old_window_proc;
	unsigned long tahoma_font;
	unsigned long tahoma_font_large;

	bool menu_opened = false;
	bool pressed_keys[256];

	std::vector<c_skinchanger::skin_type> create_skin_list();
	std::vector<c_skinchanger::skin_type> g_skin_list;
	std::vector<c_skinchanger::skin_type> g_glove_list;

	std::vector<c_skinchanger::gun_type> create_gun_list();
	std::vector<c_skinchanger::gun_type> g_gun_list;

	std::vector<c_skinchanger::knife_type> create_knife_list();
	std::vector<c_skinchanger::knife_type> g_knife_list;


	bool call_full_update = false;
private:
	bool init_interfaces();
	void setup_debug_window();

	create_interface engine_fact;
	create_interface client_fact;
	create_interface surface_fact;
	create_interface vgui2_fact;
	create_interface mat_system_fact;
	create_interface vstd_lib_fact;
	create_interface steam_fact;
	create_interface cache_fact;
	create_interface phys_fact;
	create_interface input_fact;
};

extern c_weebware g_weebware;

class c_vars {
public:
	variable_t g_unload = variable_t("unload", 0.0f);

};
extern c_vars g_vars;


#endif
