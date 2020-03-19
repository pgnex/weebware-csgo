#pragma once
#include "Header.h"

namespace imgui_custom
{
	void custom_key_button(int& key, int& id);

	inline ImVec4 ConvertFromRGBA(ImVec4 imVec);

	inline ImVec4 ConvertToRGBA(ImVec4 imVec);

	void custom_color_options(ImVec4& col, const char* name);

	void create_tab(int& tab, int set, int pos, const char* title);
	void create_button_tab(int& tab, int set, const char* title, int width, int height);
	void horizontal_margin(std::string tag, int margin = 16);
	void fix_gay_padding_shit(std::string tag);
	void a_better_slider_float(const char* label, float* v, float v_min, float v_max, const char* format);
	void a_better_slider_int(const char* label, int* v, int v_min, int v_max, const char* format = "%d");
	void a_better_combo_box(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
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