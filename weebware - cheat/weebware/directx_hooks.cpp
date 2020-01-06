#include "Header.h"
#include "shared.h"
#include "esp.h"
#include "imgui_custom.h"
#include "imgui\imgui_internal.h"
#include "hook_funcs.h"
#include <intrin.h>
#include "imgui/imgui_impl_win32.h"
#include "gui.h"

#define ICON_MIN_FA 0xf000
#define ICON_MAX_FA 0xf496

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// https://www.unknowncheats.me/forum/direct3d/66594-d3d9-vtables.html

#pragma region Prototyping
bool has_d3d = false;
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


void imgui_main(IDirect3DDevice9* pDevice);
void style_setup();
void font_setup();
#pragma endregion

using namespace imgui_custom;

LRESULT __stdcall hook_functions::hk_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		g_weebware.pressed_keys[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		g_weebware.pressed_keys[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		g_weebware.pressed_keys[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		g_weebware.pressed_keys[VK_RBUTTON] = false;
		break;
	case WM_MBUTTONDOWN:
		g_weebware.pressed_keys[VK_MBUTTON] = true;
		break;
	case WM_MBUTTONUP:
		g_weebware.pressed_keys[VK_MBUTTON] = false;
		break;
	case WM_XBUTTONDOWN:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			g_weebware.pressed_keys[VK_XBUTTON1] = true;
		}
		else if (button == XBUTTON2)
		{
			g_weebware.pressed_keys[VK_XBUTTON2] = true;
		}
		break;
	}
	case WM_XBUTTONUP:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			g_weebware.pressed_keys[VK_XBUTTON1] = false;
		}
		else if (button == XBUTTON2)
		{
			g_weebware.pressed_keys[VK_XBUTTON2] = false;
		}
		break;
	}
	case WM_KEYDOWN:
		g_weebware.pressed_keys[wParam] = true;
		break;
	case WM_KEYUP:
		g_weebware.pressed_keys[wParam] = false;
		break;
	case WM_SYSKEYDOWN:
		if (wParam == VK_MENU)
			g_weebware.pressed_keys[VK_MENU] = true;
		break;
	case WM_SYSKEYUP:
		if (wParam == VK_MENU)
			g_weebware.pressed_keys[VK_MENU] = false;
		break;
	default: break;
	}

	static bool is_down = false;
	static bool is_clicked = false;

	if (g_weebware.pressed_keys[VK_INSERT])
	{
		is_clicked = false;
		is_down = true;
	}
	else if (!g_weebware.pressed_keys[VK_INSERT] && is_down)
	{
		is_clicked = true;
		is_down = false;
	}
	else
	{
		is_clicked = false;
		is_down = false;
	}

	if (is_clicked)
	{
		g_weebware.menu_opened = !g_weebware.menu_opened;
		g_weebware.g_input_system->EnableInput(!g_weebware.menu_opened);
	}

	if (g_weebware.menu_opened)
		ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam);

	return CallWindowProc(g_weebware.old_window_proc, hWnd, uMsg, wParam, lParam);
}



std::string eraseSubStr(std::string mainStr, std::string toErase)
{
	// Search for the substring in string
	size_t pos = mainStr.find(toErase);

	if (pos != std::string::npos)
		mainStr.erase(pos, toErase.length());

	return mainStr;

}

GUI::GUI() noexcept {
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(FindWindowW(L"Valve001", NULL));
	style_setup();
	font_setup();
}


long hook_functions::present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND wnd_override, const RGNDATA* dirty_region)
{

	return 0;
}


// Call imgui setup
extern unsigned int icon_font_i_think[71052 / 4];
static const unsigned int icon_font_compressed_size = 71051;

void style_setup()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(0, 0);
	style->WindowRounding = 0.f;
	style->FramePadding = ImVec2(0, 0);
	style->TextPaddingY = 1;
	style->FrameRounding = 0.f;
	style->ItemSpacing = ImVec2(6, 6);
	style->IndentSpacing = 2.0f;
	style->ScrollbarSize = 8.f;
	style->ScrollbarRounding = 0.f;
	style->GrabMinSize = 5.0f;
	style->ItemInnerSpacing = ImVec2(4, 5);
	style->ChildRounding = 10.f;

	// NORMAL THEME (0.541, 0.169, 0.886)
	// HALLOWEEN THEME (0.922, 0.482, 0.102)

	style->Colors[ImGuiCol_Text] = ConvertFromRGBA(ImVec4(92, 92, 92, 255));
	style->Colors[ImGuiCol_TextDisabled] = ConvertFromRGBA(ImVec4(120, 120, 120, 255));
	style->Colors[ImGuiCol_WindowBg] = imgui_custom::ConvertFromRGBA(ImVec4(28, 28, 28, 255.f));
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.15f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = imgui_custom::ConvertFromRGBA(ImVec4(60.f, 60.f, 60.f, 255.f));
	style->Colors[ImGuiCol_FrameBgHovered] = imgui_custom::ConvertFromRGBA(ImVec4(60.f, 60.f, 60.f, 255.f));
	style->Colors[ImGuiCol_FrameBgActive] = imgui_custom::ConvertFromRGBA(ImVec4(60.f, 60.f, 60.f, 255.f));
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = imgui_custom::ConvertFromRGBA(ImVec4(17.f, 17.f, 17.f, 255.f));
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 255.f));
	style->Colors[ImGuiCol_SliderGrab] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 255.f));
	style->Colors[ImGuiCol_SliderGrabActive] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 200.0f));
	style->Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
	style->Colors[ImGuiCol_PopupBg] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
	style->Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
	style->Colors[ImGuiCol_ButtonActive] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 255.f));
	style->Colors[ImGuiCol_Header] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 100.0f));
	style->Colors[ImGuiCol_HeaderHovered] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 255.f));
	style->Colors[ImGuiCol_HeaderActive] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 255.f));
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ConvertFromRGBA(ImVec4(52, 52, 52, 255));
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}

void font_setup() {
	ImGuiIO& io = ImGui::GetIO();
	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig config;
	config.MergeMode = true;
	g_weebware.pFont[0] = io.Fonts->AddFontFromMemoryCompressedTTF(raleway_compressed_data, raleway_compressed_size, 14);
	g_weebware.pFont[1] = io.Fonts->AddFontFromMemoryCompressedTTF(raleway_compressed_data, raleway_compressed_size, 24);
	g_weebware.pFont[2] = io.Fonts->AddFontFromMemoryCompressedTTF(icon_font_i_think, icon_font_compressed_size, 36, &config, icon_ranges);
}


enum tabs {
	legit,
	//rage,
	vis,
	misc,
	skins,
	settings
};


