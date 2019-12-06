#include "Header.h"
#include "shared.h"
#include "esp.h"
#include "frame_stage.h"
#include "imgui_custom.h"
#include "imgui\imgui_internal.h"
#include "hook_funcs.h"
#include <intrin.h>
#include "imgui/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// https://www.unknowncheats.me/forum/direct3d/66594-d3d9-vtables.html

#pragma region Prototyping
bool has_d3d = false;
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void imgui_main(IDirect3DDevice9* pDevice);
#pragma endregion

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
#if 0

long c_hooking::hk_end_scene(IDirect3DDevice9* device)
{
	DWORD v_state = D3DZB_TRUE; //var used to obtain the CURRENT renderstate (usefull if you want to set a texture if target is visible and not visible)
								//your set texture/pixelshader if target not visible, optional
	device->GetRenderState(D3DRS_ZENABLE, &v_state);
	device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE); //Disable depth buffering and draw the model
	DWORD old_color_write_enable;
	device->GetRenderState(D3DRS_COLORWRITEENABLE, &old_color_write_enable);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
	DWORD old_src_blend;
	device->GetRenderState(D3DRS_SRCBLEND, &old_src_blend);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	DWORD old_dest_blend;
	device->GetRenderState(D3DRS_DESTBLEND, &old_dest_blend);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

	g_esp.esp_main(device);

	imgui_main(device);

	device->SetRenderState(D3DRS_ZENABLE, v_state);//Restore original depth buffer and set pixelshader or texture if target is really visible
	device->SetRenderState(D3DRS_COLORWRITEENABLE, old_color_write_enable);
	device->SetRenderState(D3DRS_SRCBLEND, old_src_blend);
	device->SetRenderState(D3DRS_DESTBLEND, old_dest_blend);
	return reinterpret_cast<long(__stdcall*)(IDirect3DDevice9*)>(g_hooking.vmt_direct_x.get_origin_func(42))(device);;
}
#endif

std::string eraseSubStr(std::string mainStr, std::string toErase)
{
	// Search for the substring in string
	size_t pos = mainStr.find(toErase);

	if (pos != std::string::npos)
		mainStr.erase(pos, toErase.length());

	return mainStr;

}

long hook_functions::end_scene(IDirect3DDevice9* device)
{
	static auto wanted_ret_address = _ReturnAddress();
	if (_ReturnAddress() == wanted_ret_address)
	{
		//backup render states
		DWORD colorwrite, srgbwrite;
		device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
		device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

		//fix drawing without calling engine functons/cl_showpos
		device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		//removes the source engine color correction
		device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

		imgui_main(device);

		device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
		device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
	}
	return g_hooking.o_endscene(device);

	// return PLH::FnCast(g_hooking.endscene_tramp, g_hooking.o_endscene)(device);
}

long hook_functions::present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND wnd_override, const RGNDATA* dirty_region)
{
	static auto wanted_ret_address = _ReturnAddress();

	if (_ReturnAddress() == wanted_ret_address)
	{
		//backup render states
		DWORD colorwrite, srgbwrite;
		device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
		device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

		//fix drawing without calling engine functons/cl_showpos
		device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		//removes the source engine color correction
		device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

		imgui_main(device);

		device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
		device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
	}

	return g_hooking.original_present(device, src, dest, wnd_override, dirty_region);
}

long hook_functions::reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_param)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	// auto hr = PLH::FnCast(g_hooking.reset_tramp, g_hooking.o_reset)(device, presentation_param);
	auto hr = g_hooking.o_reset(device, presentation_param);

	ImGui_ImplDX9_CreateDeviceObjects();

	return hr;
}


// Call imgui setup
ImFont* pFont[2];
extern unsigned int myfont_compressed_data[71052 / 4];
static const unsigned int myfont_compressed_size = 71051;

