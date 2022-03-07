#pragma once
#include "Header.h"
#include "maths.h"
#include "key_values.h"

class CRender;

class IWeaponSystem
{
	virtual void unused0() = 0;
	virtual void unused1() = 0;
public:
	virtual c_weapon_info* GetWpnData(unsigned ItemDefinitionIndex) = 0;
};

struct mstudiobone_t
{
	int sznameindex;

	inline char* const GetName(void) const
	{
		return ((char *)this) + sznameindex;
	}

	int parent;
	int bonecontroller[6];

	Vector pos;
	float quat[4];
	Vector rot;
	Vector posscale;
	Vector rotscale;

	matrix3x4 poseToBone;
	float qAlignment[4];
	int flags;
	int proctype;
	int procindex; // procedural rule
	mutable int physicsbone; // index into physically simulated bone
	inline void* GetProcedure() const
	{
		if (procindex == 0) return NULL; else return (void *)(((byte *)this) + procindex);
	};

	int surfacepropidx; // index into string tablefor property name
	inline char* const GetSurfaceProps(void) const
	{
		return ((char *)this) + surfacepropidx;
	}

	int contents; // See BSPFlags.h for the contents flags

	int unused[8]; // remove as appropriate
};

class match_extensions;
class match_events_subscription;
class match_title;
class match_network_msg_controller;
class match_system;

class match_session {
public:
	virtual void filler0() = 0; // 0
	virtual KeyValues* get_session_settings() = 0; // 1
	virtual void update_session_settings(KeyValues* settings) = 0; // 2
	virtual void command(KeyValues* command) = 0; // 3
};

class c_match_framework {
public:
	virtual void filler0() = 0;
	virtual void filler1() = 0;
	virtual void filler2() = 0;
	virtual void filler3() = 0;
	virtual void filler4() = 0;
	virtual void filler5() = 0;
	virtual void filler6() = 0;
	virtual void filler7() = 0;
	virtual void filler8() = 0;
	virtual void run_frame() = 0; // 9
	virtual match_extensions* get_match_extensions() = 0; // 10
	virtual match_events_subscription* get_events_subscription() = 0; // 11
	virtual match_title* get_match_title() = 0; // 12
	virtual match_session* get_match_session() = 0; // 13
	virtual match_network_msg_controller* get_msg_controller() = 0; // 15
	virtual match_system* get_match_system() = 0; // 15
	virtual void create_session(KeyValues* settings) = 0; // 16
	virtual void match_session(KeyValues* settings) = 0; // 17
	virtual void accept_invite(int controller) = 0; // 18
	virtual void close_session() = 0; // 19
};


struct mstudiobbox_t
{
	int bone;
	int group; // intersection group
	Vector bbmin; // bounding box 
	Vector bbmax;
	int hitboxnameindex; // offset to the name of the hitbox.
	int pad[3];
	float radius;
	int pad2[4];

	char* getHitboxName()
	{
		if (hitboxnameindex == 0)
			return "";

		return ((char*)this) + hitboxnameindex;
	}
};

struct mstudiohitboxset_t
{
	int sznameindex;

	inline char* const pszName(void) const
	{
		return ((char*)this) + sznameindex;
	}

	int numhitboxes;
	int hitboxindex;

	inline mstudiobbox_t* pHitbox(int i) const
	{
		return (mstudiobbox_t*)(((BYTE*)this) + hitboxindex) + i;
	};
};

struct mstudioposeparamdesc_t
{
	int					sznameindex;
	int					flags;
	float				start;
	float				end;
	float				loop;
};

struct studiohdr_t
{
	int id;
	int version;

	int checksum;

	char name[64];
	int length;

	Vector eyeposition;

	Vector illumposition;

	Vector hull_min;
	Vector hull_max;

	Vector view_bbmin;
	Vector view_bbmax;

	int flags;

	int numbones;
	int boneindex;

	inline mstudiobone_t* GetBone(int i) const
	{
		return (mstudiobone_t *)(((byte *)this) + boneindex) + i;
	};

	//	inline mstudiobone_t *pBone(int i) const { Assert(i >= 0 && i < numbones); return (mstudiobone_t *)(((byte *)this) + boneindex) + i; };

	int numbonecontrollers;
	int bonecontrollerindex;

	int numhitboxsets;
	int hitboxsetindex;

