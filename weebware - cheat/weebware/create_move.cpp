#include "Header.h"
#include "events.h"
#include "legit_aa.h"
#include "shared.h"
#include "create_move.h"
#include "Legit.h"
#include "Ragebot.h"
#include "hook_funcs.h"
#include <intrin.h>

c_create_move g_create_move;
c_nightmode g_nightmode;
c_utils g_utils;
LegitAntiAim g_LegitAntiAim;
c_legit g_legitbot;


bool hooks::hook_functions::clientmode_cm( float input_sample_time, c_usercmd* cmd, bool& sendpacket)
{
	//if (cmd->command_number == 0 && sendpacket) {
	//	return o_cm(g_weebware.g_client_mode, input_sample_time, cmd);
	//}

	//g_weebware.send_packet = true;

	if ( !cmd || !cmd->command_number ) {
		g_backtrack.clear_all_records( );
		return false;
	}		

	if ( !g_weebware.g_engine->is_connected( ) || !g_weebware.g_engine->is_in_game( ) ) {
		g_backtrack.clear_all_records( );
		return false;
	}

	g_create_move.local = g_weebware.g_entlist->getcliententity( g_weebware.g_engine->get_local( ) );
	Vector original_angles = cmd->viewangles;

	if ( !g_create_move.local ) {
		g_backtrack.clear_all_records( );
		return false;
	}

	g_create_move.skybox_changer( );
	g_create_move.auto_jump( cmd );
	g_create_move.auto_strafe( cmd );
	g_create_move.disable_post_processing( );
	g_create_move.draw_grenade_trajectory( );
	g_create_move.chat_spam( );
	g_create_move.rank_reveal( );
	g_create_move.slidewalk( cmd );
	g_create_move.auto_pistol( cmd );
	g_create_move.auto_jumpbug( cmd );
//	g_create_move.rainbow_name( );
	g_create_move.block_bot( cmd );
	g_create_move.auto_defuse( cmd );
	g_create_move.no_crouch_cooldown( cmd );
	g_create_move.anti_afk( cmd );
	g_nightmode.run( );

	g_create_move.fake_lag(cmd, sendpacket);
	// untrusted>??
//	g_LegitAntiAim.Run(cmd);


	g_create_move.edge_jump_pre_prediction( cmd );
	g_engine_prediction.begin( cmd, g_create_move.local );
	g_engine_prediction.end( g_create_move.local );
	g_create_move.edge_jump_post_prediction( cmd );


	if ( g_create_move.local->is_valid_player( ) && g_create_move.local->m_pActiveWeapon( ) ) {

		if ( g_weebwarecfg.skinchanger_apply_nxt ) {
			g_backtrack.clear_all_records( );
			if ( *g_weebware.g_client_state ) {
				knife_hook.force_update( );
			}
			g_backtrack.clear_all_records( );
			g_weebwarecfg.skinchanger_apply_nxt = 0;
		}
		else {
			g_create_move.local->m_pActiveWeapon( )->Update_Accuracy_Penalty( );

			// spooky stuff
			//if (!g_weebwarecfg.safemode) {
			//	ragebot::main(g_create_move.local, cmd);
			//}

			g_legitbot.run(cmd, g_create_move.local);
			g_backtrack.accuracy_boost( cmd );
			g_ai.create_move( cmd, g_create_move.local );
		}
	}

//	g_create_move.clamp_angles(cmd, original_angles, sendpacket);

	//g_create_move.real_angle = cmd->viewangles;
	//g_weebware.real_angle = cmd->viewangles;

	if (!g_weebware.send_packet)
		g_weebware.real_angle = cmd->viewangles;
	else
		g_weebware.fake_angle = cmd->viewangles;

	return false;
}


void load_named_sky( const char* sky_name ) {
	using Fn = void( __fastcall* )(const char*);
	static auto load_named_sky_fn = reinterpret_cast<Fn>(g_weebware.pattern_scan( ("engine.dll"), "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45" ));
	load_named_sky_fn( sky_name );
}



void c_create_move::skybox_changer( ) {

	c_convar* old_sky_name = g_weebware.g_convars->find_cvar( "sv_skyname" );

	if ( g_weebwarecfg.night_sky && !is_sky_set ) {
		load_named_sky( "sky_csgo_night02" );
		is_sky_set = true;
	}
	else if ( !g_weebwarecfg.night_sky && is_sky_set ) {
		load_named_sky( old_sky_name->strString );
		is_sky_set = false;
	}
}