void imgui_setup(IDirect3DDevice9* pDevice)
{
	ImGui::CreateContext();
	ImGui_ImplDX9_Init(g_weebware.h_window, pDevice);

	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(5, 5);
	style->WindowRounding = 0.f;
	style->FramePadding = ImVec2(4, 4);
	style->FrameRounding = 0.f;
	style->ItemSpacing = ImVec2(6, 6);
	style->IndentSpacing = 2.0f;
	style->ScrollbarSize = 8.f;
	style->ScrollbarRounding = 0.f;
	style->GrabMinSize = 5.0f;
	style->ItemInnerSpacing = ImVec2(2, 5);

	// NORMAL THEME (0.541, 0.169, 0.886)
	// HALLOWEEN THEME (0.922, 0.482, 0.102)

	style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = imgui_custom::ConvertFromRGBA(ImVec4(20.f, 20.f, 20.f, 255.f));
	style->Colors[ImGuiCol_ChildWindowBg] = imgui_custom::ConvertFromRGBA(ImVec4(17.f, 17.f, 17.f, 255.f));
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

	// Change active stuff later.
	style->Colors[ImGuiCol_CheckMark] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 255.f));
	style->Colors[ImGuiCol_SliderGrab] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 200.f));
	style->Colors[ImGuiCol_SliderGrabActive] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 200.f));
	style->Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
	style->Colors[ImGuiCol_PopupBg] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
	style->Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
	style->Colors[ImGuiCol_ButtonActive] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 255.f));
	style->Colors[ImGuiCol_Header] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 100.f));
	style->Colors[ImGuiCol_HeaderHovered] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 200.f));
	style->Colors[ImGuiCol_HeaderActive] = imgui_custom::ConvertFromRGBA(ImVec4(217, 80, 196, 255.f));

	style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 0.30f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);

#if 0
	// imgui has now removed to use standard button colors.
	style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	// idk also gone
	style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
#endif

	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
	ImGuiIO& io = ImGui::GetIO();
#define ICON_MIN_FA 0xf000
#define ICON_MAX_FA 0xf496
	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig config;
	config.MergeMode = true;
	pFont[0] = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 13);
	//pFont[1] = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\fa-solid-900.ttf", 50, &config, icon_ranges);
	pFont[1] = io.Fonts->AddFontFromMemoryCompressedTTF(myfont_compressed_data, myfont_compressed_size, 30, &config, icon_ranges);
	// https://github.com/juliettef/IconFontCppHeaders/blob/master/IconsFontAwesome5.h
	// https://github.com/ocornut/imgui/tree/master/misc/fonts
	has_d3d = true;
}
enum tabs {
	legit,
	rage,
	antiaim,
	vis,
	misc,
	sets,
	skins
};


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