	mstudiohitboxset_t* GetHitboxSet(int i) const
	{
		return (mstudiohitboxset_t*)(((byte*)this) + hitboxsetindex) + i;
	}

	mstudioposeparamdesc_t *pLocalPoseParameter(int i)
	{
		return (mstudioposeparamdesc_t *)(((byte *)this) + localposeparamindex) + i;
	};

	inline mstudiobbox_t* GetHitbox(int i, int set) const
	{
		mstudiohitboxset_t const* s = GetHitboxSet(set);

		if (!s)
			return NULL;

		return s->pHitbox(i);
	}

	inline int GetHitboxCount(int set) const
	{
		mstudiohitboxset_t const* s = GetHitboxSet(set);

		if (!s)
			return 0;

		return s->numhitboxes;
	}

	int numlocalanim;
	int localanimindex;

	int numlocalseq;
	int localseqindex;

	mutable int activitylistversion;
	mutable int eventsindexed;

	int numtextures;
	int textureindex;

	int numcdtextures;
	int cdtextureindex;

	int numskinref;
	int numskinfamilies;
	int skinindex;

	int numbodyparts;
	int bodypartindex;

	int numlocalattachments;
	int localattachmentindex;

	int numlocalnodes;
	int localnodeindex;
	int localnodenameindex;

	int numflexdesc;
	int flexdescindex;

	int numflexcontrollers;
	int flexcontrollerindex;

	int numflexrules;
	int flexruleindex;

	int numikchains;
	int ikchainindex;

	int nummouths;
	int mouthindex;

	int numlocalposeparameters;
	int localposeparamindex;


	int surfacepropindex;

	int keyvalueindex;
	int keyvaluesize;


	int numlocalikautoplaylocks;
	int localikautoplaylockindex;

	float mass;
	int contents;

	int numincludemodels;
	int includemodelindex;

	mutable void* virtualModel;

	int szanimblocknameindex;
	int numanimblocks;
	int animblockindex;

	mutable void* animblockModel;

	int bonetablebynameindex;

	void* pVertexBase;
	void* pIndexBase;

	byte constdirectionallightdot;

	byte rootLOD;

	byte numAllowedRootLODs;

	byte unused[1];

	int unused4;

	int numflexcontrollerui;
	int flexcontrolleruiindex;
	float flVertAnimFixedPointScale;
	int unused3[1];
	int studiohdr2index;
	int unused2[1];
};

typedef unsigned short mdlhandle_t;

enum renderabletranslucencytype_t
{
	renderable_is_opaque = 0,
	renderable_is_translucent,
	renderable_is_two_pass, // has both translucent and opaque sub-partsa
};

class imaterialvar
{
public:

};

enum imageformat
{
	image_format_unknown = -1,
	image_format_rgba8888 = 0,
	image_format_abgr8888,
	image_format_rgb888,
	image_format_bgr888,
	image_format_rgb565,
	image_format_i8,
	image_format_ia88,
	image_format_p8,
	image_format_a8,
	image_format_rgb888_bluescreen,
	image_format_bgr888_bluescreen,
	image_format_argb8888,
	image_format_bgra8888,
	image_format_dxt1,
	image_format_dxt3,
	image_format_dxt5,
	image_format_bgrx8888,
	image_format_bgr565,
	image_format_bgrx5551,
	image_format_bgra4444,
	image_format_dxt1_onebitalpha,
	image_format_bgra5551,
	image_format_uv88,
	image_format_uvwq8888,
	image_format_rgba16161616f,
	image_format_rgba16161616,
	image_format_uvlx8888,
	image_format_r32f, // single-channel 32-bit floating point
	image_format_rgb323232f, // note: d3d9 does not have this format
	image_format_rgba32323232f,
	image_format_rg1616f,
	image_format_rg3232f,
	image_format_rgbx8888,

	image_format_null, // dummy format which takes no video memory

	// compressed normal map formats
	image_format_ati2n, // one-surface ati2n / dxn format
	image_format_ati1n, // two-surface ati1n format

	image_format_rgba1010102, // 10 bit-per component render targets
	image_format_bgra1010102,
	image_format_r16f, // 16 bit fp format

