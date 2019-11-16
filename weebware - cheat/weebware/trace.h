#pragma once

#define	CONTENTS_EMPTY			0
#define	CONTENTS_SOLID			0x1
#define	CONTENTS_WINDOW			0x2
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_BLOCKLOS		0x40
#define CONTENTS_OPAQUE			0x80
#define	LAST_VISIBLE_CONTENTS	CONTENTS_OPAQUE
#define ALL_VISIBLE_CONTENTS (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))
#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED			0x200	
#define CONTENTS_BLOCKLIGHT		0x400
#define CONTENTS_TEAM1			0x800
#define CONTENTS_TEAM2			0x1000
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000
#define CONTENTS_MOVEABLE		0x4000
#define	CONTENTS_AREAPORTAL		0x8000
#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000
#define	CONTENTS_CURRENT_0		0x40000
#define	CONTENTS_CURRENT_90		0x80000
#define	CONTENTS_CURRENT_180	0x100000
#define	CONTENTS_CURRENT_270	0x200000
#define	CONTENTS_CURRENT_UP		0x400000
#define	CONTENTS_CURRENT_DOWN	0x800000
#define	CONTENTS_ORIGIN			0x1000000
#define	CONTENTS_MONSTER		0x2000000
#define	CONTENTS_DEBRIS			0x4000000
#define	CONTENTS_DETAIL			0x8000000
#define	CONTENTS_TRANSLUCENT	0x10000000
#define	CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000

#define	SURF_LIGHT		0x0001
#define	SURF_SKY2D		0x0002
#define	SURF_SKY		0x0004
#define	SURF_WARP		0x0008
#define	SURF_TRANS		0x0010
#define SURF_NOPORTAL	0x0020
#define	SURF_TRIGGER	0x0040
#define	SURF_NODRAW		0x0080
#define	SURF_HINT		0x0100
#define	SURF_SKIP		0x0200
#define SURF_NOLIGHT	0x0400
#define SURF_BUMPLIGHT	0x0800
#define SURF_NOSHADOWS	0x1000
#define SURF_NODECALS	0x2000
#define SURF_NOPAINT	0x2000
#define SURF_NOCHOP		0x4000
#define SURF_HITBOX		0x8000

#define	MASK_ALL					(0xFFFFFFFF)
#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define	MASK_PLAYERSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define	MASK_NPCSOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define	MASK_NPCFLUID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define	MASK_WATER					(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define	MASK_OPAQUE					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define MASK_OPAQUE_AND_NPCS		(MASK_OPAQUE|CONTENTS_MONSTER)
#define MASK_BLOCKLOS				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define MASK_BLOCKLOS_AND_NPCS		(MASK_BLOCKLOS|CONTENTS_MONSTER)
#define	MASK_VISIBLE					(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define MASK_VISIBLE_AND_NPCS		(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define	MASK_SHOT					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
#define MASK_SHOT_HULL				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
#define MASK_SHOT_PORTAL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define MASK_SOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
#define MASK_NPCSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define MASK_NPCWORLDSTATIC			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define MASK_NPCWORLDSTATIC_FLUID	(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
#define MASK_SPLITAREAPORTAL		(CONTENTS_WATER|CONTENTS_SLIME)
#define MASK_CURRENT				(CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define	MASK_DEADSOLID				(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)


struct cplane_t
{
	Vector	normal;
	float	dist;
	byte	type;
	byte	signbits;
	byte	padding[2];
};

class CBaseTrace
{
public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}
};

struct csurface_t
{
	const char*		name;
	short			surfaceProps;
	unsigned short	flags;
};

class CGameTrace : public CBaseTrace
{
public:
	inline bool DidHit() const
	{
		return fraction < 1.0f || allsolid || startsolid;
	}

	bool                    DidHitNonWorldEntity() const;
	int                     GetEntityIndex() const;
	bool					IsVisible() const;

public:

	float                   fractionleftsolid;
	csurface_t              surface;
	int                     hitgroup;
	short                   physicsbone;
	unsigned short          worldSurfaceIndex;
	c_base_entity*          m_pEnt;
	int                     hitbox;

