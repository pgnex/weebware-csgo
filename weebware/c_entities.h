#pragma once
#include "c_client.h"
#include "maths.h"
#include "Header.h"
#include <array>

#define retrieve_offset(table, ...)netvar_manager::_instance()->iget_offset(table, __VA_ARGS__)

class bf_read;
struct model_t;
struct mstudiobbox_t;
struct mstudiohitboxset_t;
struct studiohdr_t;

class i_handle_ent
{
public:
	virtual ~i_handle_ent()
	{
	}

	virtual void setrefehandle(const HANDLE& handle) = 0;
	virtual const HANDLE& getrefehandle() const = 0;
};

class i_client_unknown : public i_handle_ent
{
public:
	virtual void* getcollideable() = 0;
	virtual void* getclientnetworkable() = 0;
	virtual void* getclientrenderable() = 0;
	virtual void* geticliententity() = 0;
	virtual void* getbaseentity() = 0;
	virtual void* getclientthinkable() = 0;
	virtual void* getclientalphaproperty() = 0;
};

class iclientrenderable
{
public:
	virtual i_client_unknown* geticlientunknown() = 0;
	virtual Vector const& getrenderorigin(void) = 0;
	virtual Vector const& getrenderangles(void) = 0;
	virtual bool shoulddraw(void) = 0;
	virtual int getrenderflags(void) = 0; // erenderflags_xxx
	virtual void unused(void) const
	{
	}

	virtual HANDLE getshadowhandle() const = 0;
	virtual HANDLE& renderhandle() = 0;
	virtual const model_t* getmodel() const = 0;
	virtual int drawmodel(int flags, const int /*renderableinstance_t*/ & instance) = 0;
	virtual int getbody() = 0;
	virtual void getcolormodulation(float* color) = 0;
	virtual bool lodtest() = 0;
	virtual bool setupbones(matrix3x4* pbonetoworldout, int nmaxbones, int bonemask, float currenttime) = 0;
	virtual void setupweights(const matrix3x4* pbonetoworld, int nflexweightcount, float* pflexweights, float* pflexdelayedweights) = 0;
	virtual void doanimationevents(void) = 0;
	virtual void* /*ipvsnotify*/ getpvsnotifyinterface() = 0;
	virtual void getrenderbounds(Vector& mins, Vector& maxs) = 0;
	virtual void getrenderboundsworldspace(Vector& mins, Vector& maxs) = 0;
	virtual void getshadowrenderbounds(Vector& mins, Vector& maxs, int /*shadowtype_t*/ shadowtype) = 0;
	virtual bool shouldreceiveprojectedtextures(int flags) = 0;
	virtual bool getshadowcastdistance(float* pdist, int /*shadowtype_t*/ shadowtype) const = 0;
	virtual bool getshadowcastdirection(Vector* pdirection, int /*shadowtype_t*/ shadowtype) const = 0;
	virtual bool isshadowdirty() = 0;
	virtual void markshadowdirty(bool bdirty) = 0;
	virtual iclientrenderable* getshadowparent() = 0;
	virtual iclientrenderable* firstshadowchild() = 0;
	virtual iclientrenderable* nextshadowpeer() = 0;
	virtual int /*shadowtype_t*/ shadowcasttype() = 0;
	virtual void createmodelinstance() = 0;
	virtual HANDLE getmodelinstance() = 0;
	virtual const matrix3x4& renderabletoworldtransform() = 0;
	virtual int lookupattachment(const char* pattachmentname) = 0;
	virtual bool getattachment(int number, Vector& origin, Vector& angles) = 0;
	virtual bool getattachment(int number, matrix3x4& matrix) = 0;
	virtual float* getrenderclipplane(void) = 0;
	virtual int getskin() = 0;
	virtual void onthreadeddrawsetup() = 0;
	virtual bool usesflexdelayedweights() = 0;
	virtual void recordtoolmessage() = 0;
	virtual bool shoulddrawforsplitscreenuser(int nslot) = 0;
	virtual unsigned char overridealphamodulation(unsigned char nalpha) = 0;
	virtual unsigned char overrideshadowalphamodulation(unsigned char nalpha) = 0;
};