	// depth-stencil texture formats
	image_format_d16,
	image_format_d15s1,
	image_format_d32,
	image_format_d24s8,
	image_format_linear_d24s8,
	image_format_d24x8,
	image_format_d24x4s4,
	image_format_d24fs8,
	image_format_d16_shadow, // specific formats for shadow mapping
	image_format_d24x8_shadow, // specific formats for shadow mapping

	// supporting these specific formats as non-tiled for procedural cpu access (360-specific)
	image_format_linear_bgrx8888,
	image_format_linear_rgba8888,
	image_format_linear_abgr8888,
	image_format_linear_argb8888,
	mage_format_linear_bgra8888,
	image_format_linear_rgb888,
	image_format_linear_bgr888,
	image_format_linear_bgrx5551,
	image_format_linear_i8,
	image_format_linear_rgba16161616,

	image_format_le_bgrx8888,
	image_format_le_bgra8888,

	num_image_formats
};

enum previewimageretval_t
{
	material_preview_image_bad = 0,
	material_preview_image_ok,
	material_no_preview_image,
};

typedef UINT64  vertexformat_t;
struct cplane_t;

enum materialpropertytypes_t
{
	material_property_needs_lightmap = 0, // bool
	material_property_opacity, // int (enum materialpropertyopacitytypes_t)
	material_property_reflectivity, // vec3_t
	material_property_needs_bumped_lightmaps // bool
};

enum materialvarflags_t;



class imaterial
{
public:
	// Get the name of the material.  This is a full path to
	// the vmt file starting from "hl2/materials" (or equivalent) without
	// a file extension.
	virtual const char* GetName() const = 0;
	virtual const char* gettexturegroupname() const = 0;

	// Get the preferred size/bitDepth of a preview image of a material.
	// This is the sort of image that you would use for a thumbnail view
	// of a material, or in WorldCraft until it uses materials to render.
	// separate this for the tools maybe
	virtual previewimageretval_t GetPreviewImageProperties(int* width, int* height, imageformat* imageFormat, bool* isTranslucent) const = 0;

	// Get a preview image at the specified width/height and bitDepth.
	// Will do resampling if necessary.(not yet!!! :) )
	// Will do color format conversion. (works now.)
	virtual previewimageretval_t GetPreviewImage(unsigned char* data, int width, int height, imageformat imageFormat) const = 0;
	//
	virtual int             GetMappingWidth() = 0;
	virtual int             GetMappingHeight() = 0;

	virtual int             GetNumAnimationFrames() = 0;

	// For material subrects (material pages).  Offset(u,v) and scale(u,v) are normalized to texture.
	virtual bool            InMaterialPage(void) = 0;
	virtual void            GetMaterialOffset(float* pOffset) = 0;
	virtual void            GetMaterialScale(float* pScale) = 0;
	virtual imaterial* GetMaterialPage(void) = 0;

	// find a vmt variable.
	// This is how game code affects how a material is rendered.
	// The game code must know about the params that are used by
	// the shader for the material that it is trying to affect.
	virtual imaterialvar* FindVar(const char* varName, bool* found, bool complain = true) = 0;

	// The user never allocates or deallocates materials.  Reference counting is
	// used instead.  Garbage collection is done upon a call to
	// IMaterialSystem::UncacheUnusedMaterials.
	virtual void            incrementreferencecount(void) = 0;
	virtual void            DecrementReferenceCount(void) = 0;

	inline void AddRef() { incrementreferencecount(); }
	inline void Release() { DecrementReferenceCount(); }

	// Each material is assigned a number that groups it with like materials
	// for sorting in the application.
	virtual int             GetEnumerationID(void) const = 0;

	virtual void            GetLowResColorSample(float s, float t, float* color) const = 0;

	// This computes the state snapshots for this material
	virtual void            RecomputeStateSnapshots() = 0;

	// Are we translucent?
	virtual bool            IsTranslucent() = 0;

	// Are we alphatested?
	virtual bool            IsAlphaTested() = 0;

	// Are we vertex lit?
	virtual bool            IsVertexLit() = 0;

	// Gets the vertex format
	virtual vertexformat_t  GetVertexFormat() const = 0;

	// returns true if this material uses a material proxy
	virtual bool            HasProxy(void) const = 0;

	virtual bool            UsesEnvCubemap(void) = 0;

	virtual bool            NeedsTangentSpace(void) = 0;

	virtual bool            NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool            NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;

	// returns true if the shader doesn't do skinning itself and requires
	// the data that is sent to it to be preskinned.
	virtual bool            NeedsSoftwareSkinning(void) = 0;

