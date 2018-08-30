#pragma once

#include "net_channel.h"
#include "vector.h"
#include "player_info.h"
#include "key_values.h"
#include "virtual_function.h"

enum clientframestage_t
{
	frame_undefined = -1, // (haven't run any frames yet)
	frame_start,

	// a network packet is being recieved
	frame_net_update_start,
	// data has been received and we're going to start calling postdataupdate
	frame_net_update_postdataupdate_start,
	// data has been received and we've called postdataupdate on all data recipients
	frame_net_update_postdataupdate_end,
	// we've received all packets, we can now do interpolation, prediction, etc..
	frame_net_update_end,

	// we're about to start rendering the scene
	frame_render_start,
	// we've finished rendering the scene.
	frame_render_end
};


class c_usercmd
{
public:
	c_usercmd()
	{
		memset(this, 0, sizeof(*this));
	};
	virtual ~c_usercmd() {};

	unsigned long GetChecksum(void) const
	{
		return 0;
	}

	int     command_number;     // 0x04 For matching server and client commands for debugging
	int     tick_count;         // 0x08 the tick the client created this command
	QAngle  viewangles;         // 0x0C Player instantaneous view angles.
	QAngle  aimdirection;       // 0x18
	float   forwardmove;        // 0x24
	float   sidemove;           // 0x28
	float   upmove;             // 0x2C
	int     buttons;            // 0x30 Attack button states
	char    impulse;            // 0x34
	int     weaponselect;       // 0x38 Current weapon id
	int     weaponsubtype;      // 0x3C
	int     random_seed;        // 0x40 For shared random functions
	short   mousedx;            // 0x44 mouse accum in x from create move
	short   mousedy;            // 0x46 mouse accum in y from create move
	bool    hasbeenpredicted;   // 0x48 Client only, tracks whether we've predicted this command at least once
	char    pad_0x4C[0x18];     // 0x4C Current sizeof( usercmd ) =  100  = 0x64
};

struct player_info
{
private:
	int64_t unknown;
public:
	union
	{
		int64_t xuid;

		struct
		{
			int xuid_low;
			int xuid_high;
		};
	};

	char name[128];
	int userid;
	int m_nUserID;
	char guid[32 + 1];
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};


class c_engine_client
{
public:
	virtual void vfunc00() = 0;
	virtual void vfunc01() = 0;
	virtual void vfunc02() = 0;
	virtual void vfunc03() = 0;
	virtual void vfunc04() = 0;
	virtual	void get_screen_dimensions(int& width, int& height) = 0;
	virtual void vfunc06() = 0;
	virtual void vfunc07() = 0;
	virtual bool GetPlayerInfo(int ent_num, player_info* pinfo) = 0;
	virtual int GetPlayerForUserID(int userID) = 0;
	virtual void vfunc10() = 0;
	virtual void vfunc11() = 0;
	virtual void vfunc12() = 0;
	virtual void vfunc13() = 0;
	virtual void vfunc14() = 0;
	virtual void vfunc15() = 0;
	virtual void vfunc16() = 0;
	virtual void vfunc17() = 0;
	virtual void vfunc18() = 0;
	virtual void vfunc19() = 0;
	virtual void vfunc20() = 0;
	virtual void vfunc21() = 0;
	virtual void vfunc22() = 0;
	virtual void vfunc23() = 0;
	virtual void vfunc24() = 0;
	virtual void vfunc25() = 0;
	virtual void vfunc26() = 0;
	virtual void vfunc27() = 0;
	virtual void vfunc28() = 0;
	virtual void vfunc29() = 0;
	virtual void vfunc30() = 0;
	virtual void Con_NXPrintf(const struct con_nprint_s* info, const char* fmt, ...) = 0;


	unsigned int get_engine_build()
	{
		return getvfunc<unsigned int(__thiscall*)(PVOID)>(this, 104)(this);
	}

