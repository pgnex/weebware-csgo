#include "Header.h"
#include "shared.h"
#include "hook_funcs.h"
#include "chams.h"
#include <iostream>

void hooks::hook_functions::draw_model_execute( void* thisptr, void* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld, hooks::drawmodelexecute o_dme ) {

	if ( !ctx )
		return;

	// if its not a player or arms nothing needed
	const char* model_name = g_weebware.g_model_info->getmodelname( pInfo.pModel );
	if ( !( strstr( model_name, "models/player" ) ) && !( strstr( model_name, "arms" ) ) )
		return;

	if ( strstr( model_name, "models/player" ) )
		chams::dme::player_chams( thisptr, ctx, state, pInfo, pCustomBoneToWorld, o_dme );

	if ( strstr( model_name, "arms" ) )
		chams::dme::hand_chams( pInfo );

}

void chams::utils::init_key_vals( KeyValues* keyValues, char* name ) {
	static DWORD keyval_addr = 0;

	if ( !keyval_addr )keyval_addr = g_weebware.pattern_scan( "client.dll", "68 ?? ?? ?? ?? 8B C8 E8 ?? ?? ?? ?? 89 45 FC EB 07 C7 45 ?? ?? ?? ?? ?? 8B 03 56*" ) + 7;

	static DWORD dwFunction = 0;

	if ( !dwFunction ) dwFunction = keyval_addr + *reinterpret_cast< PDWORD_PTR >( keyval_addr + 1 ) + 5;

	if ( !dwFunction ) {
		return;
	}
	__asm
	{
		push name
		mov ecx, keyValues
		call dwFunction
	}
}

void chams::utils::load_from_buf( KeyValues* keyValues, char const* resourceName, const char* pBuffer, class IBaseFileSystem* pFileSystem, const char* pPathID, void* pUnknown, void* uu ) {
	static  DWORD dwFunction = 0;

	if ( !dwFunction ) dwFunction = g_weebware.pattern_scan( "client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89" );

	if ( !dwFunction ) {
		return;
	}

	__asm
	{
		push uu
		push pUnknown
		push pPathID
		push pFileSystem
		push pBuffer
		push resourceName
		mov ecx, keyValues
		call dwFunction
	}
}