auto flags_backup = 0;

void c_create_move::edge_jump_pre_prediction( c_usercmd* cmd ) {

	if ( !g_weebwarecfg.edge_jump )
		return;

	if ( !GetAsyncKeyState( g_weebwarecfg.edge_jump_key ) )
		return;

	if ( !local )
		return;

	if ( local->get_move_type( ) == MOVETYPE_LADDER || local->get_move_type( ) == MOVETYPE_NOCLIP )
		return;

	flags_backup = local->m_fFlags( );
}

void c_create_move::edge_jump_post_prediction( c_usercmd* cmd ) {

	if ( !g_weebwarecfg.edge_jump )
		return;

	if ( !GetAsyncKeyState( g_weebwarecfg.edge_jump_key ) )
		return;

	if ( !local )
		return;

	if ( local->get_move_type( ) == MOVETYPE_LADDER || local->get_move_type( ) == MOVETYPE_NOCLIP )
		return;

	if ( flags_backup & fl_onground && !(local->m_fFlags( ) & fl_onground) && !(cmd->buttons & in_jump) )
		cmd->buttons |= in_jump;

	if ( !(local->m_fFlags( ) & fl_onground) && g_weebwarecfg.duck_in_air )
		cmd->buttons |= in_duck;
}

void c_create_move::correct_movement( Vector old_view_angles, c_usercmd* cmd ) {
	Vector o_move( cmd->forwardmove, cmd->sidemove, cmd->upmove );
	float speed = o_move.size( ), yaw;
	QAngle wish_dir;
	g_maths.vector_qangles3d( o_move, wish_dir );
	yaw = DEG2RAD( cmd->viewangles.y - old_view_angles.y + wish_dir.y );
	cmd->forwardmove = cos( yaw ) * speed;
	cmd->sidemove = sin( yaw ) * speed;
	if ( cmd->viewangles.x < -90.f || cmd->viewangles.x > 90.f )
		cmd->forwardmove = -cmd->forwardmove;
}

bool post_process_disabled = false;
void c_create_move::disable_post_processing( ) {

	c_convar* mat_postprocess_enable = g_weebware.g_convars->find_cvar( "mat_postprocess_enable" );

	if ( g_weebwarecfg.disable_post_processing && !post_process_disabled ) {
		mat_postprocess_enable->SetValue( 0 );
		post_process_disabled = true;
	}
	else if ( !g_weebwarecfg.disable_post_processing && post_process_disabled ) {
		mat_postprocess_enable->SetValue( 1 );
		post_process_disabled = false;
	}

}

void c_create_move::anti_afk( c_usercmd* cmd ) {

	if ( !g_weebwarecfg.anti_afk )
		return;

	static bool updated = false;
	static int update_time = 0;

	if (!local->is_valid_player()) {
		updated = true;
		return;
	}


	if ( get_epoch_s( ) > update_time && updated ) {
		cmd->forwardmove = 30;
		updated = false;
		return;
	}

	if ( !updated ) {
		update_time = get_epoch_s( ) + 30;
		updated = true;
	}
}

void c_create_move::draw_grenade_trajectory( ) {

	c_convar* draw_grenade_trajectory = g_weebware.g_convars->find_cvar( "cl_grenadepreview" );

	if ( g_weebwarecfg.draw_grenade_traj && !grenade_traj_disabled ) {
		draw_grenade_trajectory->SetValue( 1 );
		grenade_traj_disabled = true;
	}
	else if ( !g_weebwarecfg.draw_grenade_traj && grenade_traj_disabled ) {
		draw_grenade_trajectory->SetValue( 0 );
		grenade_traj_disabled = false;
	}

}


namespace anti_trigger {