	bool get_player_info(int iIndex, s_player_info* pInfo)
	{
		return getvfunc<bool(__thiscall*)(PVOID, int, s_player_info*)>(this, 8)(this, iIndex, pInfo);
	}

	void client_cmd_unrestricted(const char* szCmdString, char flag)
	{
		getvfunc<void(__thiscall*)(PVOID, const char*, char)>(this, 114)(this, szCmdString, flag);
	}

	int get_local()
	{
		return getvfunc<int(__thiscall*)(PVOID)>(this, 12)(this);
	}
	int get_version()
	{
		return getvfunc<int(__thiscall*)(PVOID)>(this, 236)(this);
	}
	c_netchannel_info* get_net_channel(void)
	{
		return getvfunc<c_netchannel_info*(__thiscall*)(PVOID)>(this, 78)(this);
	}

	matrix3x4& w2s_matrix(void)
	{
		return getvfunc<matrix3x4& (__thiscall*)(PVOID)>(this, 37)(this);
	}

	void execute_client_cmd(char const* command)
	{
		getvfunc<void(__thiscall*)(PVOID, char const*)>(this, 105)(this, command);
	}

	void get_view_angles(Vector& ang)
	{
		return getvfunc<void(__thiscall*)(PVOID, Vector&)>(this, 18)(this, ang);
	}

	void set_view_angles(QAngle& ang)
	{
		return getvfunc<void(__thiscall*)(PVOID, QAngle&)>(this, 19)(this, ang);
	}

	bool is_connected()
	{
		return getvfunc<bool(__thiscall*)(PVOID)>(this, 27)(this);
	}

	bool is_in_game()
	{
		return getvfunc<bool(__thiscall*)(PVOID)>(this, 26)(this);
	}

	int get_max_clients()
	{
		return getvfunc<bool(__thiscall*)(PVOID)>(this, 20)(this);
	}

	const char* get_level_name()
	{
		return getvfunc<const char*(__thiscall*)(PVOID)>(this, 52)(this);
	}

	const char* get_level_name_short()
	{
		return getvfunc<const char*(__thiscall*)(PVOID)>(this, 53)(this);
	}

};

//player flags
#define	fl_onground				(1<<0)	// at rest / on the ground
#define fl_ducking				(1<<1)	// player flag -- player is fully crouched
#define	fl_waterjump			(1<<3)	// player jumping out of water
#define fl_ontrain				(1<<4) // player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define fl_inrain				(1<<5)	// indicates the entity is standing in rain
#define fl_frozen				(1<<6) // player is frozen for 3rd person camera
#define fl_atcontrols			(1<<7) // player can't move, but keeps key inputs for controlling another entity
#define	fl_client				(1<<8)	// is a player
#define fl_fakeclient			(1<<9)	// fake client, simulated server side; don't send network messages to them
#define	fl_inwater				(1<<10)	// in water


//usercmd buttons
#define in_attack		(1 << 0)
#define in_jump			(1 << 1)
#define in_duck			(1 << 2)
#define in_forward		(1 << 3)
#define in_back			(1 << 4)
#define in_use			(1 << 5)
#define in_cancel		(1 << 6)
#define in_left			(1 << 7)
#define in_right		(1 << 8)
#define in_moveleft		(1 << 9)
#define in_moveright	(1 << 10)
#define in_attack2		(1 << 11)
#define in_run			(1 << 12)
#define in_reload		(1 << 13)
#define in_alt1			(1 << 14)
#define in_alt2			(1 << 15)
#define in_score		(1 << 16)   // used by client.dll for when scoreboard is held down
#define in_speed		(1 << 17)	// player is holding the speed key
#define in_walk			(1 << 18)	// player holding walk key
#define in_zoom			(1 << 19)	// zoom key for hud zoom
#define in_weapon1		(1 << 20)	// weapon defines these bits
#define in_weapon2		(1 << 21)	// weapon defines these bits
#define in_bullrush		(1 << 22)
#define in_grenade1		(1 << 23)	// grenade 1
#define in_grenade2		(1 << 24)	// grenade 2

