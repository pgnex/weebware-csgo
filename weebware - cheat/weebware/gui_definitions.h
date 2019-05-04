#pragma once

enum input_type : char
{
	inp_click_down,
	inp_click_up,
	inp_keydown,
	inp_keyup,
	inp_mouse_move
};

enum input_messages : bool
{
	inpm_processed = 0,
	inpm_continue
};

typedef int* input_variable;

typedef float deltatime_t;

struct color_t
{
	
};

struct theme_t
{
	color_t m_ground;
	color_t m_groundaccent;

	color_t m_figure;
	color_t m_figureaccent;
};

enum object_flags
{
	object_parent = 0x01,
};