	/*Credits https://stackoverflow.com/questions/2752725/finding-whether-a-point-lies-inside-a-rectangle-or-not post by Alghyaline*/
	Vector vect2d( Vector p1, Vector p2 ) {
		Vector temp;
		temp.x = (p2.x - p1.x);
		temp.y = -1 * (p2.y - p1.y);

		return temp;
	}
	bool pointInRectangle( Vector A, Vector B, Vector C, Vector D, Vector m ) {
		Vector AB = vect2d( A, B );  float C1 = -1 * (AB.y*A.x + AB.x*A.y); float D1 = (AB.y*m.x + AB.x*m.y) + C1;
		Vector AD = vect2d( A, D );  float C2 = -1 * (AD.y*A.x + AD.x*A.y); float D2 = (AD.y*m.x + AD.x*m.y) + C2;
		Vector BC = vect2d( B, C );  float C3 = -1 * (BC.y*B.x + BC.x*B.y); float D3 = (BC.y*m.x + BC.x*m.y) + C3;
		Vector CD = vect2d( C, D );  float C4 = -1 * (CD.y*C.x + CD.x*C.y); float D4 = (CD.y*m.x + CD.x*m.y) + C4;
		return     0 >= D1 && 0 >= D4 && 0 <= D2 && 0 >= D3;
	}
	/*credits end stackoverflow*/
	bool Checkifbetween( Vector* ViewanglePoints, Vector Viewangles )
	{
		if ( pointInRectangle( ViewanglePoints[0], ViewanglePoints[1], ViewanglePoints[2], ViewanglePoints[3], Viewangles ) //first 4 normal
			 || pointInRectangle( ViewanglePoints[3], ViewanglePoints[2], ViewanglePoints[1], ViewanglePoints[0], Viewangles ) //first 4 reverse
			 || pointInRectangle( ViewanglePoints[4], ViewanglePoints[5], ViewanglePoints[6], ViewanglePoints[7], Viewangles ) //second 4 normal
			 || pointInRectangle( ViewanglePoints[7], ViewanglePoints[6], ViewanglePoints[5], ViewanglePoints[4], Viewangles ) )//second 4 reverse
		{
			return true;
		}
		return false;
	}

	bool require_fake = 0;

	void run_choke( bool& sendpacket, c_usercmd* cmd )
	{
		if ( !require_fake )
			return;

		static int ticks_choked = 0;

		if ( ticks_choked > 13 ) {
			sendpacket = true;
			ticks_choked = 0;
			require_fake = 0;
		}
		else {
			sendpacket = false;
		}

		ticks_choked++;
	}
}

void c_create_move::rank_reveal( )
{
	if ( !g_weebwarecfg.rank_reveal ) return;

	if ( GetAsyncKeyState( VK_TAB ) ) {
		using ServerRankRevealAll = char( __cdecl* )(int*);

		static uint8_t* fnServerRankRevealAll = 0;

		if ( !fnServerRankRevealAll ) {
			fnServerRankRevealAll = (uint8_t*)g_weebware.pattern_scan(("client.dll"), "55 8B EC 51 A1 ? ? ? ? 85 C0 75 37");
		}

		int v[3] = { 0,0,0 };

		reinterpret_cast<ServerRankRevealAll>(fnServerRankRevealAll)(v);
	}
}

void c_create_move::slidewalk( c_usercmd* cmd ) {

	if ( !g_weebwarecfg.misc_slidewalk )
		return;

	if ( local->get_move_type( ) == MOVETYPE_LADDER )
		return;

	if ( cmd->forwardmove > 0 )
	{
		cmd->buttons |= in_back;
		cmd->buttons &= ~in_forward;
	}

	if ( cmd->forwardmove < 0 )
	{
		cmd->buttons |= in_forward;
		cmd->buttons &= ~in_back;
	}

	if ( cmd->sidemove < 0 )
	{
		cmd->buttons |= in_moveright;
		cmd->buttons &= ~in_moveleft;
	}

	if ( cmd->sidemove > 0 )
	{
		cmd->buttons |= in_moveleft;
		cmd->buttons &= ~in_moveright;
	}
}

std::string color_codes[] = { "\x01", "\x02", "\x03", "\x04", "\x05", "\x06", "\x07", "\x08", "\x09", "\x0A", "\x0B", "\x0C", "\x0D", "\x0E", "\x0F", "\x10" };

std::string rainbowize( const std::string& s ) {
	if ( !s.size( ) ) {
		return "";
	}
	std::stringstream ss;
	ss << s[0];
	for ( uint32_t i = 1; i < s.size( ); i++ ) {
		ss << color_codes[rand( ) % 15] << s[i];
	}
	return ss.str( );
}