	// Apply constant color or alpha modulation
	virtual void            AlphaModulate(float alpha) = 0;
	virtual void            colormodulate(float r, float g, float b) = 0;

	// Material Var flags...
	virtual void            setmaterialvarflag(materialvarflags_t flag, bool on) = 0;
	virtual bool            GetMaterialVarFlag(materialvarflags_t flag) = 0;

	// Gets material reflectivity
	virtual void            GetReflectivity(Vector& reflect) = 0;

	// Gets material property flags
	virtual bool            GetPropertyFlag(materialpropertytypes_t type) = 0;

	// Is the material visible from both sides?
	virtual bool            IsTwoSided() = 0;

	// Sets the shader associated with the material
	virtual void            SetShader(const char* pShaderName) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int             GetNumPasses(void) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int             GetTextureMemoryBytes(void) = 0;

	// Meant to be used with materials created using CreateMaterial
	// It updates the materials to reflect the current values stored in the material vars
	virtual void            Refresh() = 0;

	// GR - returns true is material uses lightmap alpha for blending
	virtual bool            NeedsLightmapBlendAlpha(void) = 0;

	// returns true if the shader doesn't do lighting itself and requires
	// the data that is sent to it to be prelighted
	virtual bool            NeedsSoftwareLighting(void) = 0;

	// Gets at the shader parameters
	virtual int             ShaderParamCount() const = 0;
	virtual imaterialvar** GetShaderParams(void) = 0;

	// Returns true if this is the error material you get back from IMaterialSystem::FindMaterial if
	// the material can't be found.
	virtual bool            iserrormaterial() const = 0;

	virtual void            Unused() = 0;

	// Gets the current alpha modulation
	virtual float           GetAlphaModulation() = 0;
	virtual void            GetColorModulation(float* r, float* g, float* b) = 0;

	// Is this translucent given a particular alpha modulation?
	virtual bool            IsTranslucentUnderModulation(float fAlphaModulation = 1.0f) const = 0;

	// fast find that stores the index of the found var in the string table in local cache
	virtual imaterialvar* FindVarFast(char const* pVarName, unsigned int* pToken) = 0;

	// Sets new VMT shader parameters for the material
	virtual void            SetShaderAndParams(void* pKeyValues) = 0;
	virtual const char* GetShaderName() const = 0;

	virtual void            DeleteIfUnreferenced() = 0;

	virtual bool            IsSpriteCard() = 0;

	virtual void            CallBindProxy(void* proxyData) = 0;

	virtual void            RefreshPreservingMaterialVars() = 0;

	virtual bool            WasReloadedFromWhitelist() = 0;

	virtual bool            SetTempExcluded(bool bSet, int nExcludedDimensionLimit) = 0;

	virtual int             GetReferenceCount() const = 0;
};

class CGameTrace;


class c_unknown
{
public:
};


class iv_model_info
{
public:
	virtual ~iv_model_info(void)
	{
	}

	virtual const model_t			*getmodel(int modelindex) const = 0;
	// returns index of model by name
	virtual int						getmodelindex(const char *name) const = 0;
	// returns name of model
	virtual const char				*getmodelname(const model_t *model) const = 0;
	virtual void					unused() {};
	virtual c_unknown				*getvcollide(const model_t *model) const = 0;
	virtual c_unknown				*getvcollide(int modelindex) const = 0;
	virtual void					getmodelbounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual	void					getmodelrenderbounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual int						getmodelframecount(const model_t *model) const = 0;
	virtual int						getmodeltype(const model_t *model) const = 0;
	virtual void					*getmodelextradata(const model_t *model) = 0;
	virtual bool					modelhasmaterialproxy(const model_t *model) const = 0;
	virtual bool					istranslucent(model_t const* model) const = 0;
	virtual bool					istranslucenttwopass(const model_t *model) const = 0;
	virtual void					unused0() {};
	virtual renderabletranslucencytype_t computetranslucencytype(const model_t *model, int nskin, int nbody) = 0;
	virtual int						getmodelmaterialcount(const model_t* model) const = 0;
	virtual void					getmodelmaterials(const model_t *model, int count, imaterial** ppmaterials) = 0;
	virtual bool					ismodelvertexlit(const model_t *model) const = 0;
	virtual const char				*getmodelkeyvaluetext(const model_t *model) = 0;
	virtual bool					getmodelkeyvalue(const model_t *model, c_unknown &buf) = 0; // supports keyvalue blocks in submodels
	virtual float					getmodelradius(const model_t *model) = 0;

