#pragma once

#include "Header.h"


namespace chams {

	enum custom_mats : int {
		disabled,
		plain,
		glow_cham = 2,
		flat,
		glass,
		crystal,
		gold,
		crystal_blue,
		max
	};

	namespace utils {
		imaterial* create_glow( );
		imaterial* borrow_mat( custom_mats type );
		imaterial* create_default( );
		imaterial* create_ignorez( );
		void init_key_vals( KeyValues* keyValues, char* name );
		void load_from_buf( KeyValues* keyValues, char const* resourceName, const char* pBuffer, class IBaseFileSystem* pFileSystem, const char* pPathID, void* pUnknown, void* uu );
	}


	namespace dme {
		void player_chams( void* thisptr, void* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld, hooks::drawmodelexecute o_dme );
		void hand_chams( const modelrenderinfo_t& pInfo );
	}

	namespace se {
		void player_chams( );
		void desync_chams( );
	}
}