bool noname_done = false;
bool rainbow_done = false;
std::string o_name;

void c_create_move::rainbow_name( ) {

	if ( !g_weebwarecfg.rainbow_name && !rainbow_done && !noname_done )
		return;

	if ( !g_weebware.g_engine->is_connected( ) || !g_weebware.g_engine->is_in_game( ) ) {
		bool noname_done = false;
		bool rainbow_done = false;
		return;
	}

	s_player_info playerinfo;

	g_weebware.g_engine->get_player_info( local->EntIndex( ), &playerinfo );

	if ( o_name.empty( ) )
		o_name = playerinfo.name;

	if ( !noname_done ) {
		g_utils.set_name( "\n\xAD\xAD\xAD" );
		noname_done = true;
		return;
	}

	if ( !rainbow_done ) {
		std::string rainbow_name = rainbowize( o_name );

		g_utils.set_name( rainbow_name.c_str( ) );
		rainbow_done = true;
	}

	if ( rainbow_done && noname_done && !g_weebwarecfg.rainbow_name ) {
		g_utils.set_name( o_name.c_str( ) );

		rainbow_done = false;
		noname_done = false;
	}
}


void c_create_move::auto_jump( c_usercmd* cmd ) {
	if ( !g_weebwarecfg.auto_jump )
		return;

	if ( local->get_move_type( ) == MOVETYPE_LADDER )
		return;

	if ( g_weebwarecfg.auto_jump_hitchance < 100 ) {
		if ( (rand( ) % 99) + 1 > g_weebwarecfg.auto_jump_hitchance )
			return;
	}

	// if in air release flag
	if ( !(this->local->m_fFlags( ) & fl_onground) )
		cmd->buttons &= ~in_jump;
}

void c_create_move::auto_strafe( c_usercmd* cmd ) {

	if ( g_weebwarecfg.auto_strafe <= 0 )
		return;

	if ( this->local->m_fFlags( ) & fl_onground )
		return;

	int selection = g_weebwarecfg.auto_strafe;


	switch ( selection ) {
	case 0:
	break;
	case 1:
	if ( cmd->mousedx > 1 || cmd->mousedx < -1 )
		cmd->mousedx > 0 ? cmd->sidemove = 450 : cmd->sidemove = -450;
	break;
	case 2:
	if ( cmd->mousedx > 1 || cmd->mousedx < -1 )
		cmd->mousedx > 0 ? cmd->sidemove = 450 : cmd->sidemove = -450;
	else {
		cmd->forwardmove = 5850.f / this->local->m_vecVelocity( ).size( );
		cmd->sidemove = (cmd->command_number % 2) == 0 ? -450.f : 450.f;
	}
	break;
	default:
	break;
	}
}



// https://www.unknowncheats.me/forum/counterstrike-global-offensive/258333-antitrigger.html
// thanks master looser for these box tracing functions <3