static int tab_selection = 0;
static bool boolin = false;
void GUI::render() noexcept {

	if (g_weebware.menu_opened)
	{

		int key_counter = 0;

		ImGui::GetIO().MouseDrawCursor = g_weebware.menu_opened;
		ImGui_ImplDX9_NewFrame();

		int mouse_x, mouse_y;
		g_weebware.g_input_system->GetCursorPosition(&mouse_x, &mouse_y);
		ImGuiIO& io = ImGui::GetIO();
		auto& style = ImGui::GetStyle();
		io.MousePos.x = (float)(mouse_x);
		io.MousePos.y = (float)(mouse_y);

		static int tab_selection = tabs::legit;

		ImGui::PushFont(g_weebware.pFont[0]);

		ImGui::Begin("weebware", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize(ImVec2(800, 600));


		ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
		style.ItemSpacing = ImVec2(0, 6);
		ImGui::Columns(5, "columns", false);
		ImGui::SetColumnWidth(0, 100);
		ImGui::BeginChild("navbar", ImVec2(0, 0), false);

		imgui_custom::horizontal_margin("navtop");

		imgui_custom::create_button_tab(tab_selection, legit, "Legit", ImGui::GetContentRegionAvailWidth(), 103);
		imgui_custom::create_button_tab(tab_selection, vis, "Visuals", ImGui::GetContentRegionAvailWidth(), 103);
		imgui_custom::create_button_tab(tab_selection, misc, "Misc", ImGui::GetContentRegionAvailWidth(), 103);
		imgui_custom::create_button_tab(tab_selection, skins, "Skins", ImGui::GetContentRegionAvailWidth(), 103);
		imgui_custom::create_button_tab(tab_selection, settings, "Settings", ImGui::GetContentRegionAvailWidth(), 103);

		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		style.ItemSpacing = ImVec2(6, 6);

		// padding side
		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, 20);

		ImGui::NextColumn();


		if (tab_selection == legit) {
			// begin tab
			const char* weapon_groups[] = { "Pistols", "Rifles", "SMG","Shotguns", "Heavy", "Auto-Snipers", "AWP", "SSG08" };
			imgui_custom::horizontal_margin("legit1", 5);
			int col1_width = ImGui::GetContentRegionAvailWidth() - 16;
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 0, "Pistols", col1_width / 4, 20);
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 1, "Rifles", col1_width / 4, 20);
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 2, "SMG", col1_width / 4, 20);
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 3, "Shotguns", col1_width / 4, 20);
			imgui_custom::horizontal_margin("legit11", 5);



			ImGui::SetColumnWidth(2, 325);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("legit 1", ImVec2(0, ImGui::GetContentRegionAvail().y - 28), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("legitgb1");
			imgui_custom::custom_label_header("Aimbot");

			ImGui::Text("Activation type");
			if (g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].enable_legitbot == 2)
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].legitbot_activation_key, key_counter);

			const char* activation_type_trigger[] = { "Off", "On Fire", "On Key", "Magnetic" };

			ImGui::Combo("", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].enable_legitbot, activation_type_trigger, ARRAYSIZE(activation_type_trigger));
			ImGui::Checkbox("Silent aim", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].silent_aim);
			ImGui::Checkbox("Target teammates", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].target_teammates);
			ImGui::Checkbox("Distance FOV", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].use_dynamicfov);

			ImGui::Text("Maximum FOV");
			ImGui::SliderFloat("##Maximum FOV", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].maximum_fov, 0, 30, "%.1f");

			ImGui::Text("Sensitivity");
			ImGui::SliderFloat("##Sensitivity", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].sensitivity, 0, 100, "%.0f%%");

			ImGui::Text("First Shot Delay");
			ImGui::SliderFloat("##First Shot Delay", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].reaction_time, 0, 200, "%.0fms");

			ImGui::Text("Target Switch Delay");
			ImGui::SliderInt("##Target Switch Delay", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].aimbot_target_switch_delay, 0, 2000, "%.0f");

			ImGui::Text("Recoil compensation (P/Y)");
			ImGui::SliderFloat("P##Recoil compensation P", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].pitch_rcs, 0, 100, "%.0f%%");
			ImGui::SliderFloat("Y##Recoil compensation Y", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].yaw_rcs, 0, 100, "%.0f%%");

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
			imgui_custom::horizontal_margin("legit2", 5);
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 4, "Heavy", col1_width / 4, 20);
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 5, "Auto", col1_width / 4, 20);
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 6, "AWP", col1_width / 4, 20);
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 7, "SSGO8", col1_width / 4, 20);

			imgui_custom::horizontal_margin("legit22", 5);

			ImGui::SetColumnWidth(3, 325);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));

			ImGui::BeginChild("legit 2", ImVec2(0, 340), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("legitgb2");
			imgui_custom::custom_label_header("Triggerbot");
			ImGui::Text("Activation type");

			if (g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_active == 1)
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_key, key_counter);


			const char* activation_type[] = { "Off", "On Key", "Active" };
			ImGui::Combo("", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_active, activation_type, ARRAYSIZE(activation_type));
			ImGui::Text("Hitchance");
			ImGui::SliderFloat("##Hitchance", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_hitchance, 0, 100, "%.0f%%");
			ImGui::Text("Reaction time");
			ImGui::SliderFloat("##Delay", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_reaction, 0, 200, "%.0fms");

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
				ImGui::Checkbox("Scoped Only", &g_weebwarecfg.triggerbot_scoped_only);
			}

			ImGui::Text("Maximum FOV");
			ImGui::SliderFloat("##FOVmagnettrigger", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].magnet_trigger_fov, 0, 180.f, "%.1f");
			ImGui::Text("Sensitivity");
			ImGui::SliderFloat("##Smoothmagnettrigger", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].magnet_trigger_smooth, 0, 100.f, "%.1f");
			ImGui::Text("Target Switch Delay");
			ImGui::SliderInt("##Target Switch Delaytrigger", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_target_switch_delay, 0, 2000, "%.0f");

			ImGui::NewLine();

			ImGui::EndChild();

			imgui_custom::horizontal_margin("legitspace1");

			ImGui::BeginChild("legit 3", ImVec2(0, ImGui::GetContentRegionAvail().y - 28), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("legitgb3");
			imgui_custom::custom_label_header("Other");
			ImGui::Checkbox("Standalone RCS", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].standalone_rcs);
			ImGui::Text("RCS Factor");
			ImGui::SliderFloat("##RCS Factor", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].standalone_rcs_power, 0, 100, "%.0f%%");
			ImGui::Checkbox("Backtracking", (bool*)&g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].accuracy_boost);
			ImGui::Text("Maximum Ticks");
			ImGui::SliderFloat("##Maximum Ticks", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].legit_maximum_ticks, 0, 30, "%.f%");
			ImGui::EndChild();

			ImGui::PopStyleColor();

			// padding side
			ImGui::NextColumn();
			ImGui::SetColumnWidth(4, 20);

			ImGui::EndColumns();
		}


		if (tab_selection == vis) {
			// begin tab

	// margin top column 1
			imgui_custom::horizontal_margin("vis1");

			ImGui::SetColumnWidth(2, 325);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("misc 1", ImVec2(0, 320), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("visgb1");
			imgui_custom::custom_label_header("Players");

			ImGui::Text("Activation type");
			if (g_weebwarecfg.enable_visuals == 2)
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.enable_visuals_key, key_counter);

			const char* activation_type[] = { "Off", "Enabled", "On Key" };
			ImGui::Combo("##activationtype", &g_weebwarecfg.enable_visuals, activation_type, ARRAYSIZE(activation_type));
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

			imgui_custom::horizontal_margin("visspace1");

			ImGui::BeginChild("misc 2", ImVec2(0, ImGui::GetContentRegionAvail().y - 28), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("visgb2");
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

			ImGui::SetColumnWidth(3, 325);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("misc 3", ImVec2(0, 395), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("visgb3");
			imgui_custom::custom_label_header("Other Visuals");
			ImGui::Checkbox("Inaccuracy Circle", &g_weebwarecfg.visuals_inacc_circle);
			if (g_weebwarecfg.visuals_inacc_circle) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_innacc_circle_col, "Inacc Color");
			}

			ImGui::Checkbox("Draw FOV", &g_weebwarecfg.visuals_fov_circle);
			if (g_weebwarecfg.visuals_fov_circle) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_fov_circle_col, "Circle Color");
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
				ImGui::Combo("##hitmarkersound", &g_weebwarecfg.hitmarker_sound, hit_sound, ARRAYSIZE(hit_sound));
			}
			ImGui::Checkbox("Bullet Tracers##bullettracers", &g_weebwarecfg.enable_bullet_tracers);
			if (g_weebwarecfg.enable_bullet_tracers) {
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_bullet_tracer_col, "btcol##1");
				ImGui::Text("Expire Time");
				ImGui::SliderFloat("##tracerexpire", &g_weebwarecfg.bullet_tracer_expire, 0, 20, "%.2f");
				ImGui::Text("Width");
				ImGui::SliderFloat("##Widthtracer", &g_weebwarecfg.bullet_tracer_width, 0, 10, "%.2f");
				ImGui::Text("Amplitude");
				ImGui::SliderFloat("##Amplitudetracer", &g_weebwarecfg.bullet_tracer_amplitude, 0, 10, "%.2f");
				ImGui::Text("Speed");
				ImGui::SliderFloat("##Speedtracer", &g_weebwarecfg.bullet_tracer_speed, 0, 2, "%.2f");
			}
			ImGui::Text("Backtracking Style");
			const char* backtrackstyle[] = { "Time", "Single", "All", "Target" };
			ImGui::Combo("##backtrackingtype", &g_weebwarecfg.visuals_backtrack_style, backtrackstyle, ARRAYSIZE(backtrackstyle));
			ImGui::Checkbox("Backtrack Skeleton", &g_weebwarecfg.visuals_backtrack_dots);
			if (g_weebwarecfg.visuals_backtrack_dots)
				imgui_custom::custom_color_inline(g_weebwarecfg.visuals_backtrack_col, "Backtrack Color");
			ImGui::EndChild();

			imgui_custom::horizontal_margin("visspace2");

			ImGui::BeginChild("misc 4", ImVec2(0, ImGui::GetContentRegionAvail().y - 28), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("miscgb4");
			imgui_custom::custom_label_header("Player Chams");
			const char* cham_type[] = { "Disabled", "Plain", "Glow", "Flat", "Glass", "Crystal", "Gold", "Crystal Blue" };
			ImGui::Text("Material");
			ImGui::Combo("##chammaterials", &g_weebwarecfg.visuals_chams, cham_type, ARRAYSIZE(cham_type));
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
			ImGui::SetColumnWidth(4, 20);

			ImGui::EndColumns();


			// end tab
		}

		if (tab_selection == misc) {
			// begin tab

			// margin top column 1
			imgui_custom::horizontal_margin("misc1");

			ImGui::SetColumnWidth(2, 325);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("misc 1", ImVec2(0, 310), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("miscgb1");
			imgui_custom::custom_label_header("Misc");

			ImGui::Checkbox("Chatspam", &g_weebwarecfg.misc_chat_spammer);
			ImGui::Checkbox("Preserve Killfeed", &g_weebwarecfg.preserve_killfeed);
			ImGui::Checkbox("Rank Reveal", &g_weebwarecfg.rank_reveal);
			ImGui::Checkbox("Disable Post Processing", &g_weebwarecfg.disable_post_processing);
			ImGui::Checkbox("Anti AFK", &g_weebwarecfg.anti_afk);
			ImGui::Checkbox("Auto Accept", &g_weebwarecfg.misc_autoAccept);
			ImGui::Checkbox("Viewmodel Changer", &g_weebwarecfg.viewmodel_changer);
			ImGui::Checkbox("Rainbow Name", &g_weebwarecfg.rainbow_name);

			ImGui::Checkbox("Clantag Changer", &g_weebwarecfg.misc_clantag_changer);
			if (g_weebwarecfg.misc_clantag_changer) {
				ImGui::InputTextWithHint("Custom Clantag", "empty for default", g_weebwarecfg.custom_clantag_static, ARRAYSIZE(g_weebwarecfg.custom_clantag_static));
			}

			ImGui::Checkbox("Killsay", &g_weebwarecfg.killsay);
			if (g_weebwarecfg.killsay) {
				ImGui::InputTextWithHint("Custom Killsay", "empty for default", g_weebwarecfg.killsay_msg_custom, ARRAYSIZE(g_weebwarecfg.killsay_msg_custom));
			}

			if (g_weebwarecfg.viewmodel_changer) {
				ImGui::SliderInt("Viewmodel Offset", &g_weebwarecfg.viewmodel_offset, -100, 135);
			}

			ImGui::Checkbox("Block Bot", &g_weebwarecfg.block_bot);
			if (g_weebwarecfg.block_bot)
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.block_bot_key, key_counter);
			ImGui::Checkbox("Auto Pistol", &g_weebwarecfg.auto_pistol);
			if (g_weebwarecfg.auto_pistol)
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.auto_pistol_key, key_counter);
			ImGui::Checkbox("Auto Defuse", &g_weebwarecfg.auto_defuse);
			if (g_weebwarecfg.auto_defuse)
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.auto_defuse_key, key_counter);

			ImGui::EndChild();

			imgui_custom::horizontal_margin("miscspace1");

			ImGui::BeginChild("misc 2", ImVec2(0, ImGui::GetContentRegionAvail().y - 28), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("miscgb2");
			imgui_custom::custom_label_header("Movement");
			ImGui::Checkbox("Infinite Duck", &g_weebwarecfg.no_duck_cooldown);
			ImGui::Checkbox("Slidewalk", &g_weebwarecfg.misc_slidewalk);
			ImGui::Checkbox("Bunnyhop", &g_weebwarecfg.auto_jump);
			if (g_weebwarecfg.auto_jump) {
				ImGui::SliderInt("Hitchance##bhop", &g_weebwarecfg.auto_jump_hitchance, 0, 100, "%.0f%%");
			}
			ImGui::Checkbox("Edge Jump", &g_weebwarecfg.edge_jump);
			if (g_weebwarecfg.edge_jump)
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.edge_jump_key, key_counter);
			if (g_weebwarecfg.edge_jump) {
				ImGui::Checkbox("Duck In Air", &g_weebwarecfg.duck_in_air);
			}
			ImGui::Text("Auto Strafe");
			const char* strafe_type[] = { "Off", "Legit", "Fast" };
			ImGui::Combo("##Autostrafe", &g_weebwarecfg.auto_strafe, strafe_type, ARRAYSIZE(strafe_type));

			ImGui::EndChild();

			ImGui::PopStyleColor();

			ImGui::NextColumn();

			// margin top column 2
			imgui_custom::horizontal_margin("misc2");

			ImGui::SetColumnWidth(3, 325);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("misc 3", ImVec2(0, 220), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("miscgb3");
			imgui_custom::custom_label_header("Smartbot");
			ImGui::Checkbox("Enabled", &g_weebwarecfg.misc_ai);
			ImGui::Checkbox("Random", &g_weebwarecfg.misc_ai_random);
			ImGui::Checkbox("Engage nearest enemy", &g_weebwarecfg.misc_ai_nearest);
			ImGui::Checkbox("Defuse bombs", &g_weebwarecfg.misc_ai_defuse);
			//	ImGui::Checkbox("Defend closest sites", &g_weebwarecfg.misc_ai_defend, false);
			ImGui::Text("Rotation Speed");
			ImGui::SliderFloat("Roatation Speed", &g_weebwarecfg.misc_ai_rotationspeed, 0, 100, "%.0f%%");
			ImGui::Text("Aim Speed");
			ImGui::SliderFloat("Aim Speed##ai", &g_weebwarecfg.misc_ai_aimspeed, 0, 100, "%.0f%%");

			ImGui::EndChild();

			imgui_custom::horizontal_margin("miscspace2");

			ImGui::BeginChild("misc 4", ImVec2(0, ImGui::GetContentRegionAvail().y - 28), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("miscgb4");
			imgui_custom::custom_label_header("Fake Lag");
			ImGui::Text("Activation type");
			if (g_weebwarecfg.fake_lag == 1)
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.fake_lag_key, key_counter);

			const char* fakelag_type[] = { "Off", "On Key", "Always On" };
			ImGui::Combo("##fakelagtype", &g_weebwarecfg.fake_lag, fakelag_type, ARRAYSIZE(fakelag_type));
			if (g_weebwarecfg.fake_lag > 0) {
				ImGui::SliderInt("Fakelag Factor", &g_weebwarecfg.fake_lag_factor, 0, 6);
			}

			ImGui::EndChild();

			ImGui::PopStyleColor();

			// padding side
			ImGui::NextColumn();
			ImGui::SetColumnWidth(4, 20);

			ImGui::EndColumns();


			// end tab
		}

		if (tab_selection == skins) {
			// begin tab

	// margin top column 1
			imgui_custom::horizontal_margin("skins1");

			ImGui::SetColumnWidth(2, 325);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("skins 1", ImVec2(0, 80), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("skinsgb1");
			imgui_custom::custom_label_header("Knife Options");
			ImGui::Checkbox("Enabled##knifechanger", &g_weebwarecfg.knifechanger_enabled);
			const char* knives[] = { "Terrorist Knife", "Counter-Terrorist Knife", "Flip", "Gut", "Karambit", "M9 Bayonet", "Bayonet", "Huntsman", "Falchion", "Stiletto", "Ursus", "Navaja", "Talon", "Butterfly", "Shadow Daggers", "Bowie", "Classic", "Paracord", "Survival", "Skeleton", "Nomad" };
			ImGui::Combo("##knifecombo", &g_weebwarecfg.selected_knife_index[1], knives, ARRAYSIZE(knives));
			g_weebwarecfg.selected_knife_index[0] = g_weebware.g_knife_list[g_weebwarecfg.selected_knife_index[1]].weapon_index;

			ImGui::EndChild();

			imgui_custom::horizontal_margin("skinsspace1");

			ImGui::BeginChild("skins 2", ImVec2(0, 300), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("skinsgb2");
			imgui_custom::custom_label_header("Glove Options");

			ImGui::Checkbox("Enabled##glovechanger", &g_weebwarecfg.glovechanger_enabled);
			const char* glove_models[] = { "Default", "Sport", "Hand Wraps", "Specialist", "Driver", "Moto", "Hydra", "Bloodhound" };
			ImGui::Combo("##glovenames", &g_weebwarecfg.glove_model, glove_models, ARRAYSIZE(glove_models));
			ImGui::Text("Wear");
			ImGui::SliderFloat("##Weargloves", &g_weebwarecfg.glove_wearz, 0, 100, "%.0f%%");
			ImGui::Separator();

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

			imgui_custom::horizontal_margin("skinsspace2");

			ImGui::BeginChild("skins 3", ImVec2(0, ImGui::GetContentRegionAvail().y - 28), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("skinsgb2");
			imgui_custom::custom_label_header("Model Options");


			if (g_weebware.models_installed) {
				ImGui::Text("Player Models");
				const char* models[] = { "Off", "Reina Kousaka", "Yuno Gasai", "Kimono Luka", "Inori" };
				ImGui::Combo("##model_type", &g_weebwarecfg.anime_model, models, ARRAYSIZE(models));
				// https://gamebanana.com/skins/148058

				if (ImGui::Button("Apply", ImVec2(ImGui::GetContentRegionAvailWidth() / 1.5, 25))) {
					g_weebwarecfg.skinchanger_apply_nxt = 1;
				}
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

			ImGui::SetColumnWidth(3, 325);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ConvertFromRGBA(ImVec4(20, 20, 20, 255)));
			ImGui::BeginChild("skins 4", ImVec2(0, 220), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("skinsgb3");
			imgui_custom::custom_label_header("Smartbot");


			ImGui::EndChild();

			imgui_custom::horizontal_margin("skinsspace3");

			ImGui::BeginChild("skins 5", ImVec2(0, ImGui::GetContentRegionAvail().y - 28), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
			imgui_custom::fix_gay_padding_shit("skinsgb4");
			imgui_custom::custom_label_header("Fake Lag");


			ImGui::EndChild();

			ImGui::PopStyleColor();

			// padding side
			ImGui::NextColumn();
			ImGui::SetColumnWidth(4, 20);

			ImGui::EndColumns();


			// end tab
		}


		ImGui::End();
		ImGui::PopFont();
		ImGui::Render();

	}
}


std::vector<c_skinchanger::gun_type> filtered_guns()
{
	// Get text
	std::string filter = g_config_list.skinchanger_gunsearch;

	static std::vector<c_skinchanger::gun_type> filtered_guns = g_weebware.g_gun_list;

	static std::string change = filter;

	if (change != filter) {

		// Empty buffer
		filtered_guns.clear();
		// update, execute code for cpu saving

		for (auto part : g_weebware.g_gun_list) {

			std::string lower_search = filter;
			std::transform(lower_search.begin(), lower_search.end(), lower_search.begin(), ::tolower);
			std::string lower_skin = part.name;
			std::transform(lower_skin.begin(), lower_skin.end(), lower_skin.begin(), ::tolower);

			if (strstr(lower_skin.c_str(), lower_search.c_str())) {
				filtered_guns.push_back(part);
			}
		}

		change = filter;
	}

	return filtered_guns;
}

int convert_index_id(int index);

std::vector<c_skinchanger::skin_type> filtered_skins()
{
	// Get text
	std::string filter = g_config_list.skinchanger_skinsearch;

	static std::vector<c_skinchanger::skin_type> filtered_skins = g_weebware.g_skin_list;

	static std::string change = filter;

	if (change != filter) {

		// Empty buffer
		filtered_skins.clear();
		// update, execute code for cpu saving

		for (auto part : g_weebware.g_skin_list) {

			std::string lower_search = filter;
			std::transform(lower_search.begin(), lower_search.end(), lower_search.begin(), ::tolower);
			std::string lower_skin = part.name;
			std::transform(lower_skin.begin(), lower_skin.end(), lower_skin.begin(), ::tolower);

			if (strstr(lower_skin.c_str(), lower_search.c_str())) {
				filtered_skins.push_back(part);
			}
		}

		change = filter;
	}

	return filtered_skins;
}

int convert_index_id(int index);


/*

	static bool VisualsRelease = false;

	static int stage_header = -15.f;
	static int stage_body = 0;

	int screen_width = 0, screen_height = 0;
	g_weebware.g_engine->get_screen_dimensions(screen_width, screen_height);

	if (g_weebware.menu_opened)
	{
		int key_counter = 0;

		int pX, pY;
		g_weebware.g_input_system->GetCursorPosition(&pX, &pY);
		ImGuiIO& io = ImGui::GetIO();
		auto& style = ImGui::GetStyle();
		io.MousePos.x = (float)(pX);
		io.MousePos.y = (float)(pY);


		static int tab_selection = tabs::legit;

		// Apply default font
		ImGui::PushFont(pFont[1]);
		style.Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(17, 17, 17, 255.f));
		style.Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(17, 17, 17, 255.f));
		style.Colors[ImGuiCol_ButtonActive] = imgui_custom::ConvertFromRGBA(ImVec4(17, 17, 17, 255.f));

		static const constexpr int padding = 40;

#pragma region tabs
		ImGui::Begin("Header", &g_weebware.menu_opened, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
		ImGui::SetWindowSize(ImVec2(screen_width - (padding * 2), padding));
		stage_header++;
		if (stage_header <= 40.f)
			ImGui::SetWindowPos(ImVec2(padding, stage_header));
		else {
			ImGui::SetWindowPos(ImVec2(padding, padding));
		}


		ImGui::Columns(6, "Tabs", false);

		int column_width = screen_width - (padding * 5);

		ImGui::BeginChild("COL2", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);

		style.Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(17, 17, 17, 255.f));
		style.Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(17, 17, 17, 255.f));
		style.Colors[ImGuiCol_ButtonActive] = imgui_custom::ConvertFromRGBA(ImVec4(17, 17, 17, 255.f));

		if (tab_selection == tabs::legit) {
			style.Colors[ImGuiCol_Text] = imgui_custom::ConvertFromRGBA(ImVec4(138, 43, 226, 255.f));
		}
		if (ImGui::Button("Legit", ImVec2(column_width / 6, 20))) {
			tab_selection = tabs::legit;
		}
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);

		ImGui::EndChild();
		ImGui::NextColumn();

		ImGui::BeginChild("COL3", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);

		if (tab_selection == tabs::vis)
			style.Colors[ImGuiCol_Text] = imgui_custom::ConvertFromRGBA(ImVec4(138, 43, 226, 255.f));

		if (ImGui::Button("Visual", ImVec2(column_width / 6, 20))) {
			tab_selection = tabs::vis;
		}
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);

		ImGui::EndChild();
		ImGui::NextColumn();

		ImGui::BeginChild("COL4", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);
		if (tab_selection == tabs::misc)
			style.Colors[ImGuiCol_Text] = imgui_custom::ConvertFromRGBA(ImVec4(138, 43, 226, 255.f));

		if (ImGui::Button("Misc", ImVec2(column_width / 6, 20))) {
			tab_selection = tabs::misc;
		}
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);

		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("COL5", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);
		if (tab_selection == tabs::skins)
			style.Colors[ImGuiCol_Text] = imgui_custom::ConvertFromRGBA(ImVec4(138, 43, 226, 255.f));

		if (ImGui::Button("Skins", ImVec2(column_width / 6, 20))) {
			tab_selection = tabs::skins;
		}
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);
		ImGui::EndChild();

		ImGui::NextColumn();


		ImGui::BeginChild("COL6", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);
		if (tab_selection == tabs::settings)
			style.Colors[ImGuiCol_Text] = imgui_custom::ConvertFromRGBA(ImVec4(138, 43, 226, 255.f));
		if (ImGui::Button("Configs", ImVec2(column_width / 6, 20))) {
			tab_selection = tabs::settings;
		}
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);

		ImGui::EndChild();

		style.Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(0, 0, 0, 255.f));
		style.Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(27, 27, 27, 255.f));
		style.Colors[ImGuiCol_ButtonActive] = imgui_custom::ConvertFromRGBA(ImVec4(0, 0, 0, 255.f));

		ImGui::NextColumn();

		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);
		style.Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(30, 10, 10, 255.f));
		style.Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(30, 10, 10, 255.f));
		style.Colors[ImGuiCol_ButtonActive] = imgui_custom::ConvertFromRGBA(ImVec4(30, 10, 10, 255.f));
	//	style.Colors[ImGuiCol_ChildWindowBg] = imgui_custom::ConvertFromRGBA(ImVec4(30, 10, 10, 255.f));
		ImGui::BeginChild("COL7", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);
		if (ImGui::Button("Unload", ImVec2(column_width / 6, 20))) {
			try {
				g_hooking.unhook_all_functions();
			}
			catch (...) {}
		}
		ImGui::EndChild();

		style.Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
		style.Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(27, 27, 27, 255.f));
		style.Colors[ImGuiCol_ButtonActive] = imgui_custom::ConvertFromRGBA(ImVec4(0, 0, 0, 255.f));
	//	style.Colors[ImGuiCol_ChildWindowBg] = imgui_custom::ConvertFromRGBA(ImVec4(17.0f, 17.0f, 17.0f, 255.f));

		ImGui::End();
#pragma endregion
		stage_body -= 15;

		int new_width = screen_width;
		float new_height = screen_height * .69;
		float diff = 0;

		if (screen_width <= 1280) {
			new_width = 1400;
			diff = new_width - screen_width;
		}

		ImGui::Begin("Body", &g_weebware.menu_opened, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
		ImGui::SetWindowSize(ImVec2(new_width / 2, screen_height * .69));
		if (stage_body <= (screen_height / 2) - ((screen_height * .72) / 2)) {
			ImGui::SetWindowPos(ImVec2(((screen_width / 2) - ((screen_width / 2) / 2) - (diff / 4)), ((screen_height / 2) / 2) - (screen_height / 10)), ImGuiCond_Always);
		}
		else
			ImGui::SetWindowPos(ImVec2(((screen_width / 2) - ((screen_width / 2) / 2) - (diff / 4)), stage_body), ImGuiCond_Always);

#pragma region Legitbot
		if (tab_selection == tabs::legit) {

			ImGui::BeginChild("Type Sizing", ImVec2(ImGui::GetContentRegionAvailWidth() - 2, 30), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			const char* weapon_groups[] = { "Pistols", "Rifles", "SMG","Shotguns", "Heavy", "Auto-Snipers", "AWP", "SSG08" };

			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 0, "Pistols", new_width);
			ImGui::SameLine();
		//	ImGui::VerticalSeparator();
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 1, "Rifles", new_width);
			ImGui::SameLine();
		//	ImGui::VerticalSeparator();
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 2, "SMG", new_width);
			ImGui::SameLine();
		//	ImGui::VerticalSeparator();
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 3, "Shotguns", new_width);
			ImGui::SameLine();
		//	ImGui::VerticalSeparator();
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 4, "Heavy", new_width);
			ImGui::SameLine();
	//		ImGui::VerticalSeparator();
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 5, "Auto", new_width);
			ImGui::SameLine();
		//	ImGui::VerticalSeparator();
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 6, "AWP", new_width);
			ImGui::SameLine();
		//	ImGui::VerticalSeparator();
			ImGui::SameLine();
			imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 7, "SSG08", new_width);
			ImGui::SameLine();

			// restore colors
			style.Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(0, 0, 0, 255.f));
			style.Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(27, 27, 27, 255.f));
			style.Colors[ImGuiCol_ButtonActive] = imgui_custom::ConvertFromRGBA(ImVec4(0, 0, 0, 255.f));
			style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			ImGui::EndChild();

			ImGui::Columns(2, "LegitCOL", false);

			ImGui::BeginChild("AimbotSettings", ImVec2(0, 0), true);
			{
				ImGui::Text("Aimbot");
				ImGui::Separator();

				ImGui::Text("Activation type");
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].legitbot_activation_key, key_counter);

				const char* activation_type[] = { "Off", "On Fire", "On Key", "Magnetic" };

				ImGui::Combo("", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].enable_legitbot, activation_type, ARRAYSIZE(activation_type));
				ImGui::Checkbox("Silent aim", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].silent_aim);
				ImGui::Checkbox("Target teammates", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].target_teammates);
				ImGui::Checkbox("Distance FOV", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].use_dynamicfov);

				ImGui::Text("Maximum FOV");
				ImGui::SliderFloat("Maximum FOV", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].maximum_fov, 0, 30, "%.1f");

				ImGui::Text("Sensitivity");
				ImGui::SliderFloat("Sensitivity", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].sensitivity, 0, 100, "%.0f%%");

				ImGui::Text("First Shot Delay");
				ImGui::SliderFloat("First Shot Delay", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].reaction_time, 0, 200, "%.0fms");

				ImGui::Text("Target Switch Delay");
				ImGui::SliderInt("Target Switch Delay", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].aimbot_target_switch_delay, 0, 2000, "%.0f");

				ImGui::Text("Recoil compensation (P/Y)");
				ImGui::SliderFloat("Recoil compensation P", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].pitch_rcs, 0, 100, "%.0f%%");
				ImGui::SliderFloat("Recoil compensation Y", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].yaw_rcs, 0, 100, "%.0f%%");

				ImGui::Checkbox("Quickstop##legit", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].quick_stop);

				ImGui::Checkbox("Aim through smoke", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].aim_through_smoke);

				ImGui::Checkbox("Aim while blind", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].aim_while_blind);

				ImGui::Checkbox("Head", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].hitbox_head);
				ImGui::Checkbox("Chest", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].hitbox_chest);
				ImGui::Checkbox("Stomach", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].hitbox_stomach);

			}
			ImGui::EndChild();

			ImGui::NextColumn();

			ImGui::BeginChild("Triggerbot", ImVec2(0, ImGui::GetContentRegionAvail().y / 3 * 2), true);
			{
				ImGui::Text("Triggerbot");
				ImGui::Separator();

				ImGui::Text("Activation type");
				imgui_custom::custom_inline_keyinput(g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_key, key_counter);
				const char* activation_type[] = { "Off", "On Key", "Active" };
				ImGui::Combo("", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_active, activation_type, ARRAYSIZE(activation_type));
				ImGui::Text("Hitchance");
				ImGui::SliderFloat("Hitchance", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_hitchance, 0, 100, "%.0f%%");
				ImGui::Text("Reaction time");
				ImGui::SliderFloat("Delay", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_reaction, 0, 200, "%.0fms");

				ImGui::Checkbox("Head##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_head);
				ImGui::SameLine();
				ImGui::Checkbox("Chest##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_chest);
				ImGui::SameLine();
				ImGui::Checkbox("Stomach##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_stomach);
				ImGui::Checkbox("Arms##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_arms);
				ImGui::SameLine();
				ImGui::Checkbox("Legs##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_legs);
				ImGui::Separator();
				ImGui::Text("Magnet Triggerbot");
				ImGui::Separator();
				ImGui::Checkbox("Enable##magnet", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].magnet_triggerbot_enabled);
				ImGui::Text("Maximum FOV");
				ImGui::SliderFloat("FOV##magnettrigger", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].magnet_trigger_fov, 0, 180.f, "%.1f");
				ImGui::Text("Sensitivity");
				ImGui::SliderFloat("Smooth##magnettrigger", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].magnet_trigger_smooth, 0, 100.f, "%.1f");
				ImGui::Text("Target Switch Delay");
				ImGui::SliderInt("Target Switch Delay##trigger", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_target_switch_delay, 0, 2000, "%.0f");
				ImGui::Checkbox("Quickstop##magnet", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].quick_stop_magnet);

				if (g_weebwarecfg.legit_cfg_index == 5 || g_weebwarecfg.legit_cfg_index == 6 || g_weebwarecfg.legit_cfg_index == 7) {
					ImGui::Checkbox("Scoped Only", &g_weebwarecfg.triggerbot_scoped_only);
				}

				ImGui::Checkbox("Aim Through Smoke##magnet", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_aim_through_smoke);

			}
			ImGui::EndChild();
			ImGui::BeginChild("Other", ImVec2(0, 0), true);
			{
				ImGui::Text("Other");
				ImGui::Separator();
				ImGui::Checkbox("Standalone RCS", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].standalone_rcs);
				ImGui::Text("RCS Factor");
				ImGui::SliderFloat("RCS Factor", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].standalone_rcs_power, 0, 100, "%.0f%%");
				ImGui::Checkbox("Backtracking", (bool*)&g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].accuracy_boost);
				ImGui::Text("Maximum Ticks");
				ImGui::SliderFloat("Maximum Ticks", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].legit_maximum_ticks, 0, 30, "%.f%");
			}
			ImGui::EndChild();

		}
#pragma endregion

#pragma region Ragebot

#pragma endregion

#pragma region Visuals
		if (tab_selection == tabs::vis) {
			ImGui::Columns(2, "Visuals", false);
			{
				ImGui::BeginChild("Visuals 1", ImVec2(0, 0), true);
				{
					ImGui::Text("Player ESP");
					ImGui::Separator();
					ImGui::Text("Activation type");
					imgui_custom::custom_inline_keyinput(g_weebwarecfg.enable_visuals_key, key_counter);
					const char* activation_type[] = { "Off", "Enabled", "On Key" };
					ImGui::Combo("##activationtype", &g_weebwarecfg.enable_visuals, activation_type, ARRAYSIZE(activation_type));
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

					ImGui::Separator();
					ImGui::Text("Glow");
					ImGui::Separator();

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

					ImGui::Separator();
					ImGui::Text("Player Chams");
					ImGui::Separator();

					const char* cham_type[] = { "Disabled", "Plain", "Glow", "Flat", "Glass", "Crystal", "Gold", "Crystal Blue" };
					ImGui::Text("Material");
					ImGui::Combo("##chammaterials", &g_weebwarecfg.visuals_chams, cham_type, ARRAYSIZE(cham_type));
					if (g_weebwarecfg.visuals_chams > 0)
						imgui_custom::custom_color_inline(g_weebwarecfg.visuals_chams_col, g_weebwarecfg.visuals_chams == 2 ? g_weebwarecfg.visuals_chams_glow_col : g_weebwarecfg.visuals_chams_team_col, 1, "Enemy Color##chams1", g_weebwarecfg.visuals_chams == 2 ? "Glow Cham Color##chams2" : "Team Color##chams2");
					ImGui::Checkbox("Render Team", &g_weebwarecfg.visuals_chams_render_team);

					ImGui::Checkbox("XQZ (Through Materials)", &g_weebwarecfg.visuals_chams_xqz);
					if (g_weebwarecfg.visuals_chams_xqz)
						imgui_custom::custom_color_inline(g_weebwarecfg.visuals_chams_col_xqz, g_weebwarecfg.visuals_chams_team_col_xqz, 1, "Enemy XQZ Color##chams1", "Team XQZ Color##chams2");


					//ImGui::Separator();
					//ImGui::Text("Hand Chams");
					//ImGui::Separator();
					//ImGui::Checkbox("Enabled##handcham", &g_weebwarecfg.hand_chams, false);
					//if (g_weebwarecfg.hand_chams)
					//	imgui_custom::custom_color_inline(g_weebwarecfg.hand_cham_col, "Hand Cham Color");

					//ImGui::Checkbox("XQZ##handcham", &g_weebwarecfg.hand_chams_xqz, false);

					//ImGui::Separator();
					//ImGui::Text("Weapon Chams");
					//ImGui::Separator();
					//ImGui::Checkbox("Enabled##weaponcham", &g_weebwarecfg.weapon_chams, false);
					//if (g_weebwarecfg.weapon_chams)
					//	imgui_custom::custom_color_inline(g_weebwarecfg.weapon_cham_col, "Hand Cham Color");

				}
				ImGui::EndChild();
				ImGui::NextColumn();

				ImGui::BeginChild("Visuals 2", ImVec2(0, 0), true);
				{
					ImGui::Text("Other Visuals");
					ImGui::Separator();
					ImGui::Checkbox("Inaccuracy Circle", &g_weebwarecfg.visuals_inacc_circle);
					if (g_weebwarecfg.visuals_inacc_circle) {
						imgui_custom::custom_color_inline(g_weebwarecfg.visuals_innacc_circle_col, "Inacc Color");
					}

					ImGui::Checkbox("Draw FOV", &g_weebwarecfg.visuals_fov_circle);
					if (g_weebwarecfg.visuals_fov_circle) {
						imgui_custom::custom_color_inline(g_weebwarecfg.visuals_fov_circle_col, "Circle Color");
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
						ImGui::Combo("##hitmarkersound", &g_weebwarecfg.hitmarker_sound, hit_sound, ARRAYSIZE(hit_sound));
					}


					ImGui::Separator();
					ImGui::Text("Bullet Tracers");
					ImGui::Separator();

					ImGui::Checkbox("Enabled##bullettracers", &g_weebwarecfg.enable_bullet_tracers);
					if (g_weebwarecfg.enable_bullet_tracers) {
						imgui_custom::custom_color_inline(g_weebwarecfg.visuals_bullet_tracer_col, "btcol##1");
						ImGui::Text("Expire Time");
						ImGui::SliderFloat("##tracerexpire", &g_weebwarecfg.bullet_tracer_expire, 0, 20, "%.2f");
						ImGui::Text("Width");
						ImGui::SliderFloat("Width##tracer", &g_weebwarecfg.bullet_tracer_width, 0, 10, "%.2f");
						ImGui::Text("Amplitude");
						ImGui::SliderFloat("Amplitude##tracer", &g_weebwarecfg.bullet_tracer_amplitude, 0, 10, "%.2f");
						ImGui::Text("Speed");
						ImGui::SliderFloat("Speed##tracer", &g_weebwarecfg.bullet_tracer_speed, 0, 2, "%.2f");
					}

					ImGui::Separator();
					ImGui::Text("Backtracking");
					ImGui::Separator();
					ImGui::Text("Style");
					const char* backtrackstyle[] = { "Time", "Single", "All", "Target" };
					ImGui::Combo("##backtrackingtype", &g_weebwarecfg.visuals_backtrack_style, backtrackstyle, ARRAYSIZE(backtrackstyle));
					ImGui::Checkbox("Backtrack Skeleton", &g_weebwarecfg.visuals_backtrack_dots);
					if (g_weebwarecfg.visuals_backtrack_dots)
						imgui_custom::custom_color_inline(g_weebwarecfg.visuals_backtrack_col, "Backtrack Color");

				}

				ImGui::EndChild();

			}
		}
#pragma endregion

#pragma region Misc
		if (tab_selection == tabs::misc) {
			ImGui::Columns(2, "Miscs", false);
			{
				ImGui::BeginChild("Misc 1", ImVec2(0, 0), true);
				{
					ImGui::Text("Misc");
					ImGui::Separator();
					ImGui::Checkbox("Clantag Changer", &g_weebwarecfg.misc_clantag_changer);
					if (g_weebwarecfg.misc_clantag_changer) {
						ImGui::InputText("empty for default##clantag", g_weebwarecfg.custom_clantag_static, ARRAYSIZE(g_weebwarecfg.custom_clantag_static));
					}
					ImGui::Checkbox("Chatspam", &g_weebwarecfg.misc_chat_spammer);
					ImGui::Checkbox("Preserve Killfeed", &g_weebwarecfg.preserve_killfeed);
					ImGui::Checkbox("Auto Pistol", &g_weebwarecfg.auto_pistol);
					imgui_custom::custom_inline_keyinput(g_weebwarecfg.auto_pistol_key, key_counter);
					ImGui::Checkbox("Rank Reveal", &g_weebwarecfg.rank_reveal);
					ImGui::Checkbox("Killsay", &g_weebwarecfg.killsay);
					if (g_weebwarecfg.killsay) {
						ImGui::InputText("empty for default##killsay", g_weebwarecfg.killsay_msg_custom, ARRAYSIZE(g_weebwarecfg.killsay_msg_custom));
					}
					ImGui::Checkbox("Disable Post Processing", &g_weebwarecfg.disable_post_processing);
					ImGui::Checkbox("Anti AFK", &g_weebwarecfg.anti_afk);
					//	ImGui::Checkbox("Third Person", &g_weebwarecfg.thirdperson, false);
					//	imgui_custom::custom_inline_keyinput(g_weebwarecfg.thirdperson_key, key_counter);
					ImGui::Checkbox("Auto accept", &g_weebwarecfg.misc_autoAccept);
					ImGui::Checkbox("Viewmodel Changer", &g_weebwarecfg.viewmodel_changer);
					if (g_weebwarecfg.viewmodel_changer) {
						ImGui::SliderInt("Viewmodel Offset", &g_weebwarecfg.viewmodel_offset, -100, 135);
					}
					ImGui::Checkbox("Rainbow Name", &g_weebwarecfg.rainbow_name);
					ImGui::Checkbox("Block Bot", &g_weebwarecfg.block_bot);
					imgui_custom::custom_inline_keyinput(g_weebwarecfg.block_bot_key, key_counter);
					ImGui::Checkbox("Auto Defuse", &g_weebwarecfg.auto_defuse);
					imgui_custom::custom_inline_keyinput(g_weebwarecfg.auto_defuse_key, key_counter);

					//	ImGui::Checkbox("No Flash", &g_weebwarecfg.remove_flash, false);

					ImGui::Separator();
					ImGui::Text("Movement");
					ImGui::Separator();

					ImGui::Checkbox("Bunnyhop", &g_weebwarecfg.auto_jump);
					if (g_weebwarecfg.auto_jump) {
						ImGui::SliderInt("Hitchance##bhop", &g_weebwarecfg.auto_jump_hitchance, 0, 100, "%.0f%%");
					}
					//		ImGui::Checkbox("Auto Jump Bug", &g_weebwarecfg.auto_jumpbug, false);
					//		imgui_custom::custom_inline_keyinput(g_weebwarecfg.auto_jumpbug_key, key_counter);
					ImGui::Checkbox("Infinite Duck", &g_weebwarecfg.no_duck_cooldown);
					ImGui::Checkbox("Slidewalk", &g_weebwarecfg.misc_slidewalk);
					ImGui::Checkbox("Edge Jump", &g_weebwarecfg.edge_jump);
					imgui_custom::custom_inline_keyinput(g_weebwarecfg.edge_jump_key, key_counter);
					if (g_weebwarecfg.edge_jump) {
						ImGui::Checkbox("Duck In Air", &g_weebwarecfg.duck_in_air);
					}
					ImGui::Text("Auto Strafe");
					const char* strafe_type[] = { "Off", "Legit", "Fast" };
					ImGui::Combo("##Autostrafe", &g_weebwarecfg.auto_strafe, strafe_type, ARRAYSIZE(strafe_type));

					//ImGui::Separator();
					//ImGui::Text("Legit HVH");
					//ImGui::Separator();
					//ImGui::Checkbox("Legit AA", &g_weebwarecfg.misc_legit_aa_enabled, false);
					//ImGui::Checkbox("On Sendpacket", &g_weebwarecfg.on_sendpacket, false);
					//ImGui::Text("Tickcount mod");
					//ImGui::SliderInt("Tickcount mod", &g_weebwarecfg.tick_count_mod, 1, 20);
					//ImGui::Text("Command num mod");
					//ImGui::SliderInt("Command num mod", &g_weebwarecfg.command_num_mod, 1, 20);
					//ImGui::SliderFloat("Yaw offset", &g_weebwarecfg.yaw_offset, -180, 180);
				}
				ImGui::EndChild();

				ImGui::NextColumn();

				ImGui::BeginChild("Misc 2", ImVec2(0, 0), true);
				{
					ImGui::Text("Smartbot");
					ImGui::Separator();
					ImGui::Checkbox("Enabled", &g_weebwarecfg.misc_ai);
					ImGui::Checkbox("Random", &g_weebwarecfg.misc_ai_random);
					ImGui::Checkbox("Engage nearest enemy", &g_weebwarecfg.misc_ai_nearest);
					ImGui::Checkbox("Defuse bombs", &g_weebwarecfg.misc_ai_defuse);
					//	ImGui::Checkbox("Defend closest sites", &g_weebwarecfg.misc_ai_defend, false);
					ImGui::Text("Rotation Speed");
					ImGui::SliderFloat("Roatation Speed", &g_weebwarecfg.misc_ai_rotationspeed, 0, 100, "%.0f%%");
					ImGui::Text("Aim Speed");
					ImGui::SliderFloat("Aim Speed##ai", &g_weebwarecfg.misc_ai_aimspeed, 0, 100, "%.0f%%");

					ImGui::Separator();
					ImGui::Text("Fake Lag");
					ImGui::Separator();
					ImGui::Text("Activation type");
					imgui_custom::custom_inline_keyinput(g_weebwarecfg.fake_lag_key, key_counter);

					const char* fakelag_type[] = { "Off", "On Key", "Always On" };
					ImGui::Combo("##fakelagtype", &g_weebwarecfg.fake_lag, fakelag_type, ARRAYSIZE(fakelag_type));
					if (g_weebwarecfg.fake_lag > 0) {
						ImGui::SliderInt("Fakelag Factor", &g_weebwarecfg.fake_lag_factor, 0, 6);
					}

					//ImGui::Separator();

					//	ImGui::Text("Legit Anti-Aim");
					//	ImGui::Separator();

					//	// ImGui::Checkbox("Anti Triggerbot", &g_weebwarecfg.anti_triggerbot, false);
					//ImGui::Text("Anti Triggerbot");
					//imgui_custom::custom_inline_keyinput(g_weebwarecfg.anti_triggerbot_key, key_counter);
					//const char* activation[] = { "Off", "On Key" };
					//ImGui::Combo("##antitrigtype", &g_weebwarecfg.anti_triggerbot, activation, ARRAYSIZE(activation));

					//	ImGui::Checkbox("Enable AA", &g_weebwarecfg.misc_legit_aa_enabled, false);
					//	ImGui::Checkbox("Jitter", &g_weebwarecfg.misc_legit_aa_jitter, false);
					//	ImGui::Checkbox("Resolver", &g_weebwarecfg.misc_legit_aa_resolver, false);
					//	ImGui::Checkbox("Edge", &g_weebwarecfg.misc_legit_aa_edge, false);
						//	ImGui::Checkbox("Slide walk", &g_weebwarecfg.misc_slidewalk, false);


					ImGui::Separator();
				}
				ImGui::EndChild();

			}

		}
#pragma endregion

#pragma region skinchanger
		if (tab_selection == tabs::skins) {

			ImGui::Columns(2, "SkinChanger", false);

			float skin_tab_height = ImGui::GetContentRegionAvail().y;
			ImGui::BeginChild("Knife", ImVec2(0, skin_tab_height / 5), true);
			{
				ImGui::Text("Knife Options");
				ImGui::Separator();
				ImGui::Checkbox("Enabled##knifechanger", &g_weebwarecfg.knifechanger_enabled);
				const char* knives[] = { "Terrorist Knife", "Counter-Terrorist Knife", "Flip", "Gut", "Karambit", "M9 Bayonet", "Bayonet", "Huntsman", "Falchion", "Stiletto", "Ursus", "Navaja", "Talon", "Butterfly", "Shadow Daggers", "Bowie", "Classic", "Paracord", "Survival", "Skeleton", "Nomad" };
				ImGui::Combo("##knifecombo", &g_weebwarecfg.selected_knife_index[1], knives, ARRAYSIZE(knives));
				g_weebwarecfg.selected_knife_index[0] = g_weebware.g_knife_list[g_weebwarecfg.selected_knife_index[1]].weapon_index;
			}
			ImGui::EndChild();

			ImGui::BeginChild("Gloves", ImVec2(0, ((skin_tab_height / 5) * 3) - 5), true);
			{
				ImGui::Text("Glove Options");
				ImGui::Separator();
				ImGui::Checkbox("Enabled##glovechanger", &g_weebwarecfg.glovechanger_enabled);
				const char* glove_models[] = { "Default", "Sport", "Hand Wraps", "Specialist", "Driver", "Moto", "Hydra", "Bloodhound" };
				ImGui::Combo("##glovenames", &g_weebwarecfg.glove_model, glove_models, ARRAYSIZE(glove_models));
				ImGui::Text("Wear");
				ImGui::SliderFloat("Wear##gloves", &g_weebwarecfg.glove_wearz, 0, 100, "%.0f%%");
				ImGui::Separator();

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
			}
			ImGui::EndChild();

			ImGui::BeginChild("Models", ImVec2(0, ImGui::GetContentRegionAvail().y), true);
			{
				ImGui::Text("Models");
				ImGui::Separator();

				if (g_weebware.models_installed) {
					ImGui::Text("Player Models");
					const char* models[] = { "Off", "Reina Kousaka", "Yuno Gasai", "Kimono Luka", "Inori" };
					ImGui::Combo("##model_type", &g_weebwarecfg.anime_model, models, ARRAYSIZE(models));
					// https://gamebanana.com/skins/148058

					if (ImGui::Button("Apply", ImVec2(ImGui::GetContentRegionAvailWidth() / 1.5, 25))) {

						g_weebwarecfg.skinchanger_apply_nxt = 1;
					}
				}
				else {
					ImGui::Text("Please properly install models");
					if (ImGui::Button("Download"))
						ShellExecute(0, 0, "https://auth.weebware.net/dependencies/models.exe", 0, 0, SW_SHOW);
					if (ImGui::Button("Refresh"))
						g_weebware.models_installed = g_weebware.check_models_installed();
				}
			}
			ImGui::EndChild();

			ImGui::NextColumn();

			ImGui::BeginChild("Skins and shit", ImVec2(0, ImGui::GetContentRegionAvail().y));
			ImGui::Text(g_weebwarecfg.weapon_option_name.c_str());
			ImGui::Separator();

			ImGui::Checkbox("Enabled", &g_weebwarecfg.skinchanger_enabled);
			ImGui::Checkbox("StatTrak", &g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].stattrak_enabled);
			if (g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].stattrak_enabled)
				ImGui::InputInt("Kills", &g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].stattrak_kill_count);

			ImGui::Text("Nametag");
			ImGui::InputText("##weaponname", g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].weapon_name, ARRAYSIZE(g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].weapon_name));
			ImGui::Text("Wear");
			ImGui::SliderFloat("Wear##skins", &g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].m_wear, 0, 100, "%.0f%%");
			ImGui::InputInt("Seed", &g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].m_seed);
			ImGui::Separator();
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 5);
			ImGui::Text("Skins");
			ImGui::InputText("##Search Skin", g_config_list.skinchanger_skinsearch, ARRAYSIZE(g_config_list.skinchanger_skinsearch));
			ImGui::PopItemWidth();

			auto skin_list = filtered_skins();

			ImGui::Separator();
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
			ImGui::Separator();
			if (ImGui::Button("Force Update", ImVec2(ImGui::GetContentRegionAvailWidth(), 25)))
				g_weebwarecfg.skinchanger_apply_nxt = 1;
			ImGui::EndChild();
		}

#pragma endregion

#pragma region Configs
		if (tab_selection == tabs::settings) {
			ImGui::Columns(2, "Settings", false);
			{

				ImGui::BeginChild("configs", ImVec2(0, 0), true);
				{
					ImGui::Text("Local Configs:");
					ImGui::Separator();

					if (ImGui::Button("Refresh", ImVec2(80, 20))) {
						g_config_list.update_all_configs();
					}
					ImGui::Separator();
					ImGui::Text("New Save");
					ImGui::Separator();
					ImGui::InputText("Config Name", g_config_list.cur_save_name, ARRAYSIZE(g_config_list.cur_save_name));
					if (ImGui::Button("Save", ImVec2(80, 20)))
					{
						g_config_list.save_weebware_config();
					}
					ImGui::Separator();
					ImGui::Text("Load");
					ImGui::Separator();
					ImGui::BeginChild("Existing Configs", ImVec2(0, ImGui::GetFrameHeight() - 55), false);
					for (auto cfg : g_config_list.config_names)
					{
						if (!strstr(cfg.c_str(), "weebwareskins")) {
							if (ImGui::Selectable(eraseSubStr(cfg, ".weebware").c_str(), g_config_list.cur_load_name == cfg.c_str()))
							{
								g_config_list.cur_load_name = cfg.c_str();
							}
						}
					}
					ImGui::EndChild();
					ImGui::Separator();
					float config_width = ImGui::GetContentRegionAvailWidth() - 12;
					ImGui::BeginChild("Config Loading", ImVec2(0, 22), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

					if (ImGui::Button("Update", ImVec2(config_width / 3, 20)))
					{
						g_config_list.save_existing_weebware();
					}
					ImGui::SameLine();
					if (ImGui::Button("Load", ImVec2(config_width / 3, 20)))
					{
						g_config_list.load_weebware_config(g_config_list.cur_load_name);
						g_weebwarecfg.skinchanger_apply_nxt = 1;
					}
					ImGui::SameLine();
					if (ImGui::Button("Delete", ImVec2(config_width / 3, 20)))
					{
						g_config_list.delete_weebware_config();
					}
					ImGui::EndChild();
				}
				ImGui::EndChild();

				ImGui::NextColumn();

				ImGui::BeginChild("Browser", ImVec2(0, 0), true);
				{
					ImGui::Text("Online Configs:");
					ImGui::Separator();
					ImGui::Text("Your Configs:");
					ImGui::Separator();
					ImGui::BeginChild("Configs##yours", ImVec2(0, 200), false);
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
					ImGui::Separator();
					float your_config_width = ImGui::GetContentRegionAvailWidth() - 12;
					ImGui::BeginChild("Your Config Loading", ImVec2(0, 22), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

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
					ImGui::EndChild();
					ImGui::Separator();
					ImGui::Text("Favorited Configs:");
					ImGui::Separator();
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
					ImGui::Separator();
					float fav_config_width = ImGui::GetContentRegionAvailWidth() - 12;
					ImGui::BeginChild("Favorited Config Loading", ImVec2(0, 22), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

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
					ImGui::EndChild();
				}
				ImGui::EndChild();
			}
		}
#pragma endregion

		ImGui::End();

		// Clear font for next frame
		ImGui::PopFont();
	}
	else {
		stage_header = -15;
		stage_body = screen_height + 5.f;
	}


	ImGui::Render();
}

*/