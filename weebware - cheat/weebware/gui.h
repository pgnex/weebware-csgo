#pragma once

#include "imgui_custom.h"
#include "imgui\imgui_impl_win32.h"

using namespace imgui_custom;

class gui {
public:

	enum tabs {
		legit,
		//rage,
		vis,
		misc,
		skins,
		settings
	};

	void imgui_main();
	void shoutbox_chat();
	void weapon_override();

	void render_menu(IDirect3DDevice9* device)
	{
		static bool initialized = false;

		if (!initialized) {
			ImGui::CreateContext();
			ImGui_ImplWin32_Init(g_weebware.h_window);
			ImGui_ImplDX9_Init(device);
			style_setup();
			initialized = true;
		}

		//	renderstate_setup(device);
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		imgui_main();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	void renderstate_setup(IDirect3DDevice9* device)
	{
		device->SetRenderState(D3DRS_LIGHTING, FALSE);
		device->SetRenderState(D3DRS_FOGENABLE, FALSE);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
		device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

		device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	}

	void style_setup()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowPadding = ImVec2(0, 0);
		style->WindowRounding = 2.f;
		style->FramePadding = ImVec2(2, 1.5f);
		style->TextPaddingY = 1;
		style->FrameRounding = 0.f;
		style->ItemSpacing = ImVec2(6, 6);
		style->IndentSpacing = 2.0f;
		style->ScrollbarSize = 8.f;
		style->ScrollbarRounding = 0.f;
		style->GrabMinSize = 5.0f;
		style->ItemInnerSpacing = ImVec2(4, 5);
		style->ChildRounding = 6.f;

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
		style->Colors[ImGuiCol_TextSelectedBg] = imgui_custom::ConvertFromRGBA(ImVec4(52, 52, 52, 255));
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

		ImFontConfig config;
		config.MergeMode = true;
		g_weebware.pFont[0] = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(raleway_compressed_data, raleway_compressed_size, 14);
		g_weebware.pFont[1] = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(raleway_compressed_data, raleway_compressed_size, 24);
		g_weebware.pFont[2] = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(weebware_icon_font_data, weebware_icon_font_size, 36);
	}

	std::string erase_sub_string(std::string mainStr, std::string toErase) {

		size_t pos = mainStr.find(toErase);

		if (pos != std::string::npos)
			mainStr.erase(pos, toErase.length());

		return mainStr;
	}

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




};
extern gui g_gui;