bool c_create_move::anti_trigger( c_usercmd* cmd, bool& send_packets )
{

	//return false;

	//if ( !g_weebwarecfg.anti_triggerbot ) {
	//	return false;
	//}

	//if ( !GetAsyncKeyState( g_weebwarecfg.anti_triggerbot_key ) )
	//	return false;

	//auto org = *local->m_vecOrigin( );

	//org += (local->m_vecVelocity( ) / 6);

	//auto min = *local->m_vecMins( );
	//auto max = *local->m_vecMaxs( );

	//Vector Local_Bounding[8] = { //the comments below are not accurate
	//	Vector( org.x + min.x, org.y + min.y, org.z + min.z ), // left buttom
	//	Vector( org.x + min.x, org.y + min.y, org.z + max.z ), // left top
	//	Vector( org.x + max.x, org.y + max.y, org.z + max.z ), // right top
	//	Vector( org.x + max.x, org.y + max.y, org.z + min.z ),  // right buttom

	//	Vector( org.x + min.x, org.y + max.y, org.z + min.z ), // left buttom
	//	Vector( org.x + min.x, org.y + max.y, org.z + max.z ), // left top
	//	Vector( org.x + max.x, org.y + min.y, org.z + max.z ), // right top
	//	Vector( org.x + max.x, org.y + min.y, org.z + min.z )  // right buttom
	//};

	//for ( auto i = 1; i < 99; i++ ) {

	//	c_base_entity* cur_entity = (c_base_entity*)g_weebware.g_entlist->getcliententity( i );

	//	if ( !cur_entity )
	//		continue;

	//	if ( cur_entity->m_iHealth( ) <= 0 )
	//		continue;

	//	if ( cur_entity->m_iTeamNum( ) == local->m_iTeamNum( ) )
	//		continue;

	//	QAngle viewangle = *cur_entity->eyeangle_ptr( );

	//	Vector direction;

	//	g_maths.qangle_vector( viewangle, direction );
	//	direction *= 8012.f;

	//	Vector begin = cur_entity->get_vec_eyepos( );
	//	Vector finish = begin + direction;

	//	Vector angles[8];

	//	for ( auto i = 0; i < 8; i++ )
	//		g_maths.vector_qangles( Local_Bounding[i] - begin, angles[i] );

	//	if ( !anti_trigger::Checkifbetween( angles, viewangle ) )
	//		continue;

	//	trace_t trace;
	//	Ray_t ray;
	//	ITraceFilter filter;
	//	filter.pSkip = cur_entity;
	//	ray.Init( begin, finish );
	//	g_weebware.g_engine_trace->TraceRay( ray, 0x46004003, &filter, &trace );

	//	if ( ((trace.endpos - begin).size( ) + 10) >= (local->get_vec_eyepos( ) - begin).size( ) ) {

	//		anti_trigger::require_fake = true;

	//		break;
	//	}
	//}

	//return anti_trigger::require_fake;
	return false;
}

void c_create_move::run_fake( c_usercmd* cmd, bool& send_packet )
{

	return;

	anti_trigger::run_choke( send_packet, cmd );

	if ( anti_trigger( cmd, send_packet ) )
		return;

	if ( !g_weebwarecfg.misc_legit_aa_enabled )
		return;

	send_packet = (cmd->tick_count % 3 == 0);
}

bool can_shoot( c_base_entity* local )
{
	if ( !local )
		return false;

	if ( local->m_pActiveWeapon( )->m_flNextPrimaryAttack( ) > (float)g_weebware.g_global_vars->curtime )
		return false;
	return true;
}

c_base_entity* get_best_target( c_base_entity* local )
{
	float best_fov = 360.f;

	c_base_entity* best_entity = nullptr;

	for ( int i = 1; i <= g_weebware.g_engine->get_max_clients( ); i++ )
	{
		c_base_entity* cur_entity = (c_base_entity*)g_weebware.g_entlist->getcliententity( i );

		if ( !cur_entity )
			continue;

		if ( cur_entity->m_iHealth( ) <= 0 )
			continue;

		if ( cur_entity->m_iTeamNum( ) == local->m_iTeamNum( ) )
			continue;

		Vector center_head = cur_entity->get_bone( 8 );

		QAngle angle_to_head;

		g_maths.vector_qangles( center_head - local->get_vec_eyepos( ), angle_to_head );

		QAngle view_angles = QAngle( 0.f, 0.f, 0.f );

		g_weebware.g_engine->get_view_angles( view_angles );

		view_angles += local->m_aimPunchAngle( ) * 2.f;

		float this_fov = g_maths.get_fov( view_angles, angle_to_head );

		if ( this_fov < best_fov )
		{
			best_entity = cur_entity;
			best_fov = this_fov;
		}
	}
	return best_entity;
}

void edge_aa( Vector& edgeang, bool& willedge, c_base_entity* local )
{
	//ITraceFilter filter;
	//filter.pSkip = local;
	//Ray_t ray;
	//trace_t tr;
	//for ( int i = 0; i < 8; i++ )
	//{
	//	bool d2 = false;
	//	Vector End = local->get_vec_eyepos( );
	//	int value = 30;

	//	switch ( i )
	//	{
	//	case 0: End.y += value;
	//		edgeang.y = 90;
	//		break;
	//	case 1: End.y -= value;
	//		edgeang.y = -90;
	//		break;
	//	case 2: End.x += value;
	//		edgeang.y = 180;
	//		break;
	//	case 3: End.x -= value;
	//		edgeang.y = 0;
	//		break;
	//	case 4: End.y += value;
	//		End.x += value;
	//		edgeang.y = 30;
	//		break;
	//	case 5: End.y -= value;
	//		End.x += value;
	//		edgeang.y = -150;
	//		break;
	//	case 6: End.y += value;
	//		End.x -= value;
	//		edgeang.y = 150;
	//		d2 = true;
	//		break;
	//	case 7: End.y -= value;
	//		End.x -= value;
	//		edgeang.y = -30;
	//		break;
	//	}
	//	ray.Init( local->get_vec_eyepos( ), End );
	//	g_weebware.g_engine_trace->TraceRay( ray, MASK_SHOT, (ITraceFilter*)&filter, &tr );
	//	if ( tr.DidHit( ) )
	//	{
	//		QAngle angle_towall;
	//		g_maths.calc_angle( local->get_vec_eyepos( ), End, angle_towall );

	//		edgeang.y = angle_towall.y;

	//		if ( d2 )
	//			edgeang.y = 180;

	//		willedge = true;

	//		break;
	//	}
	//}

}