class c_clientnetworkable
{
public:
	virtual i_client_unknown* GetIClientUnknown() = 0;
	virtual void Release() = 0;
	virtual client_class* get_client_class() = 0;
	virtual void NotifyShouldTransmit(int state) = 0;
	virtual void OnPreDataChanged(int updateType) = 0;
	virtual void OnDataChanged(int updateType) = 0;
	virtual void PreDataUpdate(int updateType) = 0;
	virtual void PostDataUpdate(int updateType) = 0;
	virtual void __unkn(void) = 0;
	virtual bool IsDormant(void) = 0;
	virtual int EntIndex(void) const = 0;
	virtual void ReceiveMessage(int classID, bf_read& msg) = 0;
	virtual void* GetDataTableBasePtr() = 0;
	virtual void SetDestroyedOnRecreateEntities(void) = 0;
};

enum csgohitboxid : int
{
	head,
	neck,
	pelvis,
	stomach,
	chest,
	lower_chest,
	upper_chest,
	right_thigh,
	left_thigh,
	right_calf,
	left_calf,
	right_foot,
	left_foot,
	right_hand,
	left_hand,
	right_upper_arm,
	right_forearm,
	left_upper_arm,
	left_forearm,
	max
};

enum MoveType
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4,
	MAX_MOVETYPE
};

class c_basecombat_weapon;

class c_base_entity : public i_client_unknown, public iclientrenderable, public c_clientnetworkable
{
public:

	template <class _t>
	inline _t get_value(uintptr_t offset)
	{
		return *(_t*)((unsigned long)this + offset);
	}

	template <class _t>
	_t* get_pointer(uintptr_t offset)
	{
		return (_t*)((unsigned long)this + offset);
	}

	model_t* get_model()
	{
		return *(model_t**)((unsigned long)this + 0x6C);
	}