	virtual const studiohdr_t		*findmodel(const studiohdr_t *pstudiohdr, void **cache, const char *modelname) const = 0;
	virtual const studiohdr_t		*findmodel(void *cache) const = 0;
	virtual	c_unknown			*getvirtualmodel(const studiohdr_t *pstudiohdr) const = 0;
	virtual byte					*getanimblock(const studiohdr_t *pstudiohdr, int iblock) const = 0;
	virtual bool					hasanimblockbeenpreloaded(studiohdr_t const*, int) const = 0;

	// available on client only!!!
	virtual void					getmodelmaterialcolorandlighting(const model_t *model, Vector const& origin,
		QAngle const& angles, CGameTrace* ptrace,
		Vector& lighting, Vector& matcolor) = 0;
	virtual void					getilluminationpoint(const model_t *model, iclientrenderable *prenderable, Vector const& origin,
		QAngle const& angles, Vector* plightingcenter) = 0;

	virtual int						getmodelcontents(int modelindex) const = 0;
	virtual void					unused11() {};
	virtual studiohdr_t				*getstudiomodel(const model_t *mod) = 0;
	virtual int						getmodelspritewidth(const model_t *model) const = 0;
	virtual int						getmodelspriteheight(const model_t *model) const = 0;

	// sets/gets a map-specified fade range (client only)
	virtual void					setlevelscreenfaderange(float flminsize, float flmaxsize) = 0;
	virtual void					getlevelscreenfaderange(float *pminarea, float *pmaxarea) const = 0;

	// sets/gets a map-specified per-view fade range (client only)
	virtual void					setviewscreenfaderange(float flminsize, float flmaxsize) = 0;

	// computes fade alpha based on distance fade + screen fade (client only)
	virtual unsigned char			computelevelscreenfade(const Vector &vecabsorigin, float flradius, float flfadescale) const = 0;
	virtual unsigned char			computeviewscreenfade(const Vector &vecabsorigin, float flradius, float flfadescale) const = 0;

	// both client and server
	virtual int						getautoplaylist(const studiohdr_t *pstudiohdr, unsigned short **pautoplaylist) const = 0;

	// gets a virtual terrain collision model (creates if necessary)
	// note: this may return null if the terrain model cannot be virtualized
	virtual c_unknown			*getcollideforvirtualterrain(int index) = 0;
	virtual bool					isusingfbtexture(const model_t *model, int nskin, int nbody, void /*iclientrenderable*/ *pclientrenderable) const = 0;
	virtual const model_t			*findorloadmodel(const char *name) const = 0;
	virtual mdlhandle_t				getcachehandle(const model_t *model) const = 0;
	// returns planes of non-nodraw brush model surfaces
	virtual int						getbrushmodelplanecount(const model_t *model) const = 0;
	virtual void					getbrushmodelplane(const model_t *model, int nindex, cplane_t &plane, Vector *porigin) const = 0;
	virtual int						getsurfacepropsforvirtualterrain(int index) = 0;
	virtual bool					usesenvcubemap(const model_t *model) const = 0;
	virtual bool					usesstaticlighting(const model_t *model) const = 0;
};

class c_debug_overlay
{
public:
	int screen_pos(const Vector& vecOrigin, Vector& vecScreen)
	{
		return getvfunc<int(__thiscall*)(PVOID, const Vector&, Vector&)>(this, 13)(this, vecOrigin, vecScreen);
	}
};

class c_render_view
{
public:
	// Draw normal brush model.
	// If pMaterialOverride is non-null, then all the faces of the bmodel will
	// set this material rather than their regular material.
	virtual void			DrawBrushModel(c_base_entity *baseentity, model_t *model, const Vector& origin, const QAngle& angles, bool sort) = 0;

	// Draw brush model that has no origin/angles change ( uses identity transform )
	// FIXME, Material proxy IClientEntity *baseentity is unused right now, use DrawBrushModel for brushes with
	//  proxies for now.
	virtual void			DrawIdentityBrushModel(void *pList, model_t *model) = 0;