class c_input_system {
public:
	void GetCursorPosition(int* m_pX, int* m_pY)
	{
		return getvfunc<void(__thiscall*)(void*, int*, int*) >(this, 56)(this, m_pX, m_pY);
	}

	void EnableInput(bool bEnable)
	{
		return getvfunc<void(__thiscall*)(void*, bool)>(this, 11)(this, bEnable);
	}
};

class c_convar
{
public:
	const char* GetName()
	{
		return getvfunc<char*(__thiscall*)(void*)>(this, 5)(this);
	}
	const char* GetBaseName()
	{
		return getvfunc<char*(__thiscall*)(void*)>(this, 6)(this);
	}
	void SetValue(const char* value)
	{
		return getvfunc<void(__thiscall*)(void*, const char*)>(this, 14)(this, value);
	}

	void SetValue(float value)
	{
		return getvfunc< void(__thiscall*)(void*, float)>(this, 15)(this, value);
	}
	void SetValue(int value)
	{
		return getvfunc<void(__thiscall*)(void*, int)>(this, 16)(this, value);
	}

	char pad_0x0000[0x4];
	c_convar* pNext;
	int bRegistered;
	char* pszName;
	char* pszHelpString;
	int nFlags;
	char pad_0x0018[0x4];
	c_convar* pParent;
	char* pszDefaultValue;
	char* strString;
	int StringLength;
	float fValue;
	int nValue;
	int bHasMin;
	float fMinVal;
	int bHasMax;
	float fMaxVal;
	void* fnChangeCallback;

	float GetFloat(void) const
	{
		auto temp = *(int*)(&fValue);
		auto temp_result = (int)(temp ^ (DWORD)this);
		return *(float*)(&temp_result);
	}
};


class c_iconvar
{
public:
	c_convar* find_cvar(const char* var_name)
	{
		return getvfunc<c_convar*(__thiscall*)(void*, char const*)>(this, 15)(this, var_name);
	}
};


class i_effects
{
public:
	virtual void unknown1() = 0;
	//
	// Particle effects
	// Beam is also terminated.
	virtual void Beam(const Vector &Start, const Vector &End, int nModelIndex,
		int nHaloIndex, unsigned char frameStart, unsigned char frameRate,
		float flLife, unsigned char width, unsigned char endWidth, unsigned char fadeLength,
		unsigned char noise, unsigned char red, unsigned char green,
		unsigned char blue, unsigned char brightness, unsigned char speed) = 0;

	//-----------------------------------------------------------------------------
	// Purpose: Emits smoke sprites.
	// Input  : origin - Where to emit the sprites.
	//			scale - Sprite scale * 10.
	//			framerate - Framerate at which to animate the smoke sprites.
	//-----------------------------------------------------------------------------
	virtual void Smoke(const Vector &origin, int modelIndex, float scale, float framerate) = 0;

	virtual void Sparks(const Vector &position, int nMagnitude = 1, int nTrailLength = 1, const Vector *pvecDir = NULL) = 0;

	virtual void Dust(const Vector &pos, const Vector &dir, float size, float speed) = 0;

	virtual void MuzzleFlash(const Vector &vecOrigin, const QAngle &vecAngles, float flScale, int iType) = 0;

	// like ricochet, but no sound
	virtual void MetalSparks(const Vector &position, const Vector &direction) = 0;

	virtual void EnergySplash(const Vector &position, const Vector &direction, bool bExplosive = false) = 0;

	virtual void Ricochet(const Vector &position, const Vector &direction) = 0;

	// FIXME: Should these methods remain in this interface? Or go in some 
	// other client-server neutral interface?
	virtual float Time() = 0;
	virtual bool IsServer() = 0;

	// Used by the playback system to suppress sounds
	virtual void SuppressEffectsSounds(bool bSuppress) = 0;
};