	Vector get_bone(int i)
	{
		matrix3x4 bone_matrix[128];
		if (this->setupbones(bone_matrix, 128, 0x0007FF00, 0.0f))
		{
			return Vector(bone_matrix[i][0][3], bone_matrix[i][1][3], bone_matrix[i][2][3]);
		}
		return Vector(0, 0, 0); // return origin if invalid.
	}
	bool is_dormant()
	{
		return get_value<bool>(0xED);
	}
	bool* b_spotted()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseEntity", "m_bSpotted");
		return get_pointer<bool>(offset);
	}
	Vector get_vec_eyepos()
	{
		return (*this->m_Origin() + this->m_vecViewOffset());
	}

	int get_move_type() {
		return *reinterpret_cast<int*>((uintptr_t)this + (uintptr_t)0x258);
	}

	bool is_valid_player()
	{
		return !(!this || this == nullptr  || this == NULL || this->m_iHealth() <= 0 || this->is_dormant() || this->get_client_class()->m_ClassID != 38);
	}

	Vector* m_Origin()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseEntity", "m_vecOrigin");
		return get_pointer<Vector>(offset);
	}

	Vector m_vecViewOffset()
	{
		static uintptr_t offset = retrieve_offset("DT_BasePlayer", "localdata", "m_vecViewOffset[0]");
		return get_value<Vector>(offset);
	}

	QAngle m_angEyeAngles()
	{
		static uintptr_t offset = retrieve_offset("DT_CSPlayer", "m_angEyeAngles[0]");
		return get_value<QAngle>(offset);
	}

	QAngle* eyeangle_ptr()
	{
		static uintptr_t offset = retrieve_offset("DT_CSPlayer", "m_angEyeAngles[0]");
		return get_pointer<QAngle>(offset);
	}


	float m_flLowerBodyYawTarget()
	{
		static uintptr_t offset = retrieve_offset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		return get_value<float>(offset);
	}

	Vector m_vecVelocity()
	{
		static uintptr_t offset = retrieve_offset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		return get_value<Vector>(offset);
	}

	QAngle m_aimPunchAngle()
	{
		static uintptr_t offset = retrieve_offset("DT_BasePlayer", "localdata", "m_Local", "m_aimPunchAngle");
		return get_value<QAngle>(offset);
	}

	int m_iHealth()
	{
		static uintptr_t offset = retrieve_offset("DT_BasePlayer", "m_iHealth");
		return get_value<int>(offset);
	}

	int m_ArmorValue()
	{
		static auto offset = retrieve_offset("DT_CSPlayer", "m_ArmorValue");
		return get_value<int>(offset);
	}

	int m_fFlags()
	{
		static uintptr_t offset = retrieve_offset("DT_BasePlayer", "m_fFlags");
		return get_value<int>(offset);
	}

	int m_iTeamNum()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseEntity", "m_iTeamNum");
		return get_value<int>(offset);
	}

	int m_lifeState()
	{
		static uintptr_t offset = retrieve_offset("DT_BasePlayer", "m_lifeState");
		return get_value<int>(offset);
	}

	HANDLE m_hOwnerEntity()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseEntity", "m_hOwnerEntity");
		return get_value<HANDLE>(offset);
	}

	bool m_bGunGameImmunity()
	{
		static uintptr_t offset = retrieve_offset("DT_CSPlayer", "m_bGunGameImmunity");
		return get_value<bool>(offset);
	}

	void UpdateClientSideAnimation()
	{
		return getvfunc<void(__thiscall*)(void*)>(this, 218)(this);
	}


	bool setup_bones(matrix3x4* bone_to_world_out, int max_bones, int bonemask, float curtime)
	{
#if 0
		__asm
		{
			mov edi, this
			lea ecx, DWORD ptr ds : [edi + 0x4]
			mov edx, DWORD ptr ds : [ecx]
			push curtime
			push bonemask
			push max_bones
			push bone_to_world_out
			call DWORD ptr ds : [edx + 0x34]
		}
		return true;
#endif
		return this->setupbones(bone_to_world_out, max_bones, bonemask, curtime);
	}

	int get_tick_base()
	{
		static uintptr_t offset = retrieve_offset("DT_BasePlayer", "localdata", "m_nTickBase");
		return get_value<int>(offset);
	}

	// Save as pointers so we can patch them later for lag compensation.

	Vector* m_vecMins()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseEntity", "m_Collision", "m_vecMins");
		return get_pointer<Vector>(offset);
	}

	Vector* m_vecMaxs()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseEntity", "m_Collision", "m_vecMaxs");
		return get_pointer<Vector>(offset);
	}


	float* m_flCycle()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseAnimating", "serveranimdata", "m_flCycle");
		return get_pointer<float>(offset);
	}

	int* m_nSequence()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseAnimating", "m_nSequence");
		return get_pointer<int>(offset);
	}

	Vector get_abs_origins()
	{
		return getvfunc<Vector&(__thiscall*)(void*)>(this, 10)(this);
	}

	Vector GetAbsAngles()
	{
		return getvfunc<Vector&(__thiscall*)(void*)>(this, 11)(this);
	}

	float* m_flSimulationTime()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseEntity", "m_flSimulationTime");
		return get_pointer<float>(offset);
	}

	bool trace_from_smoke(Vector src);

	bool is_flashed()
	{
		static uintptr_t offset = retrieve_offset("DT_CSPlayer", "m_flFlashDuration");
		return get_value<float>(offset) > 0;
	}

	int* get_weapons()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseCombatCharacter", "m_hMyWeapons");
		return get_pointer<int>(offset);
	}

	HANDLE get_viewmodel_handle() {
		static uintptr_t offset = retrieve_offset("DT_BasePlayer", "m_hViewModel[0]");
		return get_value<HANDLE>(offset);
	}

	int* m_nModelIndex()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseEntity", "m_nModelIndex");
		return get_pointer<int>(offset);
	}

	// Weapons.
	c_basecombat_weapon* m_pActiveWeapon();
};