void imgui_main(IDirect3DDevice9* pDevice)
{
	if (!has_d3d) {
		imgui_setup(pDevice);
	}

	ImGui::GetIO().MouseDrawCursor = g_weebware.menu_opened;


	ImGui_ImplDX9_NewFrame();
	int screen_width = 0, screen_height = 0;
	g_weebware.g_engine->get_screen_dimensions(screen_width, screen_height);
	int new_width = 800;

	static bool VisualsRelease = false;

	if (g_weebware.menu_opened)
	{
		int key_counter = 0;

		int pX, pY;
		g_weebware.g_input_system->GetCursorPosition(&pX, &pY);
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos.x = (float)(pX);
		io.MousePos.y = (float)(pY);

		ImGui::Begin("weebware", &g_weebware.menu_opened, ImVec2(800, 600), 1.f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		{
			auto& style = ImGui::GetStyle();
			static int selected_tab = 0;

			ImGui::Columns(2, "Menu", false);
			{
				ImGui::PushFont(pFont[1]);

				style.Colors[ImGuiCol_ChildWindowBg] = imgui_custom::ConvertFromRGBA(ImVec4(20.f, 20.f, 20.f, 255.f));

				ImGui::SetColumnOffset(1, 100);

				ImGui::BeginChild("COL1", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
				{
					style.Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(20.f, 20.f, 20.f, 255.f));
					style.Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(20.f, 20.f, 20.f, 255.f));
					style.Colors[ImGuiCol_ButtonActive] = imgui_custom::ConvertFromRGBA(ImVec4(20.f, 20.f, 20.f, 255.f));

					ImGui::Spacing();
					imgui_custom::create_tab(selected_tab, tabs::legit, imgui_custom::tab_pos::first, "Legit");
					imgui_custom::create_tab(selected_tab, tabs::rage, imgui_custom::tab_pos::middle, "Rage");
					imgui_custom::create_tab(selected_tab, tabs::antiaim, imgui_custom::tab_pos::middle, "Anti-Aim");
					imgui_custom::create_tab(selected_tab, tabs::vis, imgui_custom::tab_pos::middle, "Visuals");
					imgui_custom::create_tab(selected_tab, tabs::misc, imgui_custom::tab_pos::middle, "Misc");
					imgui_custom::create_tab(selected_tab, tabs::skins, imgui_custom::tab_pos::middle, "Skins");
					imgui_custom::create_tab(selected_tab, tabs::sets, imgui_custom::tab_pos::last, "Configs");
					style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);
					style.Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(160, 10, 10, 255.f));
					style.Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(160, 10, 10, 255.f));
					style.Colors[ImGuiCol_ButtonActive] = imgui_custom::ConvertFromRGBA(ImVec4(160, 10, 10, 255.f));
					style.Colors[ImGuiCol_ChildWindowBg] = imgui_custom::ConvertFromRGBA(ImVec4(160, 10, 10, 255.f));
					if (ImGui::Button("Unload", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y))) {
						try {
							g_hooking.unhook_all_functions();
						}
						catch (...) {}
					}
					ImGui::Spacing();


					// Restore
					style.Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
					style.Colors[ImGuiCol_ChildWindowBg] = imgui_custom::ConvertFromRGBA(ImVec4(17.f, 17.f, 17.f, 255.f));
					style.Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
					style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.541, 0.169, 0.886, 1.0f);
					style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);

				}
				ImGui::EndChild();
			}
			ImGui::PopFont();

			ImGui::PushFont(pFont[0]);
			ImGui::NextColumn();
			{
				ImGui::BeginChild("COL2", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
				{
#pragma region Legit
					if (selected_tab == tabs::legit) {

						ImGui::BeginChild("Type Sizing", ImVec2(ImGui::GetContentRegionAvailWidth() - 2, 30), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
						const char* weapon_groups[] = { "Pistols", "Rifles", "SMG","Shotguns", "Heavy", "Auto-Snipers", "AWP", "SSG08" };

						for (auto x : weapon_groups) {

						}

						imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 0, "Pistols");
						ImGui::SameLine();
						ImGui::VerticalSeparator();
						ImGui::SameLine();
						imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 1, "Rifles");
						ImGui::SameLine();
						ImGui::VerticalSeparator();
						ImGui::SameLine();
						imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 2, "SMG");
						ImGui::SameLine();
						ImGui::VerticalSeparator();
						ImGui::SameLine();
						imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 3, "Shotguns");
						ImGui::SameLine();
						ImGui::VerticalSeparator();
						ImGui::SameLine();
						imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 4, "Heavy");
						ImGui::SameLine();
						ImGui::VerticalSeparator();
						ImGui::SameLine();
						imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 5, "Auto");
						ImGui::SameLine();
						ImGui::VerticalSeparator();
						ImGui::SameLine();
						imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 6, "AWP");
						ImGui::SameLine();
						ImGui::VerticalSeparator();
						ImGui::SameLine();
						imgui_custom::create_button_tab(g_weebwarecfg.legit_cfg_index, 7, "SSG08");
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
							ImGui::Checkbox("Silent aim", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].silent_aim, false);
							ImGui::Checkbox("Target teammates", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].target_teammates, false);
							ImGui::Checkbox("Distance FOV", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].use_dynamicfov, false);

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

							ImGui::Checkbox("Quickstop##legit", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].quick_stop, false);

							ImGui::Checkbox("Aim through smoke", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].aim_through_smoke, false);

							ImGui::Checkbox("Aim while blind", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].aim_while_blind, false);

							ImGui::Checkbox("Head", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].hitbox_head, false);
							ImGui::Checkbox("Chest", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].hitbox_chest, false);
							ImGui::Checkbox("Stomach", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].hitbox_stomach, false);

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

							ImGui::Checkbox("Head##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_head, false);
							ImGui::SameLine();
							ImGui::Checkbox("Chest##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_chest, false);
							ImGui::SameLine();
							ImGui::Checkbox("Stomach##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_stomach, false);
							ImGui::Checkbox("Arms##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_arms, false);
							ImGui::SameLine();
							ImGui::Checkbox("Legs##tb", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_legs, false);
							ImGui::Separator();
							ImGui::Text("Magnet Triggerbot");
							ImGui::Separator();
							ImGui::Checkbox("Enable##magnet", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].magnet_triggerbot_enabled, false);
							ImGui::Text("Maximum FOV");
							ImGui::SliderFloat("FOV##magnettrigger", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].magnet_trigger_fov, 0, 180.f, "%.1f");
							ImGui::Text("Sensitivity");
							ImGui::SliderFloat("Smooth##magnettrigger", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].magnet_trigger_smooth, 0, 100.f, "%.1f");
							ImGui::Text("Target Switch Delay");
							ImGui::SliderInt("Target Switch Delay##trigger", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_target_switch_delay, 0, 2000, "%.0f");
							ImGui::Checkbox("Quickstop##magnet", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].quick_stop_magnet, false);
							ImGui::Checkbox("Scoped Only", &g_weebwarecfg.triggerbot_scoped_only, false);

						}
						ImGui::EndChild();
						ImGui::BeginChild("Other", ImVec2(0, 0), true);
						{
							ImGui::Text("Other");
							ImGui::Separator();
							ImGui::Checkbox("Standalone RCS", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].standalone_rcs, false);
							ImGui::Text("RCS Factor");
							ImGui::SliderFloat("RCS Factor", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].standalone_rcs_power, 0, 100, "%.0f%%");
							ImGui::Checkbox("Backtracking", (bool*)&g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].accuracy_boost, false);
							ImGui::Text("Maximum Ticks");
							ImGui::SliderFloat("Maximum Ticks", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].legit_maximum_ticks, 0, 30, "%.f%");
						}
						ImGui::EndChild();

					}
