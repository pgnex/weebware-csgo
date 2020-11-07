#pragma once
#include "Header.h"

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