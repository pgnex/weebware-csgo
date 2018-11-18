#include "Header.h"
#include "shared.h"
#include "esp.h"
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
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.161f, 0.502f, 0.725f, 1.0f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.161f, 0.502f, 0.725f, 0.7f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.161f, 0.502f, 0.725f, 0.7f);
	style->Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
	style->Colors[ImGuiCol_PopupBg] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
	style->Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.161f, 0.502f, 0.725f, 1.0f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.161f, 0.502f, 0.725f, 0.4f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.161f, 0.502f, 0.725f, 0.7f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.161f, 0.502f, 0.725f, 1.0f);

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
	vis,
	misc,
	sets,
	paint
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

	static bool VisualsRelease = false;

	if (g_weebware.menu_opened)
	{
		int key_counter = 0;

		int pX, pY;
		g_weebware.g_input_system->GetCursorPosition(&pX, &pY);
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos.x = (float)(pX);
		io.MousePos.y = (float)(pY);

		ImGui::Begin(("weebware priv 2.0"), &g_weebware.menu_opened, ImVec2(700, 615), 1.f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
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
					imgui_custom::create_tab(selected_tab, tabs::legit, imgui_custom::tab_pos::first, u8"\uf05b");
					imgui_custom::create_tab(selected_tab, tabs::rage, imgui_custom::tab_pos::middle, u8"\uf21d");
					imgui_custom::create_tab(selected_tab, tabs::vis, imgui_custom::tab_pos::middle, u8"\uf06e");
					imgui_custom::create_tab(selected_tab, tabs::misc, imgui_custom::tab_pos::middle, u8"\uf1de");
					imgui_custom::create_tab(selected_tab, tabs::paint, imgui_custom::tab_pos::middle, u8"\uf1fc");
					imgui_custom::create_tab(selected_tab, tabs::sets, imgui_custom::tab_pos::last, u8"\uf085");
					ImGui::Spacing();


					// Restore
					style.Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
					style.Colors[ImGuiCol_ChildWindowBg] = imgui_custom::ConvertFromRGBA(ImVec4(17.f, 17.f, 17.f, 255.f));
					style.Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
					style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.161f, 0.502f, 0.725f, 1.0f);
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
					if (selected_tab == tabs::legit)
					{
						ImGui::Text("Weapon Type");
						ImGui::BeginChild("Type Sizing", ImVec2(ImGui::GetContentRegionAvailWidth() / 2, 20), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
						const char* weapon_groups[] = { "Pistols", "Rifles", "SMG","Shotguns", "Heavy", "Auto-Snipers", "AWP", "SSG08" };
						ImGui::Combo("", &g_weebwarecfg.legit_cfg_index, weapon_groups, ARRAYSIZE(weapon_groups));
						ImGui::EndChild();


						ImGui::Columns(2, "Legit", false);
						{
							ImGui::SetColumnOffset(1, 290);

							ImGui::BeginChild("Legit 1", ImVec2(0, 0), true);
							{
								ImGui::Text("Aimbot");
								ImGui::Separator();

								ImGui::Text("Activation type");
								imgui_custom::custom_inline_keyinput(g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].legitbot_activation_key, key_counter);

								const char* activation_type[] = { "Off", "On Fire", "On Key", "Magnetic" };
#if 0
								ImGui::Checkbox("Enabled", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].enable_legitbot, false);
#endif
								ImGui::Combo("", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].enable_legitbot, activation_type, ARRAYSIZE(activation_type));
								ImGui::Checkbox("Silent aim", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].silent_aim, false);

								ImGui::Checkbox("Distance FOV", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].use_dynamicfov, false);

								ImGui::Text("Maximum FOV");
								ImGui::SliderFloat("Maximum FOV", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].maximum_fov, 0, 30, "%.1f");

								ImGui::Text("Sensitivity");
								ImGui::SliderFloat("Sensitivity", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].sensitivity, 0, 100, "%.0f%%");

								ImGui::Text("First Shot Delay");
								ImGui::SliderFloat("First Shot Delay", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].reaction_time, 0, 200, "%.0fms");

								ImGui::Text("Recoil compensation (P/Y)");
								ImGui::SliderFloat("Recoil compensation P", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].pitch_rcs, 0, 100, "%.0f%%");
								ImGui::SliderFloat("Recoil compensation Y", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].yaw_rcs, 0, 100, "%.0f%%");

								ImGui::Checkbox("Quick stop", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].quick_stop, false);

								ImGui::Checkbox("Aim through smoke", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].aim_through_smoke, false);

								ImGui::Checkbox("Aim while blind", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].aim_while_blind, false);

								ImGui::Checkbox("Head", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].hitbox_head, false);
								ImGui::Checkbox("Chest", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].hitbox_chest, false);
								ImGui::Checkbox("Stomach", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].hitbox_stomach, false);
							}
							ImGui::EndChild();

							ImGui::NextColumn();

							ImGui::BeginChild("Legit 2", ImVec2(0, ImGui::GetContentRegionAvail().y / 3 * 2), true);
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

								ImGui::Checkbox("Head", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_head, false);
								ImGui::Checkbox("Chest", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_chest, false);
								ImGui::Checkbox("Stomach", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_stomach, false);

							}
							ImGui::EndChild();

							ImGui::BeginChild("Other", ImVec2(0, 0), true);
							{
								ImGui::Checkbox("Standalone RCS", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].standalone_rcs, false);
								ImGui::Text("RCS Factor");
								ImGui::SliderFloat("RCS Factor", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].standalone_rcs_power, 0, 100, "%.0f%%");
								ImGui::Text("Backtracking");
								const char* acc_type[] = { "Off", "Default" };
								ImGui::Combo("", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].accuracy_boost, acc_type, ARRAYSIZE(acc_type));
								ImGui::Text("Maximum Ticks");
								ImGui::SliderFloat("Maximum Ticks", &g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].legit_maximum_ticks, 0, 30, "%.f%");
							}
							ImGui::EndChild();

						}

					}