class c_bomb
{
private:

	template <class _t>
	inline _t get_value(uintptr_t offset)
	{
		return *(_t*)((unsigned long)this + offset);
	}

	template <class _t>
	_t* get_pointer(uintptr_t offset)
	{
		return (_t*)((unsigned long)this + offset);
	}

public:

	float get_blow_time()
	{
		static auto offset = retrieve_offset("DT_PlantedC4", "m_flC4Blow");
		return get_value<float>(offset);
	}

	bool is_ticking()
	{
		static auto offset = retrieve_offset("DT_PlantedC4", "m_bBombTicking");
		return get_value<bool>(offset);
	}

};

class c_entity_list
{
public:
	virtual c_clientnetworkable* getclientnetworkable(int entindex) = 0;
	virtual c_clientnetworkable* getclientnetworkablefromhandle(unsigned long entity_handle) = 0;
	virtual i_client_unknown* getclientunknownfromhandle(unsigned long entity_handle) = 0;
	virtual c_base_entity* getcliententity(int entindex) = 0;
	virtual c_base_entity* getcliententityfromhandle(HANDLE entity_handle) = 0;
	virtual int numberofentities(bool include_non_networkable) = 0;
	virtual int gethighestentityindex(void) = 0;
	virtual void setmaxentities(int maxents) = 0;
	virtual int getmaxentities() = 0;
};

enum weapon_type_id
{
	weapon_deagle = 1,
	weapon_dualberetta,
	weapon_fiveseven,
	weapon_glock,
	weapon_ak47 = 7,
	weapon_aug,
	weapon_awp,
	weapon_famas,
	weapon_g3sg1,
	weapon_galil = 13,
	weapon_m249,
	weapon_m4a4 = 16,
	weapon_mac10,
	weapon_p90 = 19,
	weapon_mp5 = 23,
	weapon_ump45,
	weapon_xm1014,
	weapon_bizon,
	weapon_mag7,
	weapon_negev,
	weapon_sawedoff,
	weapon_tec9,
	weapon_zeusx27,
	weapon_p2000,
	weapon_mp7,
	weapon_mp9,
	weapon_nova,
	weapon_p250,
	weapon_scar20 = 38,
	weapon_sg553,
	weapon_ssg08,
	weapon_knife = 42,
	weapon_flashbang,
	weapon_hegrenade,
	weapon_smokegrenade,
	weapon_molotov,
	weapon_decoy,
	weapon_inc,
	weapon_c4,
	weapon_m4a1s = 60,
	weapon_usps,
	weapon_cz75 = 63,
	weapon_revolver = 64
};

class c_weapon_info
{
public:
	char _0x0000[20];
	__int32 max_clip;			//0x0014 
	char _0x0018[12];
	__int32 max_reserved_ammo;	//0x0024 
	char _0x0028[96];
	char* hud_name;				//0x0088 
	char* weapon_name;			//0x008C 
	char _0x0090[60];
	__int32 type;				//0x00CC 
	__int32 price;				//0x00D0 
	__int32 reward;				//0x00D4 
	char _0x00D8[20];
	BYTE full_auto;				//0x00EC 
	char _0x00ED[3];
	__int32 damage;				//0x00F0 
	float armor_ratio;			//0x00F4 
	__int32 bullets;			//0x00F8 
	float penetration;			//0x00FC 
	char _0x0100[8];
	float range;				//0x0108 
	float range_modifier;		//0x010C 
	char _0x0110[16];
	BYTE silencer;				//0x0120 
	char _0x0121[15];
	float max_speed;			//0x0130 
	float max_speed_alt;		//0x0134 
	char _0x0138[76];
	__int32 recoil_seed;		//0x0184
};

class c_basecombat_weapon : public c_base_entity 
{
public:

	short filtered_index()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
		auto index = get_value<short>(offset);