#pragma endregion


#pragma region rage
					if (selected_tab == tabs::rage)
					{
						//ImGui::BeginChild("header", ImVec2(ImGui::GetContentRegionAvailWidth(), 35), true);
						//ImGui::Checkbox("Safe Mode", &g_weebwarecfg.safemode, false);
						//ImGui::SameLine();
						//ImGui::VerticalSeparator();
						//ImGui::SameLine();
						//style.Colors[ImGuiCol_Text] = imgui_custom::ConvertFromRGBA(ImVec4(200, 10, 10, 255.f));
						//ImGui::Text("By disabling safe mode, you put your account at much greater risk of ban.");
						//style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);
						//ImGui::EndChild();

						//ImGui::Columns(2, "rage", false);
						//{
						//	ImGui::BeginChild("rage 1", ImVec2(0, 0), true);
						//	{
						//		ImGui::Checkbox("Enabled##rage", &g_weebwarecfg.ragebot_enabled, false);
						//		ImGui::Checkbox("Silent Aim##ragebot", &g_weebwarecfg.ragebot_silent_aim, false);
						//		ImGui::Checkbox("No Recoil", &g_weebwarecfg.no_recoil, false);
						//		ImGui::Checkbox("Target Teammates##rage", &g_weebwarecfg.ragebot_target_team, false);
						//		ImGui::Checkbox("Quickstop##rage", &g_weebwarecfg.ragebot_autostop, false);
						//		ImGui::Checkbox("Autoshoot", &g_weebwarecfg.autoshoot_enabled, false);
						//		ImGui::Text("Autowall Minimum Damage");
						//		ImGui::SliderFloat("Autowall##mindamage", &g_weebwarecfg.autowall_min_dmg, 0, 100, "%.f%");
						//		ImGui::Text("Hitchance");
						//		ImGui::SliderFloat("Hitchance##rageslider", &g_weebwarecfg.ragebot_hitchance, 0, 100, "%.0f%%");
						//	}
						//	ImGui::EndChild();

						//	ImGui::NextColumn();

						//	ImGui::BeginChild("rage 2", ImVec2(0, 0), true);
						//	{
						//
						//	}
						//	ImGui::EndChild();

						//}

					}
#pragma endregion


#pragma region antiaim
					if (selected_tab == tabs::antiaim)
					{
						ImGui::Columns(2, "antiaim", false);
						{
							ImGui::BeginChild("antiaim 1", ImVec2(0, 0), true);
							{

							}
							ImGui::EndChild();

							ImGui::NextColumn();

							ImGui::BeginChild("antiaim 2", ImVec2(0, 0), true);
							{

							}
							ImGui::EndChild();

						}

					}
#pragma endregion