#pragma endregion


#pragma region modelstuff
					if (selected_tab == tabs::rage)
					{
						ImGui::Columns(2, "MODELCOL", false);
						{
							ImGui::SetColumnOffset(1, 290);

							ImGui::BeginChild("models gay 1", ImVec2(0, 0), true);
							{
								ImGui::Text("Knife Models");
								ImGui::Separator();
								ImGui::Checkbox("Minecraft Pickaxe", &g_weebwarecfg.minecraft_pickaxe, false);

								ImGui::Separator();
								ImGui::Text("Player Models");
								ImGui::Checkbox("Reina Kousaka", &g_weebwarecfg.reina_model, false);

							}
							ImGui::EndChild();

							ImGui::NextColumn();

							ImGui::BeginChild("models gay 2", ImVec2(0, 0), true);
							{
							}
							ImGui::EndChild();

						}
					}
#pragma endregion

#pragma region Visuals
					if (selected_tab == tabs::vis)
					{
						ImGui::Columns(2, "Visuals", false);
						{
							ImGui::SetColumnOffset(1, 290);
							ImGui::BeginChild("Visuals 1", ImVec2(0, 0), true);
							{
								ImGui::Text("Player ESP");
								ImGui::Separator();
								ImGui::Text("Activation type");
								imgui_custom::custom_inline_keyinput(g_weebwarecfg.enable_visuals_key, key_counter);
								const char* activation_type[] = { "Off", "Default", "On Key" };
								ImGui::Combo("##activationtype", &g_weebwarecfg.enable_visuals, activation_type, ARRAYSIZE(activation_type));
								ImGui::Checkbox("Teammates", &g_weebwarecfg.visuals_teammates, false);
								if (g_weebwarecfg.visuals_teammates) {
									imgui_custom::custom_color_inline(g_weebwarecfg.team_visible_col, g_weebwarecfg.team_hidden_col, true, "Visible Color (Team)##Team1", "Hidden Color (Team)##Team2");
								}
								ImGui::Checkbox("Visible only", &g_weebwarecfg.visuals_visible_only, false);

								ImGui::Checkbox("Bounding box", &g_weebwarecfg.visuals_bounding_box, false);
								if (g_weebwarecfg.visuals_bounding_box) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_bounding_col_visible, g_weebwarecfg.visuals_bounding_col_hidden, true, "Visible Color (Enemy)##box1", "Hidden Color (Enemy)##box2");
								}
								ImGui::Checkbox("Health bar", &g_weebwarecfg.visuals_health_bars, false);

								ImGui::Checkbox("Name", &g_weebwarecfg.visuals_name_esp, false);
								if (g_weebwarecfg.visuals_name_esp) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_name_esp_col_visible, g_weebwarecfg.visuals_name_esp_col_hidden, true, "Visible Color (Enemy)##name1", "Hidden Color (Enemy)##name2");
								}

								ImGui::Checkbox("Draw on dormant", &g_weebwarecfg.visuals_dormant_esp, false);
								if (g_weebwarecfg.visuals_dormant_esp) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_dormant_col, g_weebwarecfg.visuals_dormant_col_team, g_weebwarecfg.visuals_teammates, "Visible Color (Enemy)##dormant1", "Hidden Color (Enemy)##dormant2");
								}

								ImGui::Separator();
								ImGui::Text("Glow");
								ImGui::Separator();

								ImGui::Checkbox("Enabled##Glow", &g_weebwarecfg.visuals_glow_enabled, false);

								ImGui::Checkbox("Players", &g_weebwarecfg.visuals_glow_player, false);
								if (g_weebwarecfg.visuals_glow_player) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_glow_player_col, g_weebwarecfg.visuals_glow_player_col, false, "Glow Color", "##glow2");
								}

								ImGui::Checkbox("Bomb", &g_weebwarecfg.visuals_glow_c4, false);
								if (g_weebwarecfg.visuals_glow_c4) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_glow_c4_col, g_weebwarecfg.visuals_glow_c4_col, false, "Bomb Color", "##glow2");
								}

								ImGui::Checkbox("Chicken", &g_weebwarecfg.visuals_glow_chicken, false);
								if (g_weebwarecfg.visuals_glow_chicken) {
									imgui_custom::custom_color_inline(g_weebwarecfg.visuals_glow_chicken_col, g_weebwarecfg.visuals_glow_chicken_col, false, "Chicken Color", "##glow2");
								}



								ImGui::Separator();
								ImGui::Text("Chams");
								ImGui::Separator();

								const char* cham_type[] = { "Default", "Plain", "Platnium", "Glass", "Crystal", "Gold", "Dark Chrome", "Gloss", "Glow","Rim 3D", "Wildfire Gold", "Crystal Blue", "Velvet", "Darude" };
								ImGui::Text("Material");
								ImGui::Combo("##chammaterials", &g_weebwarecfg.visuals_chams, cham_type, ARRAYSIZE(cham_type));
								imgui_custom::custom_color_inline(g_weebwarecfg.visuals_chams_col, g_weebwarecfg.visuals_chams_team_col, 1, "Enemy Color##chams1", "Team Color##chams2");
								ImGui::Checkbox("Render team", &g_weebwarecfg.visuals_chams_render_team, false);

								ImGui::Checkbox("XQZ", &g_weebwarecfg.visuals_chams_xqz, false);
								imgui_custom::custom_color_inline(g_weebwarecfg.visuals_chams_col_xqz, g_weebwarecfg.visuals_chams_team_col_xqz, 1, "Enemy XQZ Color##chams1", "Team XQZ Color##chams2");



							}
							ImGui::EndChild();

							ImGui::NextColumn();

							ImGui::BeginChild("Visuals 2", ImVec2(0, 0), true);
							{
								ImGui::Text("Other ESP");
								ImGui::Separator();


								ImGui::Checkbox("Inaccuracy circle", &g_weebwarecfg.visuals_inacc_circle, false);
								imgui_custom::custom_color_inline(g_weebwarecfg.visuals_innacc_circle_col, "Inacc Color");
								ImGui::Checkbox("Watermark", &g_weebwarecfg.visuals_watermark, false);
								imgui_custom::custom_color_inline(g_weebwarecfg.water_mark_col, "watermark##1");

								ImGui::Separator();
								ImGui::Text("Backtracking");
								ImGui::Text("Style");
								const char* backtrackstyle[] = { "Time", "Single", "All", "Target" };
								ImGui::Combo("##backtrackingtype", &g_weebwarecfg.visuals_backtrack_style, backtrackstyle, ARRAYSIZE(backtrackstyle));
								ImGui::Checkbox("Backtrack Skeleton", &g_weebwarecfg.visuals_backtrack_dots, false);
								imgui_custom::custom_color_inline(g_weebwarecfg.visuals_backtrack_col, "Backtrack Color");
								ImGui::Separator();
								ImGui::Checkbox("Show on radar", &g_weebwarecfg.visuals_bspotted, false);
								ImGui::Checkbox("Bomb Timer", &g_weebwarecfg.visuals_bomb_timer, false);
								ImGui::Checkbox("Hitmarkers", &g_weebwarecfg.visuals_hitmarkers, false);
								imgui_custom::custom_color_inline(g_weebwarecfg.visuals_hitmarker_col, "Hitmarker Color");

								const char* hit_sound[] = { "None", "COD", "Anime", "Bubbles", "Custom" };
								ImGui::Text("Hitmarker Sound");
								ImGui::Combo("##hitmarkersound", &g_weebwarecfg.hitmarker_sound, hit_sound, ARRAYSIZE(hit_sound));
								ImGui::Separator();
								ImGui::Text("Nightmode");
								ImGui::Separator();
								if (ImGui::Button("Apply Nightmode")) {
									g_nightmode.night_mode();
								}
								imgui_custom::custom_color_inline(g_weebwarecfg.nightmode_col, "Nightmode Color");

								if (ImGui::Button("Fix FPS")) {
									c_convar* prop_var = g_weebware.g_convars->find_cvar("r_DrawSpecificStaticProp");
									prop_var->SetValue("-1");
								}
							}
							ImGui::EndChild();

						}
					}
