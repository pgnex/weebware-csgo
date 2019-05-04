#pragma once
#include "Header.h"
#include "maths.h"


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
	// get the name of the material.  this is a full path to 
	// the vmt file starting from "hl2/materials" (or equivalent) without
	// a file extension.
	virtual const char* getname() const = 0;
	virtual const char* gettexturegroupname() const = 0;

	// get the preferred size/bitdepth of a preview image of a material.
	// this is the sort of image that you would use for a thumbnail view
	// of a material, or in worldcraft until it uses materials to render.
	// separate this for the tools maybe
	virtual previewimageretval_t getpreviewimageproperties(int* width, int* height, imageformat* imageformat, bool* istranslucent) const = 0;

	// get a preview image at the specified width/height and bitdepth.
	// will do resampling if necessary.(not yet!!! :) )
	// will do color format conversion. (works now.)
	virtual previewimageretval_t getpreviewimage(unsigned char* data, int width, int height, imageformat imageformat) const = 0;
	//
	virtual int getmappingwidth() = 0;
	virtual int getmappingheight() = 0;
	virtual int getnumanimationframes() = 0;

	// for material subrects (material pages).  offset(u,v) and scale(u,v) are normalized to texture.
	virtual bool inmaterialpage(void) = 0;
	virtual void getmaterialoffset(float* poffset) = 0;
	virtual void getmaterialscale(float* pscale) = 0;
	virtual imaterial* getmaterialpage(void) = 0;

	// find a vmt variable.
	// this is how game code affects how a material is rendered.
	// the game code must know about the params that are used by
	// the shader for the material that it is trying to affect.
	virtual imaterialvar* findvar(const char* varname, bool* found, bool complain = true) = 0;

	// the user never allocates or deallocates materials.  reference counting is
	// used instead.  garbage collection is done upon a call to 
	// imaterialsystem::uncacheunusedmaterials.
	virtual void incrementreferencecount(void) = 0;
	virtual void decrementreferencecount(void) = 0;

	inline void addref()
	{
		// incrementreferencecount();
		typedef void(__thiscall* incrementreferencecountfn)(void*);
		return getvfunc<incrementreferencecountfn>(this, 14)(this);
	}

	inline void release()
	{
		decrementreferencecount();
	}

	// each material is assigned a number that groups it with like materials
	// for sorting in the application.
	virtual int getenumerationid(void) const = 0;
	virtual void getlowrescolorsample(float s, float t, float* color) const = 0;

	// this computes the state snapshots for this material
	virtual void recomputestatesnapshots() = 0;

	// are we translucent?
	virtual bool istranslucent() = 0;

	// are we alphatested?
	virtual bool isalphatested() = 0;

	// are we vertex lit?
	virtual bool isvertexlit() = 0;

	// gets the vertex format
	virtual vertexformat_t getvertexformat() const = 0;

	// returns true if this material uses a material proxy
	virtual bool hasproxy(void) const = 0;
	virtual bool usesenvcubemap(void) = 0;
	virtual bool needstangentspace(void) = 0;
	virtual bool needspoweroftwoframebuffertexture(bool bcheckspecifictothisframe = true) = 0;
	virtual bool needsfullframebuffertexture(bool bcheckspecifictothisframe = true) = 0;

	// returns true if the shader doesn't do skinning itself and requires
	// the data that is sent to it to be preskinned.
	virtual bool needssoftwareskinning(void) = 0;

	// apply constant color or alpha modulation
	virtual void alphamodulate(float alpha) = 0;
	virtual void colormodulate(float r, float g, float b) = 0;

	// material var flags...
	virtual void setmaterialvarflag(materialvarflags_t flag, bool on) = 0;
	virtual bool getmaterialvarflag(materialvarflags_t flag) const = 0;

	// gets material reflectivity
	virtual void getreflectivity(Vector& reflect) = 0;

	// gets material property flags
	virtual bool getpropertyflag(materialpropertytypes_t type) = 0;

	// is the material visible from both sides?
	virtual bool istwosided() = 0;

	// sets the shader associated with the material
	virtual void setshader(const char* pshadername) = 0;

	// can't be const because the material might have to precache itself.
	virtual int getnumpasses(void) = 0;

	// can't be const because the material might have to precache itself.
	virtual int gettexturememorybytes(void) = 0;

	// meant to be used with materials created using creatematerial
	// it updates the materials to reflect the current values stored in the material vars
	virtual void refresh() = 0;

	// gr - returns true is material uses lightmap alpha for blending
	virtual bool needslightmapblendalpha(void) = 0;

	// returns true if the shader doesn't do lighting itself and requires
	// the data that is sent to it to be prelighted
	virtual bool needssoftwarelighting(void) = 0;

	// gets at the shader parameters
	virtual int shaderparamcount() const = 0;
	virtual imaterialvar** getshaderparams(void) = 0;

	// returns true if this is the error material you get back from imaterialsystem::findmaterial if
	// the material can't be found.
	virtual bool iserrormaterial() const = 0;
	virtual void unused() = 0;

	// gets the current alpha modulation
	virtual float getalphamodulation() = 0;

	virtual void getcolormodulation(float* r, float* g, float* b) = 0;

	// is this translucent given a particular alpha modulation?
	virtual bool istranslucentundermodulation(float falphamodulation = 1.0f) const = 0;

	// fast find that stores the index of the found var in the string table in local cache
	virtual imaterialvar* findvarfast(char const* pvarname, unsigned int* ptoken) = 0;

	// sets new vmt shader parameters for the material
	virtual void setshaderandparams(KeyValues* pkeyvalues) = 0;
	virtual const char* getshadername() const = 0;
	virtual void deleteifunreferenced() = 0;
	virtual bool isspritecard() = 0;
	virtual void callbindproxy(void* proxydata) = 0;
	virtual void refreshpreservingmaterialvars() = 0;
	virtual bool wasreloadedfromwhitelist() = 0;
	virtual bool settempexcluded(bool bset, int nexcludeddimensionlimit) = 0;
	virtual int getreferencecount() const = 0;
};
class CGameTrace;

typedef CGameTrace trace_t;

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
		QAngle const& angles, trace_t* ptrace,
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
public:
	void force_update()
	{
		// delta tick
		*reinterpret_cast<int*>(std::uintptr_t(this) + 0x174) = -1;
	};
};