	// Mark this dynamic light as having changed this frame ( so light maps affected will be recomputed )
	virtual void			TouchLight(struct dlight_t *light) = 0;
	// Draw 3D Overlays
	virtual void			Draw3DDebugOverlays(void) = 0;
	// Sets global blending fraction
	virtual void			SetBlend(float blend) = 0;
	virtual float			GetBlend(void) = 0;

	// Sets global color modulation
	virtual void			SetColorModulation(float const* blend) = 0;
	virtual void			GetColorModulation(float* blend) = 0;

	// Wrap entire scene drawing
	virtual void			SceneBegin(void) = 0;
	//virtual void			SceneEnd( void ) = 0;

	// Gets the fog volume for a particular point
	virtual void			GetVisibleFogVolume(const Vector& eyePoint, void *pInfo) = 0;
};

struct view_setup_t {
	__int32   x;
	__int32   m_nUnscaledX;
	__int32   y;
	__int32   m_nUnscaledY;
	__int32   width;
	__int32   m_nUnscaledWidth;
	__int32   height;
	__int32   m_nUnscaledHeight;
	char      pad1[0x90];
	float     fov;
	float     viewmodel_fov;
	Vector    origin;
	Vector    angles;
	char      pad2[0x7C];
};

class c_global_vars
{
public:
	float realtime;
	int framecount;
	float absoluteframetime;
	float absoluteframestarttimestddev;
	float curtime;
	float frametime;
	int maxclients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
	int simTicksThisFrame;
	int network_protocol;
	void* pSaveData;
private:
	bool m_bClient;
public:
	bool m_bRemoteClient;
private:
	int nTimestampNetworkingBase;
	int nTimestampRandomizeWindow;
};

class c_unknownmat_class {};

class c_mat_system 
{
public:
	imaterial* create_mat(const char* pMaterialName, KeyValues* pVMTKeyValues)
	{
		return getvfunc<imaterial*(__thiscall*)(PVOID, const char*, KeyValues*)>(this, 83)(this, pMaterialName, pVMTKeyValues);
	}