	CGameTrace() { }

private:
	CGameTrace(const CGameTrace& vOther);
};

class IHandleEntity
{
public:

	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle() = 0;
	virtual void GetRefEHandle() const = 0;
};



struct Ray_t
{
	__declspec(align(16)) Vector m_Start;
	__declspec(align(16)) Vector m_Delta;
	__declspec(align(16)) Vector m_StartOffset;
	__declspec(align(16)) Vector m_Extents;
	//without your matrix3x4
	bool m_IsRay;
	bool m_IsSwept;

	void Init(Vector& vecStart, Vector& vecEnd)
	{
		m_Delta = vecEnd - vecStart;

		m_IsSwept = (m_Delta.size_sqr() != 0);

		m_Extents.x = m_Extents.y = m_Extents.z = 0.0f;

		m_IsRay = true;

		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.0f;

		m_Start = vecStart;
	}
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(c_base_entity* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}
	virtual int	GetTraceType()
	{
		return 0;
	}
	void* pSkip;
};

class c_trace_customfilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(c_base_entity* pEntity, int contentsMask)
	{
		return pEntity == pTarget;
	}

	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_ENTITIES_ONLY;
	}

	void* pTarget;
};

class c_trace_customfilterworld : public ITraceFilter
{
public:
	bool ShouldHitEntity(c_base_entity* pEntity, int contentsMask)
	{
		return pEntity == pTarget;
	}

	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pTarget;
};

class ITraceListData
{
public:
	virtual ~ITraceListData() = 0;
	virtual void Reset() = 0;
	virtual bool IsEmpty() = 0;
	virtual bool CanTraceRay(const Ray_t &ray) = 0;
};

class i_engine_trace
{
public:
	virtual int		GetPointContents(const Vector &vecAbsPosition, int contentsMask = 0xFFFFFFFF, void** ppEntity = NULL) = 0;
	virtual int		GetPointContents_WorldOnly(const Vector &vecAbsPosition, int contentsMask = 0xFFFFFFFF) = 0;
	virtual int		GetPointContents_Collideable(void *pCollide, const Vector &vecAbsPosition) = 0;
	virtual void	ClipRayToEntity(const Ray_t &ray, unsigned int fMask, IHandleEntity *pEnt, CGameTrace *pTrace) = 0;
	virtual void	ClipRayToCollideable(const Ray_t &ray, unsigned int fMask, void *pCollide, CGameTrace *pTrace) = 0;
	virtual void	TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, CGameTrace *pTrace) = 0;
	virtual void	SetupLeafAndEntityListRay(const Ray_t &ray, void *pTraceData) = 0;
	virtual void    SetupLeafAndEntityListBox(const Vector &vecBoxMin, const Vector &vecBoxMax, ITraceListData *pTraceData) = 0;
	virtual void	TraceRayAgainstLeafAndEntityList(const Ray_t &ray, void *pTraceData, unsigned int fMask, ITraceFilter *pTraceFilter, CGameTrace *pTrace) = 0;
	virtual void	SweepCollideable(void *pCollide, const Vector &vecAbsStart, const Vector &vecAbsEnd, const Vector &vecAngles, unsigned int fMask, ITraceFilter *pTraceFilter, CGameTrace *pTrace) = 0;
	virtual void	EnumerateEntities(const Ray_t &ray, bool triggers, void *pEnumerator) = 0;
	virtual void	EnumerateEntities(const Vector &vecAbsMins, const Vector &vecAbsMaxs, void *pEnumerator) = 0;
	virtual void*	GetCollideable(IHandleEntity *pEntity) = 0;
	virtual int		GetStatByIndex(int index, bool bClear) = 0;
	virtual void	unknown() = 0;
	virtual void	 GetCollidableFromDisplacementsInAABB() = 0;
	virtual int		GetNumDisplacements() = 0;
	virtual void	GetDisplacementMesh(int nIndex, void *pMeshTriList) = 0;
	virtual bool	unknown2() = 0;
	virtual bool	PointOutsideWorld(const Vector &ptTest) = 0;
	virtual int		GetLeafContainingPoint(const Vector &ptTest) = 0;
	virtual void*	AllocTraceListData() = 0;
};