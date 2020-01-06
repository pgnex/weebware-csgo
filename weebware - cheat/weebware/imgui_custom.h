#pragma once
#include "Header.h"

namespace imgui_custom
{
	void custom_key_button(int& key);

	inline ImVec4 ConvertFromRGBA(ImVec4 imVec);

	inline ImVec4 ConvertToRGBA(ImVec4 imVec);

	void custom_color_options(ImVec4& col, const char* name);

	void create_tab(int& tab, int set, int pos, const char* title);
	void create_button_tab(int& tab, int set, const char* title, int width, int height);
	void horizontal_margin(std::string tag, int margin = 20);
	void fix_gay_padding_shit(std::string tag);
	enum tab_pos
	{
		first,
		middle,
		last
	};

	void custom_color_inline(ImVec4& col, const char* id);
	void custom_color_inline(ImVec4& col, ImVec4& col2, bool should_draw_second, const char* id, const char* id2);
	void custom_inline_keyinput(int& key, int& id);
	void custom_label_header(std::string text);
}