void c_create_move::no_crouch_cooldown( c_usercmd* cmd ) {

	if ( !g_weebwarecfg.no_duck_cooldown )
		return;

	cmd->buttons |= in_bullrush;
}

void c_create_move::auto_pistol( c_usercmd* cmd ) {

	if ( !g_weebwarecfg.auto_pistol )
		return;

	if ( !GetAsyncKeyState( g_weebwarecfg.auto_pistol_key ) )
		return;

	c_basecombat_weapon* weapon = local->m_pActiveWeapon( );

	if ( !weapon )
		return;

	if ( !weapon->is_pistol( ) )
		return;

	float flCurTime = local->get_tick_base( ) * g_weebware.g_global_vars->interval_per_tick;
	float flNextAttack = weapon->m_flNextPrimaryAttack( );
	if ( flNextAttack > flCurTime ) {
		cmd->buttons &= ~in_attack;
		cmd->buttons |= in_attack;
		cmd->buttons &= ~in_attack;
	}
}


void c_create_move::fake_lag( c_usercmd* cmd, bool send_packets ) {

	if ( g_weebwarecfg.fake_lag <= 0 ) {
		if ( g_weebwarecfg.fake_lag_fact > 0 )
			g_weebwarecfg.fake_lag_fact = 0;
		g_weebware.send_packet = true;
		return;
	}

	if ( !local->is_valid_player( ) ) {
		g_weebware.send_packet = true;
		return;
	}

	if ( local->m_iHealth( ) <= 0 ) {
		g_weebware.send_packet = true;
		return;
	}

	if ( g_weebwarecfg.fake_lag == 1 && !GetAsyncKeyState( g_weebwarecfg.fake_lag_key ) ) {
		g_weebware.send_packet = true;
		return;
	}

	if ( g_weebware.g_engine->is_voice_recording( ) ) {
		g_weebware.send_packet = true;
		return;
	}

	static int chokes = 0;

	if ( g_weebwarecfg.fake_lag_fact > 0 )
		g_weebware.send_packet = (chokes == 0) ? true : false;

	chokes++;

	if ( chokes > g_weebwarecfg.fake_lag_fact )
		chokes = 0;
}

void c_create_move::auto_defuse( c_usercmd* cmd ) {

	if ( !g_weebwarecfg.auto_defuse )
		return;

	if ( !GetAsyncKeyState( g_weebwarecfg.auto_defuse_key ) )
		return;

	if ( !local )
		return;

	if ( local->m_iTeamNum( ) != 3 )
		return;

	c_bomb* bomb = nullptr;

	for ( int i = 1; i < g_weebware.g_entlist->gethighestentityindex( ); i++ ) {
		c_base_entity* entity = g_weebware.g_entlist->getcliententity( i );

		if ( !entity )
			continue;

		if ( entity->get_client_class( )->m_ClassID == cplantedc4 ) {
			bomb = (c_bomb*)entity;
			break;
		}
	}

	if ( !bomb || bomb->is_bomb_defused( ) )
		return;

	float bomb_timer = bomb->get_blow_time( ) - g_weebware.g_global_vars->curtime;

	if ( local->m_bHasDefuser( ) && bomb_timer > 5.5f )
		return;

	if ( !local->m_bHasDefuser( ) && bomb_timer > 10.5f )
		return;

	float distance = local->m_vecOrigin( )->dist_to( bomb->get_vec_origin( ) );
	if ( distance <= 75.0f )
		cmd->buttons |= in_use;
}

