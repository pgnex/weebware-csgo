#include "Header.h"
#include "shared.h"
#include "esp.h"
#include "drawing.h"
#include "d3dx9esp.h"
#include "gui.h"
#include "imgui_custom.h"
#include "imgui\imgui_internal.h"
#include "imgui\imgui_impl_win32.h"
#include "hook_funcs.h"
#include <intrin.h>

using namespace imgui_custom;

// https://www.unknowncheats.me/forum/direct3d/66594-d3d9-vtables.html

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

c_d3dxesp g_d3dxesp;


LRESULT __stdcall hook_functions::hk_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg == WM_KEYDOWN) {
		if (wParam == VK_INSERT) {
			g_weebware.menu_opened = !g_weebware.menu_opened;
			g_weebware.g_input_system->EnableInput(!g_weebware.menu_opened);
		}
	}

	return (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) && g_weebware.menu_opened) ? 1 : CallWindowProcA(g_weebware.old_window_proc, hWnd, uMsg, wParam, lParam);
}


long hook_functions::present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND wnd_override, const RGNDATA* dirty_region)
{

	IDirect3DStateBlock9* pixelState = NULL;
	IDirect3DVertexDeclaration9* vertexDeclaration;
	IDirect3DVertexShader9* vertexShader;

	if (device->CreateStateBlock(D3DSBT_ALL, &pixelState) < 0)
		return g_hooking.o_present(device, src, dest, wnd_override, dirty_region);

	pixelState->Capture();
	device->GetVertexDeclaration(&vertexDeclaration);
	device->GetVertexShader(&vertexShader);


	// we do any drawing / rendering here..
	g_gui.render_menu(device);



	pixelState->Apply();
	pixelState->Release();
	device->SetVertexShader(vertexShader);
	device->SetVertexDeclaration(vertexDeclaration);

	return g_hooking.o_present(device, src, dest, wnd_override, dirty_region);
}

long hook_functions::end_scene(IDirect3DDevice9* device)
{

	IDirect3DStateBlock9* pixelState = NULL;
	IDirect3DVertexDeclaration9* vertexDeclaration;
	IDirect3DVertexShader9* vertexShader;

	if (device->CreateStateBlock(D3DSBT_ALL, &pixelState) < 0) {
		//return PLH::FnCast(g_hooking.endscene_tramp, g_hooking.o_endscene)(device);
		return g_hooking.o_endscene(device);
	}


	pixelState->Capture();
	device->GetVertexDeclaration(&vertexDeclaration);
	device->GetVertexShader(&vertexShader);


	// we do any drawing / rendering here..
	g_gui.render_menu(device);

	// if they DONT have screenshot proof on and user is taking a screenshot, render esp
	if (!(g_weebwarecfg.screenshot_proof && g_weebware.g_engine->is_taking_screenshot())) {
		g_d3dxesp.d9esp_main(device);
	}
	

	pixelState->Apply();
	pixelState->Release();
	device->SetVertexShader(vertexShader);
	device->SetVertexDeclaration(vertexDeclaration);

	//return PLH::FnCast(g_hooking.endscene_tramp, g_hooking.o_endscene)(device);
	return g_hooking.o_endscene(device);
}


long hook_functions::reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_param) 
{
	if (!g_gui.imgui_initialized) {
		return g_hooking.o_reset(device, presentation_param);
	}

	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto hr = g_hooking.o_reset(device, presentation_param);

	if (hr == D3D_OK) {
		ImGui_ImplDX9_CreateDeviceObjects();
		g_weebware.init_fonts();
		g_d3dxesp.d3dx_reset();
	}
	return hr;
}