		if (index == 42 || index == 59 || index == 41
			|| index == 500 || index == 505 || index == 506
			|| index == 507 || index == 508 || index == 509
			|| index == 515 || index == 512 || index == 523
			|| index == 520 || index == 519 || index == 522)
			return 69;

		return index;
	}

	short m_iItemDefinitionIndex()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
		return get_value<short>(offset);
	}

	short* m_iItemDefinitionIndexPtr()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
		return get_pointer<short>(offset);
	}

	float Get_Innacuracy()
	{
		return getvfunc<float(__thiscall*)(void*)>(this, 471)(this);
	}

	float GetSpread()
	{
		return getvfunc<float(__thiscall*)(void*)>(this, 440)(this);
	}

	void Update_Accuracy_Penalty()
	{
		getvfunc<void(__thiscall*)(void*)>(this, 472)(this);
	}

	bool is_zeus()
	{
		return this->m_iItemDefinitionIndex() == weapon_type_id::weapon_zeusx27;
	}

	bool is_knife()
	{
		auto index = this->m_iItemDefinitionIndex();

		return (index == 42 || index == 59 || index == 41
			|| index == 500 || index == 505 || index == 506
			|| index == 507 || index == 508 || index == 509
			|| index == 515 || index == 512 || index == 523
			|| index == 520 || index == 519 || index == 522);

		return (!is_firearm() && !is_zeus() && !is_grenade() && !is_bomb());
	}

	bool is_pistol()
	{
		int weapon_id = this->m_iItemDefinitionIndex();

		return (weapon_id == weapon_type_id::weapon_glock || weapon_id == weapon_type_id::weapon_p2000
			|| weapon_id == weapon_type_id::weapon_p250 || weapon_id == weapon_type_id::weapon_deagle
			|| weapon_id == weapon_type_id::weapon_revolver || weapon_id == weapon_type_id::weapon_dualberetta
			|| weapon_id == weapon_type_id::weapon_tec9 || weapon_id == weapon_type_id::weapon_fiveseven
			|| weapon_id == weapon_type_id::weapon_usps || weapon_id == weapon_type_id::weapon_cz75);
	}

	bool is_rifle()
	{
		int weapon_id = this->m_iItemDefinitionIndex();

		return weapon_id == weapon_type_id::weapon_ak47
			|| weapon_id == weapon_type_id::weapon_m4a4
			|| weapon_id == weapon_type_id::weapon_m4a1s
			|| weapon_id == weapon_type_id::weapon_galil
			|| weapon_id == weapon_type_id::weapon_famas
			|| weapon_id == weapon_type_id::weapon_aug
			|| weapon_id == weapon_type_id::weapon_sg553;
	}

	bool is_smg()
	{
		int weapon_id = this->m_iItemDefinitionIndex();

		return weapon_id == weapon_type_id::weapon_mac10 || weapon_id == weapon_type_id::weapon_mp7
			|| weapon_id == weapon_type_id::weapon_mp9 || weapon_id == weapon_type_id::weapon_p90 ||
			weapon_id == weapon_type_id::weapon_bizon || weapon_id == weapon_type_id::weapon_ump45 ||
			weapon_id == weapon_type_id::weapon_mp5;
	}

	bool is_autosniper()
	{
		int weapon_id = this->m_iItemDefinitionIndex();

		return weapon_id == weapon_type_id::weapon_scar20 || weapon_id == weapon_type_id::weapon_g3sg1;
	}

	bool is_awp()
	{
		return (this->m_iItemDefinitionIndex() == weapon_type_id::weapon_awp);
	}

	bool is_heavy()
	{
		int weapon_id = this->m_iItemDefinitionIndex();

		return weapon_id == weapon_type_id::weapon_negev || weapon_id == weapon_type_id::weapon_m249;
	}

	bool is_scout()
	{
		return (this->m_iItemDefinitionIndex() == weapon_type_id::weapon_ssg08);
	}

	bool is_shotgun()
	{
		int weapon_id = this->m_iItemDefinitionIndex();

		return weapon_id == weapon_type_id::weapon_xm1014 || weapon_id == weapon_type_id::weapon_nova
			|| weapon_id == weapon_type_id::weapon_sawedoff || weapon_id == weapon_type_id::weapon_mag7;
	}

	bool is_grenade()
	{
		int weapon_id = this->m_iItemDefinitionIndex();

		return (weapon_id == weapon_type_id::weapon_hegrenade || weapon_id == weapon_type_id::weapon_decoy
			|| weapon_id == weapon_type_id::weapon_flashbang || weapon_id == weapon_type_id::weapon_molotov
			|| weapon_id == weapon_type_id::weapon_smokegrenade || weapon_id == weapon_type_id::weapon_inc);
	}

	bool is_bomb()
	{
		return this->m_iItemDefinitionIndex() == weapon_type_id::weapon_c4;
	}

	bool is_firearm()
	{
		return is_pistol() || is_rifle() ||
			is_smg() || is_autosniper() || is_awp()
			|| is_shotgun() || is_heavy() || is_scout();
	}

	float m_flNextPrimaryAttack()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
		return get_value<float>(offset);
	}

	c_weapon_info* get_weapon_info()
	{
		return getvfunc<c_weapon_info*(__thiscall*)(void*)>(this, 448)(this);
	}

	int* get_paint_kit()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseAttributableItem", "m_nFallbackPaintKit");
		return get_pointer<int>(offset);
	}

	int* get_fallbackseed()
	{	
		static uintptr_t offset = retrieve_offset("DT_BaseAttributableItem", "m_nFallbackSeed");
		return get_pointer<int>(offset);
	}

	float* get_fallbackwear()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseAttributableItem", "m_flFallbackWear");
		return get_pointer<float>(offset);
	}

	int* get_accountid()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iAccountID");
		return get_pointer<int>(offset);
	}

	char* get_custom_name()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_szCustomName");
		return get_pointer<char>(offset);
	}

	int* get_item_id_high()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemIDHigh");
		return get_pointer<int>(offset);
	}

	int* get_original_owner_xuidlow()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
		return get_pointer<int>(offset);
	}

	int* get_original_owner_xuidhigh()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
		return get_pointer<int>(offset);
	}

	void set_model_index(int index)
	{
		getvfunc<void(__thiscall*)(void*, int)>(this, 75)(this, index);
	}

	int* m_nModelIndex()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseViewModel", "m_nModelIndex");
		return get_pointer<int>(offset);
	}
	int* m_iWorldModelIndex()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseCombatWeapon", "m_iWorldModelIndex");
		return get_pointer<int>(offset);
	}
	int* m_nViewModelIndex()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseViewModel", "m_nViewModelIndex");
		return get_pointer<int>(offset);
	}

	int* m_iViewModelIndex()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseCombatWeapon", "m_iViewModelIndex");
		return get_pointer<int>(offset);
	}

	HANDLE GetWeaponWorldModelHandle() {
		static uintptr_t offset = retrieve_offset("DT_BaseCombatWeapon", "m_hWeaponWorldModel");
		return get_value<HANDLE>(offset);
	}

	int Clip1()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseCombatWeapon", "m_iClip1");
		return get_value<int>(offset);
	}
};

class c_viewmodel
{
public:
	template <class _t>
	inline _t get_value(uintptr_t offset)
	{
		return *(_t*)((unsigned long)this + offset);
	}

	template <class _t>
	_t* get_pointer(uintptr_t offset)
	{
		return (_t*)((unsigned long)this + offset);
	}

	int* m_nModelIndex()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseViewModel", "m_nModelIndex");
		return get_pointer<int>(offset);
	}

	HANDLE get_weapon_handle()
	{
		static uintptr_t offset = retrieve_offset("DT_BaseViewModel", "m_hWeapon");
		return get_value<HANDLE>(offset);
	}
};

class c_weaponworldmodel : public c_viewmodel
{
public:
};