#pragma region Visuals
					if (selected_tab == tabs::vis) {
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
								ImGui::Checkbox("Teammates", &g_weebwarecfg.visuals_teammates, false);
								if (g_weebwarecfg.visuals_teammates) {
									imgui_custom::custom_color_inline(g_weebwarecfg.team_visible_col, g_weebwarecfg.team_hidden_col, true, "Visible Color (Team)##Team1", "Hidden Color (Team)##Team2");
								}
								ImGui::Checkbox("Visible only", &g_weebwarecfg.visuals_visible_only, false);

								ImGui::Checkbox("Bounding Box", &g_weebwarecfg.visuals_bounding_box, false);
								if (g_weebwarecfg.visuals_bounding_box) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_bounding_col_visible, g_weebwarecfg.visuals_bounding_col_hidden, true, "Visible Color (Enemy)##box1", "Hidden Color (Enemy)##box2");
								}

								ImGui::Checkbox("Corner Box", &g_weebwarecfg.visuals_corner_box, false);
								if (g_weebwarecfg.visuals_corner_box) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_corner_col_visible, g_weebwarecfg.visuals_corner_col_hidden, true, "Visible Color (Enemy)##corner1", "Hidden Color (Enemy)##corner2");
								}

								ImGui::Checkbox("Skeleton", &g_weebwarecfg.visuals_skeleton, false);
								if (g_weebwarecfg.visuals_skeleton) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_skeleton_col_visible, g_weebwarecfg.visuals_skeleton_col_hidden, true, "Visible Color (Enemy)##skele1", "Hidden Color (Enemy)##skele2");
								}


								ImGui::Checkbox("Health Bar", &g_weebwarecfg.visuals_health_bars, false);

								ImGui::Checkbox("Name", &g_weebwarecfg.visuals_name_esp, false);
								if (g_weebwarecfg.visuals_name_esp) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_name_esp_col_visible, g_weebwarecfg.visuals_name_esp_col_hidden, true, "Visible Color (Enemy)##name1", "Hidden Color (Enemy)##name2");
								}

								ImGui::Checkbox("Weapon", &g_weebwarecfg.visuals_weapon_esp, false);
								if (g_weebwarecfg.visuals_weapon_esp) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_weapon_esp_col, "Weapon ESP Color");
								}

								ImGui::Checkbox("Ammo", &g_weebwarecfg.visuals_ammo_esp, false);
								if (g_weebwarecfg.visuals_ammo_esp) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_ammo_esp_col, "Ammo ESP Color");
								}

								ImGui::Checkbox("Defusing Indicator", &g_weebwarecfg.defusing_indicator, false);
								if (g_weebwarecfg.defusing_indicator) {
									imgui_custom::custom_color_inline(g_weebwarecfg.defusing_indicator_col, "dfcol##1");
								}

								ImGui::Checkbox("Draw On Dormant", &g_weebwarecfg.visuals_dormant_esp, false);
								if (g_weebwarecfg.visuals_dormant_esp) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_dormant_col, g_weebwarecfg.visuals_dormant_col_team, g_weebwarecfg.visuals_teammates, "Visible Color (Enemy)##dormant1", "Hidden Color (Enemy)##dormant2");
								}
								ImGui::Checkbox("Show On Radar", &g_weebwarecfg.visuals_bspotted, false);

								ImGui::Separator();
								ImGui::Text("Glow");
								ImGui::Separator();

								ImGui::Checkbox("Enabled##Glow", &g_weebwarecfg.visuals_glow_enabled, false);

								ImGui::Checkbox("Players", &g_weebwarecfg.visuals_glow_player, false);
								if (g_weebwarecfg.visuals_glow_player) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_glow_player_col_visible, g_weebwarecfg.visuals_glow_player_col_hidden, g_weebwarecfg.visuals_glow_hidden_col, "Glow Color (Visible)", "Glow Color (Hidden)");
									ImGui::Checkbox("Hidden Color", &g_weebwarecfg.visuals_glow_hidden_col, false);
								}

								ImGui::Checkbox("Team##glow", &g_weebwarecfg.visuals_glow_team, false);
								if (g_weebwarecfg.visuals_glow_team) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_glow_team_col, "Team Glow Color");
								}

								ImGui::Checkbox("Weapon##glow", &g_weebwarecfg.visuals_glow_weapon, false);
								if (g_weebwarecfg.visuals_glow_weapon) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_glow_weapon_col, g_weebwarecfg.visuals_glow_weapon_col, false, "Weapon Color", "##glowwep");
								}

								ImGui::Checkbox("Bomb", &g_weebwarecfg.visuals_glow_c4, false);
								if (g_weebwarecfg.visuals_glow_c4) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_glow_c4_col, g_weebwarecfg.visuals_glow_c4_col, false, "Bomb Color", "##glowbomb");
								}

								ImGui::Checkbox("Chicken", &g_weebwarecfg.visuals_glow_chicken, false);
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
								ImGui::Checkbox("Render Team", &g_weebwarecfg.visuals_chams_render_team, false);

								ImGui::Checkbox("XQZ (Through Materials)", &g_weebwarecfg.visuals_chams_xqz, false);
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
								ImGui::Checkbox("Inaccuracy Circle", &g_weebwarecfg.visuals_inacc_circle, false);
								if (g_weebwarecfg.visuals_inacc_circle) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_innacc_circle_col, "Inacc Color");
								}

								ImGui::Checkbox("Draw FOV", &g_weebwarecfg.visuals_fov_circle, false);
								if (g_weebwarecfg.visuals_fov_circle) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_fov_circle_col, "Circle Color");
								}

								ImGui::Checkbox("Sniper Crosshair", &g_weebwarecfg.visuals_sniper_crosshair, false);
								if (g_weebwarecfg.visuals_sniper_crosshair) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_sniper_crosshair_col, "Sniper Crosshair Color");
								}
								ImGui::Checkbox("Recoil Crosshair", &g_weebwarecfg.visuals_recoil_crosshair, false);
								if (g_weebwarecfg.visuals_recoil_crosshair) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_recoil_crosshair_col, "Recoil Crosshair Color");
								}

								ImGui::Checkbox("Watermark", &g_weebwarecfg.visuals_watermark, false);
								if (g_weebwarecfg.visuals_watermark)
									imgui_custom::custom_color_inline(g_weebwarecfg.water_mark_col, "watermark##1");
								ImGui::Checkbox("Bomb Timer", &g_weebwarecfg.visuals_bomb_timer, false);
								ImGui::Checkbox("Wireframe Smoke", &g_weebwarecfg.wireframe_smoke, false);
								ImGui::Checkbox("Night Sky", &g_weebwarecfg.night_sky, false);
								ImGui::Checkbox("Nightmode", &g_weebwarecfg.visuals_nightmode, false);
								ImGui::Checkbox("Screenshot Proof", &g_weebwarecfg.screenshot_proof, false);
								ImGui::Checkbox("No Smoke", &g_weebwarecfg.no_smoke, false);
								ImGui::Checkbox("Spectator List", &g_weebwarecfg.spec_list, false);
								ImGui::Checkbox("Grenade Trajectory", &g_weebwarecfg.draw_grenade_traj, false);
								ImGui::Checkbox("Hitmarkers", &g_weebwarecfg.visuals_hitmarkers, false);
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

								ImGui::Checkbox("Enabled##bullettracers", &g_weebwarecfg.enable_bullet_tracers, false);
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
								ImGui::Checkbox("Backtrack Skeleton", &g_weebwarecfg.visuals_backtrack_dots, false);
								if (g_weebwarecfg.visuals_backtrack_dots)
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_backtrack_col, "Backtrack Color");

							}

							ImGui::EndChild();

						}
					}