#pragma endregion

#pragma region Misc
					if (selected_tab == tabs::misc)
					{
						ImGui::Checkbox("Enable", &g_weebwarecfg.enable_misc, false);

						ImGui::Columns(2, "Miscs", false);
						{
							ImGui::SetColumnOffset(1, 290);
							ImGui::BeginChild("Misc 1", ImVec2(0, 0), true);
							{
								ImGui::Text("Misc");
								ImGui::Separator();
								ImGui::Checkbox("Bunnyhop", &g_weebwarecfg.auto_jump, false);
								ImGui::Checkbox("Clantag Changer", &g_weebwarecfg.misc_clantag_changer, false);
								ImGui::Checkbox("Chatspam", &g_weebwarecfg.misc_chat_spammer, false);
								ImGui::Checkbox("Rank Reveal", &g_weebwarecfg.rank_reveal, false);
								ImGui::Checkbox("Thirdperson", &g_weebwarecfg.thirdperson, false);
								// ImGui::Checkbox("Auto accept", &g_weebwarecfg.misc_autoAccept, false);

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

								ImGui::Text("Legit Anti-Aim");
								ImGui::Separator();

								// ImGui::Checkbox("Anti Triggerbot", &g_weebwarecfg.anti_triggerbot, false);
								ImGui::Text("Anti Triggerbot");
								imgui_custom::custom_inline_keyinput(g_weebwarecfg.anti_triggerbot_key, key_counter);
								const char* activation[] = { "Off", "On Key" };
								ImGui::Combo("##backtrackingtype", &g_weebwarecfg.anti_triggerbot, activation, ARRAYSIZE(activation));

								ImGui::Checkbox("Enable AA", &g_weebwarecfg.misc_legit_aa_enabled, false);
								ImGui::Checkbox("Jitter", &g_weebwarecfg.misc_legit_aa_jitter, false);
								ImGui::Checkbox("Resolver", &g_weebwarecfg.misc_legit_aa_resolver, false);
								ImGui::Checkbox("Edge", &g_weebwarecfg.misc_legit_aa_edge, false);
								//	ImGui::Checkbox("Slide walk", &g_weebwarecfg.misc_slidewalk, false);


								ImGui::Separator();
							}
							ImGui::EndChild();

						}
					}