static int tab_selection = 0;
void gui::imgui_main() {

	ImGui::GetIO().MouseDrawCursor = g_weebware.menu_opened;

	if (g_weebware.menu_opened)
	{
		int key_counter = 0;

		int mouse_x, mouse_y;
		g_weebware.g_input_system->GetCursorPosition(&mouse_x, &mouse_y);
		ImGuiIO& io = ImGui::GetIO();
		auto& style = ImGui::GetStyle();
		io.MousePos.x = (float)(mouse_x);
		io.MousePos.y = (float)(mouse_y);

		static int tab_selection = gui::tabs::legit;

		ImGui::PushFont(g_weebware.pFont[0]);

		ImGui::Begin("weebware", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize(ImVec2(800, 600));


		ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
		style.ItemSpacing = ImVec2(0, 6);
		ImGui::Columns(5, "columns", false);
		ImGui::SetColumnWidth(0, 100);
		ImGui::BeginChild("navbar", ImVec2(0, 0), false);

		imgui_custom::horizontal_margin("navtop", 20);


		// font icons
		ImGui::PushFont(g_weebware.pFont[2]);
		imgui_custom::create_button_tab(tab_selection, gui::tabs::legit, "A", ImGui::GetContentRegionAvailWidth(), 103);
		imgui_custom::create_button_tab(tab_selection, gui::tabs::vis, "B", ImGui::GetContentRegionAvailWidth(), 103);
		imgui_custom::create_button_tab(tab_selection, gui::tabs::misc, "C", ImGui::GetContentRegionAvailWidth(), 103);
		imgui_custom::create_button_tab(tab_selection, gui::tabs::skins, "D", ImGui::GetContentRegionAvailWidth(), 103);
		imgui_custom::create_button_tab(tab_selection, gui::tabs::settings, "E", ImGui::GetContentRegionAvailWidth(), 103);
		ImGui::PopFont();

		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		style.ItemSpacing = ImVec2(6, 6);

		// padding side
		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, 20);

		ImGui::NextColumn();


		if (tab_selection == gui::tabs::legit) {
			// begin tab
			const char* weapon_groups[] = { "Pistols", "Rifles", "SMG","Shotguns", "Heavy", "Auto-Snipers", "AWP", "SSG08" };
			imgui_custom::horizontal_margin("legit1", 6);
			int col1_width = ImGui::GetContentRegionAvailWidth() - 16;


			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 0, "Pistols", col1_width / 4, 20);
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 1, "Rifles", col1_width / 4, 20);
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 2, "SMG", col1_width / 4, 20);
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 3, "Shotguns", col1_width / 4, 20);
			ImGui::PopStyleVar();

			imgui_custom::horizontal_margin("legit11", 1);

			ImGui::SetColumnWidth(2, 328);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("legit 1", ImVec2(0, ImGui::GetContentRegionAvail().y - 24), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("legitgb1");
			imgui_custom::horizontal_margin("legittxtmargintop1", 2);
			imgui_custom::custom_label_header("Aimbot");

			ImGui::Text("Activation type");
			if (g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].enable_legitbot == 2)
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].legitbot_activation_key, key_counter);

			const char* activation_type_trigger[] = { "Off", "On Fire", "On Key", "Magnetic" };

			imgui_custom::a_better_combo_box("##aimcombo", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].enable_legitbot, activation_type_trigger, ARRAYSIZE(activation_type_trigger));
			ImGui::Checkbox("Silent aim", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].silent_aim);
			ImGui::Checkbox("Target teammates", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].target_teammates);
			ImGui::Checkbox("Distance FOV", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].use_dynamicfov);

			ImGui::Text("Maximum FOV");
			imgui_custom::a_better_slider_float("##Maximum FOV", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].maximum_fov, 0, 30, "%.1f");

			ImGui::Text("Sensitivity");
			imgui_custom::a_better_slider_float("##Sensitivity", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].sensitivity, 0, 100, "%.0f%%");

			ImGui::Text("First Shot Delay");
			imgui_custom::a_better_slider_float("##First Shot Delay", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].reaction_time, 0, 200, "%.0fms");

			ImGui::Text("Target Switch Delay");
			imgui_custom::a_better_slider_int("##Target Switch Delay", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].aimbot_target_switch_delay, 0, 2000, "%.0f");

			ImGui::Text("Recoil compensation (P/Y)");
			imgui_custom::a_better_slider_float("P##Recoil compensation P", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].pitch_rcs, 0, 100, "%.0f%%");
			imgui_custom::a_better_slider_float("Y##Recoil compensation Y", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].yaw_rcs, 0, 100, "%.0f%%");

			ImGui::Checkbox("Quickstop##legit", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].quick_stop);

			ImGui::Checkbox("Aim through smoke", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].aim_through_smoke);

			ImGui::Checkbox("Aim while blind", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].aim_while_blind);

			ImGui::Checkbox("Head", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].hitbox_head);
			ImGui::Checkbox("Chest", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].hitbox_chest);
			ImGui::Checkbox("Stomach", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].hitbox_stomach);
			ImGui::EndChild();

			ImGui::PopStyleColor();

			ImGui::NextColumn();

			// margin top column 2
			imgui_custom::horizontal_margin("legit2", 6);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 4, "Heavy", col1_width / 4, 20);
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 5, "Auto", col1_width / 4, 20);
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 6, "AWP", col1_width / 4, 20);
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 7, "SSGO8", col1_width / 4, 20);
			ImGui::PopStyleVar();

			imgui_custom::horizontal_margin("legit22", 1);

			ImGui::SetColumnWidth(3, 328);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));

			ImGui::BeginChild("legit 2", ImVec2(0, 340), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("legitgb2");
			imgui_custom::horizontal_margin("legittxtmargintop2", 2);
			imgui_custom::custom_label_header("Triggerbot");
			ImGui::Text("Activation type");

			if (g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_active == 1)
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_key, key_counter);


			const char* activation_type[] = { "Off", "On Key", "Active" };
			imgui_custom::a_better_combo_box("##triggercombo", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_active, activation_type, ARRAYSIZE(activation_type));
			ImGui::Text("Hitchance");
			imgui_custom::a_better_slider_float("##Hitchance", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_hitchance, 0, 100, "%.0f%%");
			ImGui::Text("Reaction time");
			imgui_custom::a_better_slider_float("##Delay", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_reaction, 0, 200, "%.0fms");

			ImGui::Checkbox("Head##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_head);
			ImGui::SameLine();
			ImGui::Checkbox("Chest##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_chest);
			ImGui::SameLine();
			ImGui::Checkbox("Stomach##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_stomach);
			ImGui::Checkbox("Arms##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_arms);
			ImGui::SameLine();
			ImGui::Checkbox("Legs##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_legs);
			imgui_custom::custom_label_header("Magnet Triggerbot");
			ImGui::Checkbox("Enable##magnet", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].magnet_triggerbot_enabled);
			ImGui::Checkbox("Quickstop##magnet", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].quick_stop_magnet);
			ImGui::Checkbox("Aim Through Smoke##magnet", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_aim_through_smoke);

			if (g_weebwarecfg.legit_cfg_index == 5 || g_weebwarecfg.legit_cfg_index == 6 || g_weebwarecfg.legit_cfg_index == 7) {
				ImGui::Checkbox("Scoped Only", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_scoped_only);
			}

			ImGui::Text("Maximum FOV");
			imgui_custom::a_better_slider_float("##FOVmagnettrigger", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].magnet_trigger_fov, 0, 180.f, "%.1f");
			ImGui::Text("Sensitivity");
			imgui_custom::a_better_slider_float("##Smoothmagnettrigger", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].magnet_trigger_smooth, 0, 100.f, "%.1f");
			ImGui::Text("Target Switch Delay");
			imgui_custom::a_better_slider_int("##Target Switch Delaytrigger", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_target_switch_delay, 0, 2000, "%.0f");

			ImGui::NewLine();

			ImGui::EndChild();

			imgui_custom::horizontal_margin("legitspace1", 4);

			ImGui::BeginChild("legit 3", ImVec2(0, ImGui::GetContentRegionAvail().y - 24), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("legitgb3");
			imgui_custom::horizontal_margin("legittxtmargintop3", 2);
			imgui_custom::custom_label_header("Other");
			ImGui::Checkbox("Standalone RCS", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].standalone_rcs);
			ImGui::Text("RCS Factor");
			imgui_custom::a_better_slider_float("##RCS Factor", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].standalone_rcs_power, 0, 100, "%.0f%%");
			ImGui::Checkbox("Backtracking", (bool*)&g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].accuracy_boost);
			ImGui::Text("Maximum Ticks");
			imgui_custom::a_better_slider_float("##Maximum Ticks", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].legit_maximum_ticks, 0, 30, "%.f%");
			ImGui::EndChild();

			ImGui::PopStyleColor();

			// padding side
			ImGui::NextColumn();
			ImGui::SetColumnWidth(4, 8);

			ImGui::EndColumns();
		}


		if (tab_selection == gui::tabs::vis) {
			// begin tab

	// margin top column 1
			imgui_custom::horizontal_margin("vis1");

			ImGui::SetColumnWidth(2, 328);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("misc 1", ImVec2(0, 340), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("visgb1");
			imgui_custom::horizontal_margin("vistxtmargintop1", 2);
			imgui_custom::custom_label_header("Players");

			ImGui::Text("Activation type");
			if (g_weebwarecfg.enable_visuals == 2) {
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.enable_visuals_key, key_counter);
			}

			const char* activation_type[] = { "Off", "Enabled", "On Key" };
			imgui_custom::a_better_combo_box("##activationtype", &g_weebwarecfg.enable_visuals, activation_type, ARRAYSIZE(activation_type));
			ImGui::Checkbox("Teammates", &g_weebwarecfg.visuals_teammates);
			if (g_weebwarecfg.visuals_teammates) {
				imgui_custom::custom_color_inline(g_weebwarecfg.team_visible_col, g_weebwarecfg.team_hidden_col, true, "Visible Color (Team)##Team1", "Hidden Color (Team)##Team2");
			}
			ImGui::Checkbox("Visible only", &g_weebwarecfg.visuals_visible_only);

			ImGui::Checkbox("Bounding Box", &g_weebwarecfg.visuals_bounding_box);
			if (g_weebwarecfg.visuals_bounding_box) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_bounding_col_visible, g_weebwarecfg.visuals_bounding_col_hidden, true, "Visible Color (Enemy)##box1", "Hidden Color (Enemy)##box2");
			}

			ImGui::Checkbox("Corner Box", &g_weebwarecfg.visuals_corner_box);
			if (g_weebwarecfg.visuals_corner_box) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_corner_col_visible, g_weebwarecfg.visuals_corner_col_hidden, true, "Visible Color (Enemy)##corner1", "Hidden Color (Enemy)##corner2");
			}

			ImGui::Checkbox("Skeleton", &g_weebwarecfg.visuals_skeleton);
			if (g_weebwarecfg.visuals_skeleton) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_skeleton_col_visible, g_weebwarecfg.visuals_skeleton_col_hidden, true, "Visible Color (Enemy)##skele1", "Hidden Color (Enemy)##skele2");
			}


			ImGui::Checkbox("Health Bar", &g_weebwarecfg.visuals_health_bars);

			ImGui::Checkbox("Name", &g_weebwarecfg.visuals_name_esp);
			if (g_weebwarecfg.visuals_name_esp) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_name_esp_col_visible, g_weebwarecfg.visuals_name_esp_col_hidden, true, "Visible Color (Enemy)##name1", "Hidden Color (Enemy)##name2");
			}

			ImGui::Checkbox("Weapon", &g_weebwarecfg.visuals_weapon_esp);
			if (g_weebwarecfg.visuals_weapon_esp) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_weapon_esp_col, "Weapon ESP Color");
			}

			ImGui::Checkbox("Ammo", &g_weebwarecfg.visuals_ammo_esp);
			if (g_weebwarecfg.visuals_ammo_esp) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_ammo_esp_col, "Ammo ESP Color");
			}

			ImGui::Checkbox("Defusing Indicator", &g_weebwarecfg.defusing_indicator);
			if (g_weebwarecfg.defusing_indicator) {
				imgui_custom::custom_color_inline(g_weebwarecfg.defusing_indicator_col, "dfcol##1");
			}

			ImGui::Checkbox("Draw On Dormant", &g_weebwarecfg.visuals_dormant_esp);
			if (g_weebwarecfg.visuals_dormant_esp) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_dormant_col, g_weebwarecfg.visuals_dormant_col_team, g_weebwarecfg.visuals_teammates, "Visible Color (Enemy)##dormant1", "Hidden Color (Enemy)##dormant2");
			}
			ImGui::Checkbox("Show On Radar", &g_weebwarecfg.visuals_bspotted);
			ImGui::EndChild();

			imgui_custom::horizontal_margin("visspace1", 4);

			ImGui::BeginChild("misc 2", ImVec2(0, ImGui::GetContentRegionAvail().y - 24), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("visgb2");
			imgui_custom::horizontal_margin("vistxtmargintop2", 2);
			imgui_custom::custom_label_header("Glow");
			ImGui::Checkbox("Enabled##Glow", &g_weebwarecfg.visuals_glow_enabled);

			ImGui::Checkbox("Players", &g_weebwarecfg.visuals_glow_player);
			if (g_weebwarecfg.visuals_glow_player) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_glow_player_col_visible, g_weebwarecfg.visuals_glow_player_col_hidden, g_weebwarecfg.visuals_glow_hidden_col, "Glow Color (Visible)", "Glow Color (Hidden)");
				ImGui::Checkbox("Hidden Color", &g_weebwarecfg.visuals_glow_hidden_col);
			}

			ImGui::Checkbox("Team##glow", &g_weebwarecfg.visuals_glow_team);
			if (g_weebwarecfg.visuals_glow_team) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_glow_team_col, "Team Glow Color");
			}

			ImGui::Checkbox("Weapon##glow", &g_weebwarecfg.visuals_glow_weapon);
			if (g_weebwarecfg.visuals_glow_weapon) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_glow_weapon_col, g_weebwarecfg.visuals_glow_weapon_col, false, "Weapon Color", "##glowwep");
			}

			ImGui::Checkbox("Bomb", &g_weebwarecfg.visuals_glow_c4);
			if (g_weebwarecfg.visuals_glow_c4) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_glow_c4_col, g_weebwarecfg.visuals_glow_c4_col, false, "Bomb Color", "##glowbomb");
			}

			ImGui::Checkbox("Chicken", &g_weebwarecfg.visuals_glow_chicken);
			if (g_weebwarecfg.visuals_glow_chicken) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_glow_chicken_col, g_weebwarecfg.visuals_glow_chicken_col, false, "Chicken Color", "##glow2");
			}
			ImGui::EndChild();

			ImGui::PopStyleColor();

			ImGui::NextColumn();

			// margin top column 2
			imgui_custom::horizontal_margin("vis2");

			ImGui::SetColumnWidth(3, 328);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("misc 3", ImVec2(0, 400), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("visgb3");
			imgui_custom::horizontal_margin("vistxtmargintop3", 2);
			imgui_custom::custom_label_header("Other Visuals");
			ImGui::Checkbox("Inaccuracy Circle", &g_weebwarecfg.visuals_inacc_circle);
			if (g_weebwarecfg.visuals_inacc_circle) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_innacc_circle_col, "Inacc Color");
			}

			ImGui::Checkbox("Sniper Crosshair", &g_weebwarecfg.visuals_sniper_crosshair);
			if (g_weebwarecfg.visuals_sniper_crosshair) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_sniper_crosshair_col, "Sniper Crosshair Color");
			}
			ImGui::Checkbox("Recoil Crosshair", &g_weebwarecfg.visuals_recoil_crosshair);
			if (g_weebwarecfg.visuals_recoil_crosshair) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_recoil_crosshair_col, "Recoil Crosshair Color");
			}

			ImGui::Checkbox("Watermark", &g_weebwarecfg.visuals_watermark);
			if (g_weebwarecfg.visuals_watermark)
				imgui_custom::custom_color_inline(g_weebwarecfg.water_mark_col, "watermark##1");
			ImGui::Checkbox("Bomb Timer", &g_weebwarecfg.visuals_bomb_timer);
			ImGui::Checkbox("Wireframe Smoke", &g_weebwarecfg.wireframe_smoke);
			ImGui::Checkbox("Night Sky", &g_weebwarecfg.night_sky);
			ImGui::Checkbox("Nightmode", &g_weebwarecfg.visuals_nightmode);
			ImGui::Checkbox("Screenshot Proof", &g_weebwarecfg.screenshot_proof);
			ImGui::Checkbox("No Smoke", &g_weebwarecfg.no_smoke);
			ImGui::Checkbox("Spectator List", &g_weebwarecfg.spec_list);
			ImGui::Checkbox("Grenade Trajectory", &g_weebwarecfg.draw_grenade_traj);
			ImGui::Checkbox("Hitmarkers", &g_weebwarecfg.visuals_hitmarkers);
			if (g_weebwarecfg.visuals_hitmarkers)
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_hitmarker_col, "Hitmarker Color");
			const char* hit_sound[] = { "None", "COD", "Anime", "Bubbles", "Custom" };
			if (g_weebwarecfg.visuals_hitmarkers) {
				ImGui::Text("Hitmarker Sound");
				imgui_custom::a_better_combo_box("##hitmarkersound", &g_weebwarecfg.hitmarker_sound, hit_sound, ARRAYSIZE(hit_sound));
			}
			ImGui::Checkbox("Bullet Tracers##bullettracers", &g_weebwarecfg.enable_bullet_tracers);
			if (g_weebwarecfg.enable_bullet_tracers) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_bullet_tracer_col, "btcol##1");
				ImGui::Text("Expire Time");
				imgui_custom::a_better_slider_float("##tracerexpire", &g_weebwarecfg.bullet_tracer_expire, 0, 20, "%.2f");
				ImGui::Text("Width");
				imgui_custom::a_better_slider_float("##Widthtracer", &g_weebwarecfg.bullet_tracer_width, 0, 10, "%.2f");
				ImGui::Text("Amplitude");
				imgui_custom::a_better_slider_float("##Amplitudetracer", &g_weebwarecfg.bullet_tracer_amplitude, 0, 10, "%.2f");
				ImGui::Text("Speed");
				imgui_custom::a_better_slider_float("##Speedtracer", &g_weebwarecfg.bullet_tracer_speed, 0, 2, "%.2f");
			}
			ImGui::Text("Backtracking Style");
			const char* backtrackstyle[] = { "Time", "Single", "All", "Target" };
			imgui_custom::a_better_combo_box("##backtrackingtype", &g_weebwarecfg.visuals_backtrack_style, backtrackstyle, ARRAYSIZE(backtrackstyle));
			ImGui::Checkbox("Backtrack Skeleton", &g_weebwarecfg.visuals_backtrack_dots);
			if (g_weebwarecfg.visuals_backtrack_dots) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_backtrack_col, "Backtrack Color");
			}

			imgui_custom::horizontal_margin("bottommarginlegit1", 4);
			ImGui::EndChild();

			imgui_custom::horizontal_margin("visspace2", 4);

			ImGui::BeginChild("misc 4", ImVec2(0, ImGui::GetContentRegionAvail().y - 24), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("miscgb4");
			imgui_custom::horizontal_margin("vistxtmargintop4", 2);
			imgui_custom::custom_label_header("Player Chams");
			const char* cham_type[] = { "Disabled", "Plain", "Glow", "Flat", "Glass", "Crystal", "Gold", "Crystal Blue" };
			ImGui::Text("Material");
			imgui_custom::a_better_combo_box("##chammaterials", &g_weebwarecfg.visuals_chams, cham_type, ARRAYSIZE(cham_type));
			if (g_weebwarecfg.visuals_chams > 0)
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_chams_col, g_weebwarecfg.visuals_chams == 2 ? g_weebwarecfg.visuals_chams_glow_col : g_weebwarecfg.visuals_chams_team_col, 1, "Enemy Color##chams1", g_weebwarecfg.visuals_chams == 2 ? "Glow Cham Color##chams2" : "Team Color##chams2");
			ImGui::Checkbox("Render Team", &g_weebwarecfg.visuals_chams_render_team);

			ImGui::Checkbox("XQZ (Through Materials)", &g_weebwarecfg.visuals_chams_xqz);
			if (g_weebwarecfg.visuals_chams_xqz)
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_chams_col_xqz, g_weebwarecfg.visuals_chams_team_col_xqz, 1, "Enemy XQZ Color##chams1", "Team XQZ Color##chams2");

			ImGui::EndChild();

			ImGui::PopStyleColor();

			// padding side
			ImGui::NextColumn();
			ImGui::SetColumnWidth(4, 8);

			ImGui::EndColumns();


			// end tab
		}

		if (tab_selection == gui::tabs::misc) {
			// begin tab

			// margin top column 1
			imgui_custom::horizontal_margin("misc1");

			ImGui::SetColumnWidth(2, 328);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("misc 1", ImVec2(0, 350), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("miscgb1");
			imgui_custom::horizontal_margin("misctxtmargintop1", 2);
			imgui_custom::custom_label_header("Misc");

			ImGui::Checkbox("Chatspam", &g_weebwarecfg.misc_chat_spammer);
			ImGui::Checkbox("Preserve Killfeed", &g_weebwarecfg.preserve_killfeed);
			ImGui::Checkbox("Rank Reveal", &g_weebwarecfg.rank_reveal);
			ImGui::Checkbox("Disable Post Processing", &g_weebwarecfg.disable_post_processing);
			ImGui::Checkbox("Anti AFK", &g_weebwarecfg.anti_afk);
			ImGui::Checkbox("Auto Accept", &g_weebwarecfg.misc_autoAccept);
			ImGui::Checkbox("Rainbow Name", &g_weebwarecfg.rainbow_name);

			ImGui::Checkbox("Clantag Changer", &g_weebwarecfg.misc_clantag_changer);
			if (g_weebwarecfg.misc_clantag_changer) {
				ImGui::InputTextWithHint("##Custom Clantag", "Enter Custom Clantag", g_weebwarecfg.custom_clantag_static, ARRAYSIZE(g_weebwarecfg.custom_clantag_static));
			}

			ImGui::Checkbox("Killsay", &g_weebwarecfg.killsay);
			if (g_weebwarecfg.killsay) {
				ImGui::InputTextWithHint("##Custom Killsay", "Enter Custom Killsay", g_weebwarecfg.killsay_msg_custom, ARRAYSIZE(g_weebwarecfg.killsay_msg_custom));
			}

			ImGui::Checkbox("Viewmodel Changer", &g_weebwarecfg.viewmodel_changer);
			if (g_weebwarecfg.viewmodel_changer) {
				imgui_custom::a_better_slider_int("##Viewmodel Offset", &g_weebwarecfg.viewmodel_offset, -100, 135);
			}

			ImGui::Checkbox("Block Bot", &g_weebwarecfg.block_bot);
			if (g_weebwarecfg.block_bot) {
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.block_bot_key, key_counter);
			}

			ImGui::Checkbox("Auto Pistol", &g_weebwarecfg.auto_pistol);
			if (g_weebwarecfg.auto_pistol) {
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.auto_pistol_key, key_counter);
			}

			ImGui::Checkbox("Auto Defuse", &g_weebwarecfg.auto_defuse);
			if (g_weebwarecfg.auto_defuse) {
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.auto_defuse_key, key_counter);
			}

			ImGui::EndChild();

			imgui_custom::horizontal_margin("miscspace1", 4);

			ImGui::BeginChild("misc 2", ImVec2(0, ImGui::GetContentRegionAvail().y - 24), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("miscgb2");
			imgui_custom::horizontal_margin("misctxtmargintop2", 2);
			imgui_custom::custom_label_header("Movement");
			ImGui::Checkbox("Infinite Duck", &g_weebwarecfg.no_duck_cooldown);
			ImGui::Checkbox("Slidewalk", &g_weebwarecfg.misc_slidewalk);
			ImGui::Checkbox("Bunnyhop", &g_weebwarecfg.auto_jump);
			if (g_weebwarecfg.auto_jump) {
				imgui_custom::a_better_slider_int("Hitchance##bhop", &g_weebwarecfg.auto_jump_hitchance, 0, 100, "%.0f%%");
			}
			ImGui::Checkbox("Edge Jump", &g_weebwarecfg.edge_jump);
			if (g_weebwarecfg.edge_jump)
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.edge_jump_key, key_counter);
			if (g_weebwarecfg.edge_jump) {
				ImGui::Checkbox("Duck In Air", &g_weebwarecfg.duck_in_air);
			}
			ImGui::Text("Auto Strafe");
			const char* strafe_type[] = { "Off", "Legit", "Fast" };
			imgui_custom::a_better_combo_box("##Autostrafe", &g_weebwarecfg.auto_strafe, strafe_type, ARRAYSIZE(strafe_type));

			ImGui::EndChild();

			ImGui::PopStyleColor();

			ImGui::NextColumn();

			// margin top column 2
			imgui_custom::horizontal_margin("misc2");

			ImGui::SetColumnWidth(3, 328);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("misc 3", ImVec2(0, 220), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("miscgb3");
			imgui_custom::horizontal_margin("misctxtmargintop3", 2);
			imgui_custom::custom_label_header("Smartbot");
			ImGui::Checkbox("Enabled", &g_weebwarecfg.misc_ai);
			ImGui::Checkbox("Random", &g_weebwarecfg.misc_ai_random);
			ImGui::Checkbox("Engage nearest enemy", &g_weebwarecfg.misc_ai_nearest);
			ImGui::Checkbox("Defuse bombs", &g_weebwarecfg.misc_ai_defuse);
			//	ImGui::Checkbox("Defend closest sites", &g_weebwarecfg.misc_ai_defend, false);
			ImGui::Text("Rotation Speed");
			imgui_custom::a_better_slider_float("##Roatation Speed", &g_weebwarecfg.misc_ai_rotationspeed, 0, 100, "%.0f%%");
			ImGui::Text("Aim Speed");
			imgui_custom::a_better_slider_float("##Aim Speedai", &g_weebwarecfg.misc_ai_aimspeed, 0, 100, "%.0f%%");

			ImGui::EndChild();

			imgui_custom::horizontal_margin("miscspace2", 4);

			ImGui::BeginChild("misc 4", ImVec2(0, ImGui::GetContentRegionAvail().y - 24), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("miscgb4");
			imgui_custom::horizontal_margin("misctxtmargintop4", 2);
			imgui_custom::custom_label_header("Fake Lag");
			ImGui::Text("Activation type");
			if (g_weebwarecfg.fake_lag == 1)
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.fake_lag_key, key_counter);

			const char* fakelag_type[] = { "Off", "On Key", "Always On" };
			imgui_custom::a_better_combo_box("##fakelagtype", &g_weebwarecfg.fake_lag, fakelag_type, ARRAYSIZE(fakelag_type));
			if (g_weebwarecfg.fake_lag > 0) {
				imgui_custom::a_better_slider_int("Fakelag Factor", &g_weebwarecfg.fake_lag_factor, 0, 6);
			}

			ImGui::EndChild();

			ImGui::PopStyleColor();

			// padding side
			ImGui::NextColumn();
			ImGui::SetColumnWidth(4, 8);

			ImGui::EndColumns();


			// end tab
		}

		if (tab_selection == gui::tabs::skins) {
			// begin tab

	// margin top column 1
			imgui_custom::horizontal_margin("skins1");

			ImGui::SetColumnWidth(2, 328);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("skins 1", ImVec2(0, 100), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("skinsgb1");
			imgui_custom::horizontal_margin("skinstxtmargintop1", 2);
			imgui_custom::custom_label_header("Knife Options");
			ImGui::Checkbox("Enabled##knifechanger", &g_weebwarecfg.knifechanger_enabled);
			const char* knives[] = { "Terrorist Knife", "Counter-Terrorist Knife", "Flip", "Gut", "Karambit", "M9 Bayonet", "Bayonet", "Huntsman", "Falchion", "Stiletto", "Ursus", "Navaja", "Talon", "Butterfly", "Shadow Daggers", "Bowie", "Classic", "Paracord", "Survival", "Skeleton", "Nomad" };
			imgui_custom::a_better_combo_box("##knifecombo", &g_weebwarecfg.selected_knife_index[1], knives, ARRAYSIZE(knives));
			g_weebwarecfg.selected_knife_index[0] = g_weebware.g_knife_list[g_weebwarecfg.selected_knife_index[1]].weapon_index;

			ImGui::EndChild();

			imgui_custom::horizontal_margin("skinsspace1", 4);

			ImGui::BeginChild("skins 2", ImVec2(0, 300), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("skinsgb2");
			imgui_custom::horizontal_margin("skinstxtmargintop2", 2);
			imgui_custom::custom_label_header("Glove Options");

			ImGui::Checkbox("Enabled##glovechanger", &g_weebwarecfg.glovechanger_enabled);
			const char* glove_models[] = { "Default", "Sport", "Hand Wraps", "Specialist", "Driver", "Moto", "Hydra", "Bloodhound" };
			imgui_custom::a_better_combo_box("##glovenames", &g_weebwarecfg.glove_model, glove_models, ARRAYSIZE(glove_models));
			ImGui::Text("Wear");
			imgui_custom::a_better_slider_float("##Weargloves", &g_weebwarecfg.glove_wearz, 0, 100, "%.0f%%");

			if (g_weebwarecfg.glove_model > 0) {
				std::vector<const char*> v = glove_changer.set_glove_skin_array();
				//ImGui::Combo("##gloveskins", &g_weebwarecfg.glove_skin, v.data(), v.size());
				int loopi = 0;
				for (auto skin_part : v)
				{
					std::string name = std::string(skin_part) + "##" + std::string(skin_part);
					if (ImGui::Selectable(name.c_str(), name == g_weebwarecfg.glove_skin_cur))
					{
						g_weebwarecfg.glove_skin = loopi;
						g_weebwarecfg.glove_skin_cur = name;
						g_weebwarecfg.skinchanger_apply_nxt = 1;
					}
					loopi++;
				}
			}

			ImGui::EndChild();

			imgui_custom::horizontal_margin("skinsspace2", 4);

			ImGui::BeginChild("skins 3", ImVec2(0, ImGui::GetContentRegionAvail().y - 24), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("skinsgb2");
			imgui_custom::horizontal_margin("skinstxtmargintop3", 2);
			imgui_custom::custom_label_header("Model Options");


			if (g_weebware.models_installed) {
				ImGui::Text("Player Models");
				const char* models[] = { "Off", "Reina Kousaka", "Yuno Gasai", "Kimono Luka", "Inori" };
				imgui_custom::a_better_combo_box("##model_type", &g_weebwarecfg.anime_model, models, ARRAYSIZE(models));
				// https://gamebanana.com/skins/148058

				ImGui::PushStyleColor(ImGuiCol_Button, ConvertFromRGBA(ImVec4(40, 40, 40, 155)));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ConvertFromRGBA(ImVec4(50, 50, 50, 155)));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);

				if (ImGui::Button("Apply", ImVec2(ImGui::GetContentRegionAvailWidth() / 1.5, 25))) {
					g_weebwarecfg.skinchanger_apply_nxt = 1;
				}
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
			}
			else {
				ImGui::Text("Please properly install models");
				if (ImGui::Button("Download"))
					ShellExecute(0, 0, "https://auth.weebware.net/dependencies/models.exe", 0, 0, SW_SHOW);
				if (ImGui::Button("Refresh"))
					g_weebware.models_installed = g_weebware.check_models_installed();
			}


			ImGui::EndChild();

			ImGui::PopStyleColor();

			ImGui::NextColumn();

			// margin top column 2
			imgui_custom::horizontal_margin("skins2");

			ImGui::SetColumnWidth(3, 328);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("skins 5", ImVec2(0, ImGui::GetContentRegionAvail().y - 24), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("skinsgb4");
			imgui_custom::horizontal_margin("skinstxtmargintop4", 2);
			imgui_custom::custom_label_header(g_weebwarecfg.weapon_option_name.c_str());

			ImGui::Checkbox("Enabled", &g_weebwarecfg.skinchanger_enabled);
			ImGui::Checkbox("StatTrak", &g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].stattrak_enabled);
			if (g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].stattrak_enabled)
				ImGui::InputInt("Kills", &g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].stattrak_kill_count);

			ImGui::Text("Nametag");
			ImGui::PushStyleColor(ImGuiCol_Text, ConvertFromRGBA(ImVec4(188, 188, 188, 200)));
			ImGui::InputText("##weaponname", g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].weapon_name, ARRAYSIZE(g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].weapon_name));
			ImGui::PopStyleColor();
			ImGui::Text("Wear");
			imgui_custom::a_better_slider_float("##Wearskins", &g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].m_wear, 0, 100, "%.0f%%");
			ImGui::PushStyleColor(ImGuiCol_Text, ConvertFromRGBA(ImVec4(188, 188, 188, 200)));
			ImGui::InputInt("Seed", &g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].m_seed);
			ImGui::PopStyleColor();
			
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 5);
			imgui_custom::custom_label_header("Skins");
			ImGui::PushStyleColor(ImGuiCol_Text, ConvertFromRGBA(ImVec4(188, 188, 188, 200)));
			ImGui::InputText("##Search Skin", g_config_list.skinchanger_skinsearch, ARRAYSIZE(g_config_list.skinchanger_skinsearch));
			ImGui::PopStyleColor();
			ImGui::PopItemWidth();

			auto skin_list = g_gui.filtered_skins();

			ImGui::BeginChild("Existing Skins", ImVec2(0, ImGui::GetContentRegionAvail().y - 40), false);
			// Enumerate skins at start of game and filter them out b4 drawing
			for (c_skinchanger::skin_type skin_part : skin_list)
			{
				std::string name = skin_part.name + "##" + std::to_string(skin_part.id);
				if (ImGui::Selectable(name.c_str(), g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].m_paint_kit == skin_part.id))
				{
					g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].m_paint_kit = skin_part.id;
					g_weebwarecfg.skinchanger_apply_nxt = 1;
				}
			}
			ImGui::EndChild();
			ImGui::PushStyleColor(ImGuiCol_Button, ConvertFromRGBA(ImVec4(40, 40, 40, 155)));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ConvertFromRGBA(ImVec4(50, 50, 50, 155)));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
			if (ImGui::Button("Force Update", ImVec2(ImGui::GetContentRegionAvailWidth() - 5, 25))) {
				g_weebwarecfg.skinchanger_apply_nxt = 1;
			}
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::EndChild();

			ImGui::PopStyleColor();

			// padding side
			ImGui::NextColumn();
			ImGui::SetColumnWidth(4, 8);

			ImGui::EndColumns();


			// end tab
		}

		if (tab_selection == gui::tabs::settings) {
			// begin tab

			// margin top column 1
			imgui_custom::horizontal_margin("cfggm1");

			ImGui::SetColumnWidth(2, 328);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("cfg 1", ImVec2(0, ImGui::GetContentRegionAvail().y - 24), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("cfggb1");
			imgui_custom::horizontal_margin("cfgtxtmargintop1", 2);
			imgui_custom::custom_label_header("Local Configs");
			ImGui::Text("New Config");
			ImGui::InputText("Config Name", g_config_list.cur_save_name, ARRAYSIZE(g_config_list.cur_save_name));


			ImGui::PushStyleColor(ImGuiCol_Button, ConvertFromRGBA(ImVec4(40, 40, 40, 155)));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ConvertFromRGBA(ImVec4(50, 50, 50, 155)));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
			if (ImGui::Button("Save", ImVec2(80, 20)))
			{
				g_config_list.save_weebware_config();
			}
			ImGui::PopStyleVar();

			ImGui::SameLine();


			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
			if (ImGui::Button("Refresh", ImVec2(80, 20))) 
			{
				g_config_list.update_all_configs();
			}
			ImGui::PopStyleVar();

			ImGui::BeginChild("Existing Configs", ImVec2(0, ImGui::GetFrameHeight() - 55), false);
			for (auto cfg : g_config_list.config_names)
			{
				if (!strstr(cfg.c_str(), "weebwareskins")) {
					if (ImGui::Selectable(g_gui.erase_sub_string(cfg, ".weebware").c_str(), g_config_list.cur_load_name == cfg.c_str()))
					{
						g_config_list.cur_load_name = cfg.c_str();
					}
				}
			}
			ImGui::EndChild();

			float config_width = ImGui::GetContentRegionAvailWidth() - 12;
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
			if (ImGui::Button("Update", ImVec2((config_width / 3) - 2, 20)))
			{
				g_config_list.save_existing_weebware();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load", ImVec2((config_width / 3) - 2, 20)))
			{
				g_config_list.load_weebware_config(g_config_list.cur_load_name);
				g_weebwarecfg.skinchanger_apply_nxt = 1;
			}
			ImGui::SameLine();
			if (ImGui::Button("Delete", ImVec2((config_width / 3) - 2, 20)))
			{
				g_config_list.delete_weebware_config();
			}
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
	

			ImGui::EndChild();

			ImGui::PopStyleColor();

			ImGui::NextColumn();

			// margin top column 2
			imgui_custom::horizontal_margin("cfg2");

			ImGui::SetColumnWidth(3, 328);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("cfg 3", ImVec2(0, 260), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("cfggb3");
			imgui_custom::horizontal_margin("cfgtxtmargintop3", 2);
			imgui_custom::custom_label_header("Your Configs");
			ImGui::BeginChild("Configs##yours", ImVec2(0, 180), false);
			int index_yours = 0;
			for (auto cfg : g_config_list.config_browser_yours)
			{
				if (ImGui::Selectable(cfg.c_str(), g_config_list.cur_config_browser_yours_name == cfg.c_str()))
				{
					g_config_list.cur_config_browser_yours_name = cfg.c_str();
					g_config_list.cur_id_yours = (g_config_list.your_configs_buffer[index_yours].at("id"));
				}
				index_yours++;
			}
			ImGui::EndChild();
			float your_config_width = ImGui::GetContentRegionAvailWidth() - 12;

			ImGui::PushStyleColor(ImGuiCol_Button, ConvertFromRGBA(ImVec4(40, 40, 40, 155)));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ConvertFromRGBA(ImVec4(50, 50, 50, 155)));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
			if (ImGui::Button("Refresh##yours", ImVec2(your_config_width / 3, 20)))
			{
				g_config_list.get_your_configs();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load##yours", ImVec2(your_config_width / 3, 20)))
			{
				g_config_list.load_config_from_memory(g_config_list.cur_id_yours);
				g_weebwarecfg.skinchanger_apply_nxt = 1;
			}
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::EndChild();

			ImGui::PopStyleColor();

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("cfg 4", ImVec2(0, ImGui::GetContentRegionAvail().y - 24), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("cfggb4");
			imgui_custom::horizontal_margin("cfgtxtmargintop3", 2);
			imgui_custom::custom_label_header("Favorited Configs");
			ImGui::BeginChild("Configs##fav", ImVec2(0, ImGui::GetFrameHeight() - 55), false);
			int index_fav = 0;
			for (auto cfg : g_config_list.config_browser_fav)
			{
				if (ImGui::Selectable(cfg.c_str(), g_config_list.cur_config_browser_fav_name == cfg.c_str()))
				{
					g_config_list.cur_config_browser_fav_name = cfg.c_str();
					g_config_list.cur_id_fav = (g_config_list.favorite_configs_buffer[index_fav].at("id"));
				}
				index_fav++;
			}
			ImGui::EndChild();
			float fav_config_width = ImGui::GetContentRegionAvailWidth() - 12;

			ImGui::PushStyleColor(ImGuiCol_Button, ConvertFromRGBA(ImVec4(40, 40, 40, 155)));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ConvertFromRGBA(ImVec4(50, 50, 50, 155)));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);
			if (ImGui::Button("Refresh##fav", ImVec2(fav_config_width / 3, 20)))
			{
				g_config_list.get_favorited_configs();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load##fav", ImVec2(fav_config_width / 3, 20)))
			{
				g_config_list.load_config_from_memory(g_config_list.cur_id_fav);
				g_weebwarecfg.skinchanger_apply_nxt = 1;
			}
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::EndChild();

			ImGui::PopStyleColor();

			// padding side
			ImGui::NextColumn();
			ImGui::SetColumnWidth(4, 8);

			ImGui::EndColumns();


			// end tab
		}

		ImGui::End();
		ImGui::PopFont();

	}
}