	imaterial* find_material(const char* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = 0)
	{
		return getvfunc<imaterial*(__thiscall*)(PVOID, const char*, const char*, bool, const char*)>(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}

	unsigned short first_material() 
	{
		return getvfunc<unsigned short(__thiscall*)(PVOID)>(this, 86)(this);
	}

	unsigned short next_material(unsigned short handle)
	{
		return getvfunc<unsigned short(__thiscall*)(PVOID, unsigned short)>(this, 87)(this, handle);
	}

	unsigned short invalid_material()
	{
		return getvfunc<unsigned short(__thiscall*)(PVOID)>(this, 88)(this);
	}

	imaterial* get_mat(unsigned short handle)
	{
		return getvfunc<imaterial*(__thiscall*)(PVOID, unsigned short)>(this, 89)(this, handle);
	}
};

#ifndef TEXTURE_GROUP_NAMES_H
#define TEXTURE_GROUP_NAMES_H
#ifdef _WIN32
#pragma once
#endif

// These are given to FindMaterial to reference the texture groups that show up on the 
#define TEXTURE_GROUP_LIGHTMAP						"Lightmaps"
#define TEXTURE_GROUP_WORLD							"World textures"
#define TEXTURE_GROUP_MODEL							"Model textures"
#define TEXTURE_GROUP_VGUI							"VGUI textures"
#define TEXTURE_GROUP_PARTICLE						"Particle textures"
#define TEXTURE_GROUP_DECAL							"Decal textures"
#define TEXTURE_GROUP_SKYBOX						"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				"ClientEffect textures"
#define TEXTURE_GROUP_OTHER							"Other textures"
#define TEXTURE_GROUP_PRECACHED						"Precached"				// TODO: assign texture groups to the precached materials
#define TEXTURE_GROUP_CUBE_MAP						"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					"RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					"Unaccounted textures"	// Textures that weren't assigned a texture group.
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		"Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			"Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			"Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					"DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					"ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					"Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				"Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					"Morph Targets"

#endif // TEXTURE_GROUP_NAMES_H


struct modelrenderinfo_t
{
	Vector origin;
	Vector angles;
	char pad[0x4];
	c_unknownmat_class* pRenderable;
	const model_t* pModel;
	const matrix3x4* pModelToWorld;
	const matrix3x4* pLightingOffset;
	const Vector* pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	c_unknownmat_class instance;
};

enum overridetype_t
{
	override_normal = 0,
	override_build_shadows,
	override_depth_write,
	override_whatever
};

class c_model_render
{
public:
	virtual int drawmodel(int flags, iclientrenderable* prenderable, c_unknownmat_class instance, int entity_index, const model_t* model, Vector const& origin, Vector const& angles, int skin, int body, int hitboxset, const matrix3x4* modeltoworld = 0, const matrix3x4* plightingoffset = 0) = 0;
	virtual void forcedmaterialoverride(imaterial* newmaterial, overridetype_t noverridetype = overridetype_t::override_normal, int a = 0) = 0;
	virtual bool isforcedmaterialoverride() = 0;
	virtual void setviewtarget(const c_unknownmat_class* pstudiohdr, int nbodyindex, const Vector& target) = 0;
	virtual c_unknownmat_class createinstance(iclientrenderable* prenderable, c_unknownmat_class* pcache = 0) = 0;
	virtual void destroyinstance(c_unknownmat_class handle) = 0;
	virtual void setstaticlighting(c_unknownmat_class handle, c_unknownmat_class* phandle) = 0;
	virtual c_unknownmat_class getstaticlighting(c_unknownmat_class handle) = 0;
	virtual bool changeinstance(c_unknownmat_class handle, iclientrenderable* prenderable) = 0;
	virtual void adddecal(c_unknownmat_class handle, c_unknownmat_class const& ray, Vector const& decalup, int decalindex, int body, bool nopokethru = false, int maxlodtodecal = -1) = 0;
	virtual void removealldecals(c_unknownmat_class handle) = 0;
	virtual bool modelhasdecals(c_unknownmat_class handle) = 0;
	virtual void removealldecalsfromallmodels() = 0;
	virtual matrix3x4* drawmodelshadowsetup(iclientrenderable* prenderable, int body, int skin, c_unknownmat_class* pinfo, matrix3x4* pcustombonetoworld = 0) = 0;
	virtual void drawmodelshadow(iclientrenderable* prenderable, const c_unknownmat_class& info, matrix3x4* pcustombonetoworld = 0) = 0;
	virtual bool recomputestaticlighting(c_unknownmat_class handle) = 0;
	virtual void releaseallstaticpropcolordata() = 0;
	virtual void restoreallstaticpropcolordata() = 0;
	virtual int drawmodelex(modelrenderinfo_t& pinfo) = 0;
	virtual int drawmodelexstaticprop(modelrenderinfo_t& pinfo) = 0;
	virtual bool drawmodelsetup(modelrenderinfo_t& pinfo, c_unknownmat_class* pstate, matrix3x4** ppbonetoworldout) = 0;
	virtual void drawmodelexecute(c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pinfo, matrix3x4* pcustombonetoworld = 0) = 0;
	virtual void setuplighting(const Vector& veccenter) = 0;
	virtual int drawstaticproparrayfast(c_unknownmat_class* pprops, int count, bool bshadowdepth) = 0;
	virtual void suppressenginelighting(bool bsuppress) = 0;
	virtual void setupcolormeshes(int ntotalverts) = 0;
	virtual void setuplightingex(const Vector& veccenter, c_unknownmat_class handle) = 0;
	virtual bool getbrightestshadowinglightsource(const Vector& veccenter, Vector& lightpos, Vector& lightbrightness, bool ballownontaggedlights) = 0;
	virtual void computelightingstate(int ncount, const c_unknownmat_class* pquery, c_unknownmat_class* pstate, c_unknownmat_class** ppenvcubemaptexture) = 0;
	virtual void getmodeldecalhandles(LPVOID* pdecals, int ndecalstride, int ncount, const c_unknownmat_class* phandles) = 0;
	virtual void computestaticlightingstate(int ncount, const c_unknownmat_class* pquery, c_unknownmat_class* pstate, c_unknownmat_class* pdecalstate, c_unknownmat_class** ppstaticlighting, c_unknownmat_class** ppenvcubemaptexture, void* pcolormeshhandles) = 0;
	virtual void cleanupstaticlightingstate(int ncount, void* pcolormeshhandles) = 0;
};


class c_clientstate
{
	class c_clock_drift_mgr {
	public:
		float clock_offsets[17];
		std::uint32_t current_clock_offset;
		std::uint32_t server_tick;
		std::uint32_t client_tick;
	};

public:

	char pad1[156];
	net_channel* net_channel;
	std::uint32_t challenge_nr;
	char pad2[100];
	std::uint32_t signon_state;
	char pad3[8];
	float next_cmd_time;
	std::uint32_t server_count;
	std::uint32_t current_sequence;
	char pad4[8];
	c_clock_drift_mgr clock_drift_mgr;
	std::uint32_t delta_tick;
	bool paused;
	char pad5[3];
	std::uint32_t view_entity;
	std::uint32_t player_slot;
	char level_name[260];
	char level_name_short[80];
	char group_name[80];
	char pad6[92];
	std::uint32_t max_clients;
	char pad7[18820];
	float last_server_tick_time;
	bool insimulation;
	char pad8[3];
	unsigned int oldtickcount;
	float tick_remainder;
	float frame_time;
	std::uint32_t last_outgoing_command;
	std::uint32_t choked_commands;
	std::uint32_t last_command_ack;
	std::uint32_t command_ack;
	std::uint32_t sound_sequence;
	char pad9[80];
	Vector viewangles;

	void full_update()
	{
		// delta tick
		*reinterpret_cast<int*>(std::uintptr_t(this) + 0x174) = -1;
	};


};

struct surfacephysicsparams_t
{
	float    friction;
	float    elasticity;
	float    density;
	float    thickness;
	float    dampening;
};

struct surfaceaudioparams_t
{
	float    reflectivity;             // like elasticity, but how much sound should be reflected by this surface
	float    hardnessFactor;           // like elasticity, but only affects impact sound choices
	float    roughnessFactor;          // like friction, but only affects scrape sound choices   
	float    roughThreshold;           // surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes
	float    hardThreshold;            // surface hardness > this causes "hard" impacts, < this causes "soft" impacts
	float    hardVelocityThreshold;    // collision velocity > this causes "hard" impacts, < this causes "soft" impacts   
	float    highPitchOcclusion;       //a value betweeen 0 and 100 where 0 is not occluded at all and 100 is silent (except for any additional reflected sound)
	float    midPitchOcclusion;
	float    lowPitchOcclusion;
};

struct surfacesoundnames_t
{
	unsigned short    walkStepLeft;
	unsigned short    walkStepRight;
	unsigned short	  runStepLeft;
	unsigned short	  runStepRight;
	unsigned short    impactSoft;
	unsigned short    impactHard;
	unsigned short    scrapeSmooth;
	unsigned short    scrapeRough;
	unsigned short    bulletImpact;
	unsigned short    rolling;
	unsigned short    breakSound;
	unsigned short    strainSound;
};

struct surfacegameprops_t
{
public:
	float maxSpeedFactor;
	float jumpFactor;
	float flPenetrationModifier;
	float flDamageModifier;
	unsigned short material;
	byte climbable;
	char pad00[0x4];

};

struct surfacedata_t
{
	surfacephysicsparams_t    physics;
	surfaceaudioparams_t    audio;
	surfacesoundnames_t        sounds;
	surfacegameprops_t        game;
};

class c_phys_surface_props
{
public:
	virtual ~c_phys_surface_props( void ) {}

	// parses a text file containing surface prop keys
	virtual int		ParseSurfaceData( const char* pFilename, const char* pTextfile ) = 0;
	// current number of entries in the database
	virtual int		SurfacePropCount( void ) const = 0;

	virtual int		GetSurfaceIndex( const char* pSurfacePropName ) const = 0;
	virtual void	GetPhysicsProperties( int surfaceDataIndex, float* density, float* thickness, float* friction, float* elasticity ) const = 0;

	virtual surfacedata_t* GetSurfaceData( int surfaceDataIndex ) = 0;
	virtual const char* GetString( unsigned short stringTableIndex ) const = 0;


	virtual const char* GetPropName( int surfaceDataIndex ) const = 0;

	// sets the global index table for world materials
	// UNDONE: Make this per-CPhysCollide
	virtual void	SetWorldMaterialIndexTable( int* pMapArray, int mapSize ) = 0;

	// NOTE: Same as GetPhysicsProperties, but maybe more convenient
	virtual void	GetPhysicsParameters( int surfaceDataIndex, surfacephysicsparams_t* pParamsOut ) const = 0;
};