imaterial* chams::utils::borrow_mat( custom_mats type ) {
	const char* material_list[] = { "", "", "", "debug/debugdrawflat", "models/inventory_items/cologne_prediction/cologne_prediction_glass", "models/inventory_items/trophy_majors/crystal_clear", "models/inventory_items/trophy_majors/gold", "models/inventory_items/trophy_majors/crystal_blue" };

	// TEXTURE_GROUP_MODEL : TEXTURE_GROUP_OTHER
	imaterial* mat = g_weebware.g_mat_sys->find_material( material_list[type], TEXTURE_GROUP_MODEL );
	if ( !mat || mat->iserrormaterial( ) )
		return g_weebware.g_mat_sys->find_material( "models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_MODEL );

	mat->incrementreferencecount( );
	return mat;
}

imaterial* chams::utils::create_default( ) {


	std::stringstream s;

	s << "\"VertexLitGeneric\" {\n\n\t";
	s << "\"basetexture\" \"vgui/white_additive\"\n\t";
	s << "\"$additive\" \"0\"\n\t";
	//s << "\"$envmap\" \"models/effects/cube_white\"\n\t";
	//s << "\"$envmaptint\"" << " \"[" << 1 << " " << 1 << " " << 1 << "]\"\n\t";
	s << "\"$ignorez\" \"0\"\n\t";
	//s << "\"$envmapfresnelminmaxexp\" \"[0 1 2]\"\n\t";
	s << "\"$alpha\" \"1\"\n";
	s << "}";


	KeyValues* keyValues = ( KeyValues* )malloc( sizeof( KeyValues ) );
	init_key_vals( keyValues, "VertexLitGeneric" );
	load_from_buf( keyValues, "default.vmt", s.str( ).c_str( ), 0, 0, 0, 0 );
	imaterial* created_mat = g_weebware.g_mat_sys->create_mat( "default.vmt", keyValues );

	if ( !created_mat || created_mat->iserrormaterial( ) )
		return NULL;

	return created_mat;
}

imaterial* chams::utils::create_ignorez( ) {


	std::stringstream s;

	s << "\"VertexLitGeneric\" {\n\n\t";
	s << "\"basetexture\" \"vgui/white_additive\"\n\t";
	s << "\"$additive\" \"0\"\n\t";
	//s << "\"$envmap\" \"models/effects/cube_white\"\n\t";
	//s << "\"$envmaptint\"" << " \"[" << 1 << " " << 1 << " " << 1 << "]\"\n\t";
	s << "\"$ignorez\" \"1\"\n\t";
	//s << "\"$envmapfresnelminmaxexp\" \"[0 1 2]\"\n\t";
	s << "\"$alpha\" \"1\"\n";
	s << "}";


	KeyValues* keyValues = ( KeyValues* )malloc( sizeof( KeyValues ) );
	init_key_vals( keyValues, "VertexLitGeneric" );
	load_from_buf( keyValues, "ignorez.vmt", s.str( ).c_str( ), 0, 0, 0, 0 );
	imaterial* created_mat = g_weebware.g_mat_sys->create_mat( "ignorez.vmt", keyValues );

	if ( !created_mat || created_mat->iserrormaterial( ) )
		return NULL;

	return created_mat;
}

imaterial* chams::utils::create_glow( ) {

	std::stringstream s;

	s << "\"VertexLitGeneric\" {\n\n\t";
	s << "\"basetexture\" \"vgui/white\"\n\t";
	s << "\"$additive\" \"0\"\n\t";
	s << "\"$envmap\" \"models/effects/cube_white\"\n\t";
	s << "\"$envmaptint\"" << " \"[" << 1 << " " << 1 << " " << 1 << "]\"\n\t";
	s << "\"$envmapfresnel\" \"1\"\n\t";
	s << "\"$envmapfresnelminmaxexp\" \"[0 1 2]\"\n\t";
	s << "\"$alpha\" \"0.8\"\n";
	s << "}";


	KeyValues* keyValues = ( KeyValues* )malloc( sizeof( KeyValues ) );
	init_key_vals( keyValues, "VertexLitGeneric" );
	load_from_buf( keyValues, "glow.vmt", s.str( ).c_str( ), 0, 0, 0, 0 );
	imaterial* created_mat = g_weebware.g_mat_sys->create_mat( "glow.vmt", keyValues );

	return created_mat;
}

void chams::dme::player_chams( void* thisptr, void* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld, hooks::drawmodelexecute o_dme ) {

	if ( !g_weebwarecfg.visuals_chams > 0 )
		return;

	// get local
	c_base_entity* local = g_weebware.g_entlist->getcliententity( g_weebware.g_engine->get_local( ) );
	if ( !local )
		return;

	c_base_entity* model_ent = g_weebware.g_entlist->getcliententity( pInfo.entity_index );
	if ( !model_ent || !model_ent->is_valid_player( ) )
		return;

	if ( ( local->m_iTeamNum( ) == model_ent->m_iTeamNum( ) ) && !g_weebwarecfg.visuals_chams_render_team )
		return;

	// setting up mats
	static bool init = false;
	static imaterial* mat_list[custom_mats::max];
	if ( !init ) {
		// grab pre-generated materials
		for ( auto i = 1; i < custom_mats::max; i++ ) {
			mat_list[i] = utils::borrow_mat( static_cast< custom_mats >( i ) );
		}
		mat_list[custom_mats::plain] = utils::create_default( );
		mat_list[custom_mats::glow_cham] = utils::create_glow( );
		init = true;
	}

	// last null check,,
	if ( !mat_list[g_weebwarecfg.visuals_chams] )
		return;

	if ( mat_list[g_weebwarecfg.visuals_chams]->iserrormaterial( ) )
		return;

	// setup colors
	c_color col, xqz_col;
	col = c_color( model_ent->m_iTeamNum( ) == local->m_iTeamNum( ) ? g_weebwarecfg.visuals_chams_team_col : g_weebwarecfg.visuals_chams_col );
	xqz_col = c_color( model_ent->m_iTeamNum( ) == local->m_iTeamNum( ) ? g_weebwarecfg.visuals_chams_team_col_xqz : g_weebwarecfg.visuals_chams_col_xqz );

	float col_blend[4] = { col.r / 255.f, col.g / 255.f, col.b / 255.f, 1.f };
	float xqz_col_blend[4] = { xqz_col.r / 255.f, xqz_col.g / 255.f, xqz_col.b / 255.f, 1.f };

	// do chams
	switch ( g_weebwarecfg.visuals_chams ) {
	case custom_mats::plain: {
		if ( g_weebwarecfg.visuals_chams_xqz ) {
			static imaterial* plain_ignorez = utils::create_ignorez( );
			g_weebware.g_render_view->SetBlend( xqz_col.a / 255.f );
			g_weebware.g_render_view->SetColorModulation( xqz_col_blend );
			g_weebware.g_model_render->forcedmaterialoverride( plain_ignorez );
			// orig..
			o_dme( g_weebware.g_model_render, ctx, state, pInfo, pCustomBoneToWorld );
		}
		g_weebware.g_render_view->SetBlend( col.a / 255.f );
		g_weebware.g_render_view->SetColorModulation( col_blend );
		g_weebware.g_model_render->forcedmaterialoverride( mat_list[custom_mats::plain] );
		return;
	}
	case custom_mats::glow_cham: {
		c_color glow_clr = g_weebwarecfg.visuals_chams_glow_col;

		g_weebware.g_render_view->SetBlend( col.a / 255.f );
		g_weebware.g_render_view->SetColorModulation( col_blend );
		mat_list[custom_mats::plain]->setmaterialvarflag( material_var_ignorez, false );
		g_weebware.g_model_render->forcedmaterialoverride( mat_list[custom_mats::plain] );

		bool found = false;
		imaterialvar* pVar = mat_list[g_weebwarecfg.visuals_chams]->FindVar( "$envmaptint", &found );
		if ( found )
			( *( void( __thiscall** )( int, float, float, float ) )( *( DWORD* )pVar + 44 ) )( ( uintptr_t )pVar, glow_clr.r / 255.f, glow_clr.g / 255.f, glow_clr.b / 255.f );

		g_weebware.g_render_view->SetBlend( xqz_col.a / 255.f );
		g_weebware.g_render_view->SetColorModulation( xqz_col_blend );
		mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag( material_var_ignorez, false );
		g_weebware.g_model_render->forcedmaterialoverride( mat_list[g_weebwarecfg.visuals_chams] );
		return;
	}
	default: {
		if ( g_weebwarecfg.visuals_chams_xqz ) {
			g_weebware.g_render_view->SetBlend( xqz_col.a / 255.f );
			g_weebware.g_render_view->SetColorModulation( xqz_col_blend );
			mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag( material_var_ignorez, true );
			g_weebware.g_model_render->forcedmaterialoverride( mat_list[g_weebwarecfg.visuals_chams] );
			// orig..
			o_dme( g_weebware.g_model_render, ctx, state, pInfo, pCustomBoneToWorld );
		}
		g_weebware.g_render_view->SetBlend( col.a / 255.f );
		g_weebware.g_render_view->SetColorModulation( col_blend );
		mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag( material_var_ignorez, false );
		g_weebware.g_model_render->forcedmaterialoverride( mat_list[g_weebwarecfg.visuals_chams] );
		return;
	}
	}
}


void chams::dme::hand_chams( const modelrenderinfo_t& pInfo ) {

}