void c_create_move::block_bot( c_usercmd* cmd ) {

	if ( !g_weebwarecfg.block_bot )
		return;

	if ( !GetAsyncKeyState( g_weebwarecfg.block_bot_key ) )
		return;

	float bestdist = 250.f;
	int index = -1;

	for ( int i = 1; i < g_weebware.g_engine->get_max_clients( ); i++ ) {
		c_base_entity* entity = (c_base_entity*)g_weebware.g_entlist->getcliententity( i );

		if ( !entity )
			continue;

		if ( !entity->is_valid_player( ) )
			continue;

		if ( entity == local )
			continue;

		float dist = local->m_vecOrigin( )->dist_to( *entity->m_vecOrigin( ) );

		if ( dist < bestdist ) {
			bestdist = dist;
			index = i;
		}
	}

	if ( index == -1 )
		return;

	c_base_entity* target = (c_base_entity*)g_weebware.g_entlist->getcliententity( index );

	if ( !target )
		return;
	
	QAngle angles;
	g_maths.calc_angle( *local->m_vecOrigin( ), *target->m_vecOrigin( ), angles );
	
	angles.y -= local->m_angEyeAngles( ).y;
	g_maths.normalize_angle( angles );
	
	if ( angles.y < 0.0f )
		cmd->sidemove = 450.f;
	else if ( angles.y > 0.0f )
		cmd->sidemove = -450.f;
}


void c_create_move::chat_spam( ) {

	if ( !g_weebwarecfg.misc_chat_spammer )
		return;

	static int elapsed_ticks = 0;

	int selection = int( static_cast<int>(cspam_weebware.size( ))* rand( ) / (RAND_MAX + 1.0) );

	elapsed_ticks++;

	// calculate server tickrate
	int tick_rate = 1 / g_weebware.g_global_vars->interval_per_tick;

	// after server refresh we should be able to execute another cmd
	if ( elapsed_ticks >= tick_rate ) {
		g_weebware.g_engine->execute_client_cmd( ("say " + cspam_weebware.at( selection )).c_str( ) );
		elapsed_ticks = 0;
	}
}

bool c_create_move::is_ground( ) {

	//Vector origin = *local->m_vecOrigin( );
	//origin += local->m_vecVelocity( ) * g_weebware.g_global_vars->interval_per_tick;

	//Ray_t ray;
	//ray.Init( origin, origin - Vector( 0, 0, 9 ) );

	//ITraceFilter traceFilter;

	//traceFilter.pSkip = (void*)local;

	//trace_t tr;

	//g_weebware.g_engine_trace->TraceRay( ray, MASK_PLAYERSOLID, &traceFilter, &tr );

	//if ( !tr.DidHit( ) )
	//	return false;


	//return true;
	return false;
}

void c_create_move::auto_jumpbug( c_usercmd* cmd ) {

	if ( !g_weebwarecfg.auto_jumpbug )
		return;

	if ( !GetAsyncKeyState( g_weebwarecfg.auto_jumpbug_key ) )
		return;

	if ( !(local->m_fFlags( ) & fl_onground) ) {
		cmd->buttons |= in_duck;
		if ( is_ground( ) ) {
			cmd->buttons &= ~in_duck;
			cmd->buttons |= in_jump;
		}
	}
}

void c_create_move::clamp_angles( c_usercmd* cmd, Vector original_angles, bool& sendpacket ) {
	//if ( cmd->buttons & in_attack )
	//	sendpacket = true;

	//QAngle cmd_view = cmd->viewangles;

	//g_maths.normalize_angle( cmd_view );
	//g_maths.clamp_angle( cmd_view );

	//cmd->viewangles = cmd_view;

	//g_create_move.correct_movement( original_angles, cmd );
	//g_maths.normalize_angle( cmd->viewangles );
	//g_maths.clamp_angle( cmd->viewangles );

	//if ( cmd->forwardmove > 450 )
	//	cmd->forwardmove = 450;

	//if ( cmd->forwardmove < -450 )
	//	cmd->forwardmove = -450;

	//if ( cmd->sidemove > 450 )
	//	cmd->sidemove = 450;

	//if ( cmd->sidemove < -450 )
	//	cmd->sidemove = -450;
}