#pragma endregion

#pragma region Settings

					if (selected_tab == tabs::sets)
					{
						ImGui::Columns(2, "Settings", false);
						{
							ImGui::SetColumnOffset(1, 290);

							ImGui::BeginChild("configs", ImVec2(0, 0), true);
							{
								ImGui::Text("Configs");
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
								ImGui::Text("Load");
								ImGui::Separator();
								ImGui::BeginChild("Existing Configs", ImVec2(0, 355), false);
								for (auto cfg : g_config_list.config_names)
								{
									if (!strstr(cfg.c_str(), "weebwareskins")) {
										if (ImGui::Selectable(cfg.c_str(), g_config_list.cur_load_name == cfg.c_str()))
										{
											g_config_list.cur_load_name = cfg.c_str();
										}
									}
								}
								ImGui::EndChild();
								ImGui::Separator();
								ImGui::BeginChild("Config Loading", ImVec2(0, 22), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
								ImGui::Columns(3, "LoadSettings", false);
								ImGui::SetColumnOffset(1, 85);
								ImGui::SetColumnOffset(2, 180);
								if (ImGui::Button("Save", ImVec2(80, 20), ImGuiButtonFlags_Outlined))
								{
									g_config_list.save_existing_weebware();
								}
								ImGui::NextColumn();
								if (ImGui::Button("Load", ImVec2(80, 20), ImGuiButtonFlags_Outlined))
								{
									g_config_list.load_weebware_config();
								}
								ImGui::NextColumn();
								if (ImGui::Button("Delete", ImVec2(80, 20), ImGuiButtonFlags_Outlined))
								{
									g_config_list.delete_weebware_config();
								}
								ImGui::EndChild();
								ImGui::Separator();
								if (ImGui::Button("Unload", ImVec2(80, 20), ImGuiButtonFlags_Outlined))
								{
									g_hooking.unhook_all_functions();
								}
							}
							ImGui::EndChild();

							ImGui::NextColumn();

							ImGui::BeginChild("colors", ImVec2(0, 0), true);
							{
								ImGui::Text("Settings");
								ImGui::Separator();
							}
							ImGui::EndChild();

						}
					}


#pragma endregion

#pragma region Skins
					if (selected_tab == tabs::paint) {

						ImGui::Checkbox("Enabled", &g_weebwarecfg.skinchanger_enabled, false);

						ImGui::Columns(2, "SkinChanger", false);
						ImGui::SetColumnOffset(1, 290);


						ImGui::BeginChild("GunList", ImVec2(0, 0), true);
						ImGui::Text("Guns");
						ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 5);
						ImGui::InputText("##Search Gun", g_config_list.skinchanger_gunsearch, ARRAYSIZE(g_config_list.skinchanger_gunsearch));
						ImGui::PopItemWidth();

						auto gun_list = filtered_guns();

						ImGui::Separator();
						ImGui::BeginChild("Existing Guns", ImVec2(0, 240), false);
						// Enumerate skins at start of game and filter them out b4 drawing
						for (auto gun_part : gun_list)
						{
							if (ImGui::Selectable(gun_part.name.c_str(), g_weebwarecfg.selected_gun_index == gun_part.id))
							{
								g_weebwarecfg.skinchanger_selected_gun = convert_index_id(gun_part.id);
								g_weebwarecfg.selected_gun_index = gun_part.id;
							}
						}
						ImGui::EndChild();
						ImGui::Separator();

						ImGui::BeginChild("Existing Knives", ImVec2(0, 250), false);
						// Enumerate skins at start of game and filter them out b4 drawing

						try {
							int loopi = 0;

							for (auto knife_part : g_weebware.g_knife_list)
							{
								if (ImGui::Selectable(knife_part.weapon_name.c_str(), g_weebwarecfg.selected_knife_index[0] == knife_part.weapon_index))
								{
									g_weebwarecfg.selected_knife_index[0] = knife_part.weapon_index;
									g_weebwarecfg.selected_knife_index[1] = loopi;
								}
								++loopi;
							}
						}
						catch (...) {}

						ImGui::EndChild();

						ImGui::EndChild();


						ImGui::NextColumn();

						ImGui::BeginChild("SkinList", ImVec2(0, 0), true);
						ImGui::Text("Skins");
						ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 5);
						ImGui::InputText("##Search Skin", g_config_list.skinchanger_skinsearch, ARRAYSIZE(g_config_list.skinchanger_skinsearch));
						ImGui::PopItemWidth();

						auto skin_list = filtered_skins();

						ImGui::Separator();
						ImGui::BeginChild("Existing Skins", ImVec2(0, 340), false);
						// Enumerate skins at start of game and filter them out b4 drawing
						for (auto skin_part : skin_list)
						{
							std::string name = skin_part.name + "##" + std::to_string(skin_part.id);
							if (ImGui::Selectable(name.c_str(), g_weebwareskinscfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].m_paint_kit == skin_part.id))
							{
								g_weebwareskinscfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].m_paint_kit = skin_part.id;
							}
						}
						ImGui::EndChild();
						ImGui::Separator();

						ImGui::Text("Settings");
						ImGui::Separator();
						ImGui::InputInt("Seed", &g_weebwareskinscfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].m_seed);
						ImGui::InputFloat("Wear", &g_weebwareskinscfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun].m_wear);
						// ImGui::Text("Weapon name");
						// ImGui::InputText("##Gun Name", g_weebwareskinscfg.skin_wheel[g_weebwarecfg.skinchanger_selected_gun]., 32);
						if (ImGui::Button("Apply", ImVec2(ImGui::GetContentRegionAvailWidth(), 25), ImGuiButtonFlags_Outlined)) {

							g_weebwarecfg.skinchanger_apply_nxt = 1;

						}

						ImGui::EndChild();

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