#pragma endregion

#pragma region Misc
					if (selected_tab == tabs::misc) {

						ImGui::Columns(2, "Miscs", false);
						{
							ImGui::BeginChild("Misc 1", ImVec2(0, 0), true);
							{
								ImGui::Text("Misc");
								ImGui::Separator();
								ImGui::Checkbox("Clantag Changer", &g_weebwarecfg.misc_clantag_changer, false);
								if (g_weebwarecfg.misc_clantag_changer) {
									ImGui::InputText("empty for default##clantag", g_weebwarecfg.custom_clantag_static, ARRAYSIZE(g_weebwarecfg.custom_clantag_static));
								}
								ImGui::Checkbox("Chatspam", &g_weebwarecfg.misc_chat_spammer, false);
								ImGui::Checkbox("Preserve Killfeed", &g_weebwarecfg.preserve_killfeed, false);
								ImGui::Checkbox("Auto Pistol", &g_weebwarecfg.auto_pistol, false);
								imgui_custom::custom_inline_keyinput(g_weebwarecfg.auto_pistol_key, key_counter);
								ImGui::Checkbox("Rank Reveal", &g_weebwarecfg.rank_reveal, false);
								ImGui::Checkbox("Killsay", &g_weebwarecfg.killsay, false);
								if (g_weebwarecfg.killsay) {
									ImGui::InputText("empty for default##killsay", g_weebwarecfg.killsay_msg_custom, ARRAYSIZE(g_weebwarecfg.killsay_msg_custom));
								}
								ImGui::Checkbox("Disable Post Processing", &g_weebwarecfg.disable_post_processing, false);
								ImGui::Checkbox("Anti AFK", &g_weebwarecfg.anti_afk, false);
								//	ImGui::Checkbox("Third Person", &g_weebwarecfg.thirdperson, false);
								//	imgui_custom::custom_inline_keyinput(g_weebwarecfg.thirdperson_key, key_counter);
								ImGui::Checkbox("Auto accept", &g_weebwarecfg.misc_autoAccept, false);
								ImGui::Checkbox("Viewmodel Changer", &g_weebwarecfg.viewmodel_changer, false);
								if (g_weebwarecfg.viewmodel_changer) {
									ImGui::SliderInt("Viewmodel Offset", &g_weebwarecfg.viewmodel_offset, -100, 135);
								}
								ImGui::Checkbox("Rainbow Name", &g_weebwarecfg.rainbow_name, false);
								ImGui::Checkbox("Block Bot", &g_weebwarecfg.block_bot, false);
								imgui_custom::custom_inline_keyinput(g_weebwarecfg.block_bot_key, key_counter);
								ImGui::Checkbox("Auto Defuse", &g_weebwarecfg.auto_defuse, false);
								imgui_custom::custom_inline_keyinput(g_weebwarecfg.auto_defuse_key, key_counter);

								//	ImGui::Checkbox("No Flash", &g_weebwarecfg.remove_flash, false);

								ImGui::Separator();
								ImGui::Text("Movement");
								ImGui::Separator();

								ImGui::Checkbox("Bunnyhop", &g_weebwarecfg.auto_jump, false);
								if (g_weebwarecfg.auto_jump) {
									ImGui::SliderInt("Hitchance##bhop", &g_weebwarecfg.auto_jump_hitchance, 0, 100, "%.0f%%");
								}
								//		ImGui::Checkbox("Auto Jump Bug", &g_weebwarecfg.auto_jumpbug, false);
								//		imgui_custom::custom_inline_keyinput(g_weebwarecfg.auto_jumpbug_key, key_counter);
								ImGui::Checkbox("Infinite Duck", &g_weebwarecfg.no_duck_cooldown, false);
								ImGui::Checkbox("Slidewalk", &g_weebwarecfg.misc_slidewalk, false);
								ImGui::Checkbox("Edge Jump", &g_weebwarecfg.edge_jump, false);
								imgui_custom::custom_inline_keyinput(g_weebwarecfg.edge_jump_key, key_counter);
								if (g_weebwarecfg.edge_jump) {
									ImGui::Checkbox("Duck In Air", &g_weebwarecfg.duck_in_air, false);
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
								ImGui::Checkbox("Enabled", &g_weebwarecfg.misc_ai, false);
								ImGui::Checkbox("Random", &g_weebwarecfg.misc_ai_random, false);
								ImGui::Checkbox("Engage nearest enemy", &g_weebwarecfg.misc_ai_nearest, false);
								ImGui::Checkbox("Defuse bombs", &g_weebwarecfg.misc_ai_defuse, false);
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
									ImGui::SliderInt("Fakelag Factor", &g_weebwarecfg.fake_lag_factor, 0, 12);
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

#pragma region Skins
					if (selected_tab == tabs::skins) {

						ImGui::Columns(2, "SkinChanger", false);

						float skin_tab_height = ImGui::GetContentRegionAvail().y;
						ImGui::BeginChild("Knife", ImVec2(0, skin_tab_height / 5), true);
						{
							ImGui::Text("Knife Options");
							ImGui::Separator();
							ImGui::Checkbox("Enabled##knifechanger", &g_weebwarecfg.knifechanger_enabled, false);
							const char* knives[] = { "Terrorist Knife", "Counter-Terrorist Knife", "Flip", "Gut", "Karambit", "M9 Bayonet", "Bayonet", "Huntsman", "Falchion", "Stiletto", "Ursus", "Navaja", "Talon", "Butterfly", "Shadow Daggers", "Bowie", "Classic", "Paracord", "Survival", "Skeleton", "Nomad" };
							ImGui::Combo("##knifecombo", &g_weebwarecfg.selected_knife_index[1], knives, ARRAYSIZE(knives));
							g_weebwarecfg.selected_knife_index[0] = g_weebware.g_knife_list[g_weebwarecfg.selected_knife_index[1]].weapon_index;
						}
						ImGui::EndChild();

						ImGui::BeginChild("Gloves", ImVec2(0, ((skin_tab_height / 5) * 3) - 5), true);
						{
							ImGui::Text("Glove Options");
							ImGui::Separator();
							ImGui::Checkbox("Enabled##glovechanger", &g_weebwarecfg.glovechanger_enabled, false);
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

								if (ImGui::Button("Apply", ImVec2(ImGui::GetContentRegionAvailWidth() / 1.5, 25), ImGuiButtonFlags_Outlined)) {

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

						ImGui::BeginChild("Skins and shit", ImVec2(0, ImGui::GetContentRegionAvail().y), true);
						ImGui::Text(g_weebwarecfg.weapon_option_name.c_str());
						ImGui::Separator();

						ImGui::Checkbox("Enabled", &g_weebwarecfg.skinchanger_enabled, false);
						ImGui::Checkbox("StatTrak", &g_weebwarecfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].stattrak_enabled, false);
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
						if (ImGui::Button("Force Update", ImVec2(ImGui::GetContentRegionAvailWidth(), 25), ImGuiButtonFlags_Outlined))
							g_weebwarecfg.skinchanger_apply_nxt = 1;
						ImGui::EndChild();
					}
#pragma endregion

#pragma region Settings
					if (selected_tab == tabs::sets) {

						ImGui::Columns(2, "Settings", false);
						{

							ImGui::BeginChild("configs", ImVec2(0, 0), true);
							{
								ImGui::Text("Local Configs:");
								ImGui::Separator();

								if (ImGui::Button("Refresh", ImVec2(80, 20), ImGuiButtonFlags_Outlined)) {
									g_config_list.update_all_configs();
								}
								ImGui::Separator();
								ImGui::Text("New Save");
								ImGui::Separator();
								ImGui::InputText("Config Name", g_config_list.cur_save_name, ARRAYSIZE(g_config_list.cur_save_name));
								if (ImGui::Button("Save", ImVec2(80, 20), ImGuiButtonFlags_Outlined))
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

								if (ImGui::Button("Update", ImVec2(config_width / 3, 20), ImGuiButtonFlags_Outlined))
								{
									g_config_list.save_existing_weebware();
								}
								ImGui::SameLine();
								if (ImGui::Button("Load", ImVec2(config_width / 3, 20), ImGuiButtonFlags_Outlined))
								{
									g_config_list.load_weebware_config(g_config_list.cur_load_name);
									g_weebwarecfg.skinchanger_apply_nxt = 1;
								}
								ImGui::SameLine();
								if (ImGui::Button("Delete", ImVec2(config_width / 3, 20), ImGuiButtonFlags_Outlined))
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

								if (ImGui::Button("Refresh##yours", ImVec2(your_config_width / 3, 20), ImGuiButtonFlags_Outlined))
								{
									g_config_list.get_your_configs();
								}
								ImGui::SameLine();
								if (ImGui::Button("Load##yours", ImVec2(your_config_width / 3, 20), ImGuiButtonFlags_Outlined))
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

								if (ImGui::Button("Refresh##fav", ImVec2(fav_config_width / 3, 20), ImGuiButtonFlags_Outlined))
								{
									g_config_list.get_favorited_configs();
								}
								ImGui::SameLine();
								if (ImGui::Button("Load##fav", ImVec2(fav_config_width / 3, 20), ImGuiButtonFlags_Outlined))
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

				}

				ImGui::EndChild();
			}

			ImGui::PopFont();


		}
		ImGui::End();
	}

	ImGui::Render();
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/bb174369(v=vs.85).aspx 
// STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount) PURE;
// 82

struct d3dchams_indexing
{
	D3DPRIMITIVETYPE type;
	INT base_vertex_index;
	UINT min_index;
	UINT num_vertices;
	UINT start_index;
	UINT prim_count;
};

d3dchams_indexing ent_data[99];

#if 0
HRESULT draw_indexed_primitive(IDirect3DDevice9 *device, D3DPRIMITIVETYPE type, INT base_vertex_index, UINT min_index, UINT num_vertices, UINT start_index, UINT prim_count)
{
	// Save the data to the struct.
	if (m_ent)
	{
		d3dchams_indexing tmp;
		tmp.type = type;
		tmp.base_vertex_index = base_vertex_index;
		tmp.min_index = min_index;
		tmp.num_vertices = num_vertices;
		tmp.start_index = start_index;
		tmp.prim_count = prim_count;
		ent_data[m_ent->index] = tmp;
	}
	// Return original
}

#endif