#pragma once

#include "shared.h"

// #include "../math/Vector.hpp"
// #include "../csgostructs.hpp"
// #include "valve_sdk\misc\EHandle.hpp"

#define MAX_BEAM_ENTS	10
#define NOISE_DIVISIONS	128

class Beam_t {
public:
	void    *unk_1; // probably 2-3 vmts?
	void    *unk_2;
	void    *unk_3;
	Vector	m_Mins;
	Vector	m_Maxs;
	void    *m_queryHandleHalo;
	float	m_haloProxySize;
	Beam_t  *next;
	int		type;
	int		flags;
	int		numAttachments;
	Vector	attachment[MAX_BEAM_ENTS];
	Vector	delta;
	float	t;
	float	freq;
	float	die;
	float	width;
	float	endWidth;
	float	fadeLength;
	float	amplitude;
	float	life;
	float	r, g, b;
	float	brightness;
	float	speed;
	float	frameRate;
	float	frame;
	int		segments;
	// EHANDLE	entity[ MAX_BEAM_ENTS ];
	// uint8_t _pad_00[ sizeof( unsigned long ) * 10 ];
	int		attachmentIndex[MAX_BEAM_ENTS];
	int		modelIndex;
	int		haloIndex;
	float	haloScale;
	int		frameCount;
	float	rgNoise[NOISE_DIVISIONS + 1];
	void    *trail;
	float	start_radius;
	float	end_radius;
	bool	m_bCalculatedNoise;
	float	m_flHDRColorScale;
};

struct ImpactEntry_t {
	Vector m_pos;
	bool   m_is_last_impact;

	__forceinline ImpactEntry_t() :
		m_pos{ 0.f, 0.f, 0.f }, m_is_last_impact{ false } {

	}

	__forceinline ImpactEntry_t(const Vector &pos, bool is_last_impact) :
		m_pos{ pos }, m_is_last_impact{ is_last_impact } {

	}
};

struct ImpactData_t {
	bool                         m_skip;
	Vector                       m_shoot_pos;
	int                          m_tickbase;
	float                        m_curtime;
	std::vector< ImpactEntry_t > m_impacts;

	__forceinline ImpactData_t() :
		m_skip{ false }, m_shoot_pos{ 0.f, 0.f, 0.f }, m_tickbase{ -1 }, m_curtime{ -1.f }, m_impacts{} {

	}

	__forceinline void clear() {
		m_shoot_pos = Vector(0.f, 0.f, 0.f);
		m_tickbase = -1;
		m_curtime = -1.f;

		m_impacts.clear();
	}
};

struct BeamInfo_t {
	int			 m_nType;
	c_base_entity *m_pStartEnt;
	int			 m_nStartAttachment;
	c_base_entity *m_pEndEnt;
	int			 m_nEndAttachment;
	Vector		 m_vecStart;
	Vector		 m_vecEnd;
	int			 m_nModelIndex;
	const char	 *m_pszModelName;
	int			 m_nHaloIndex;
	const char	 *m_pszHaloName;
	float		 m_flHaloScale;
	float		 m_flLife;
	float		 m_flWidth;
	float		 m_flEndWidth;
	float		 m_flFadeLength;
	float		 m_flAmplitude;
	float		 m_flBrightness;
	float		 m_flSpeed;
	int			 m_nStartFrame;
	float		 m_flFrameRate;
	float		 m_flRed;
	float		 m_flGreen;
	float		 m_flBlue;
	bool		 m_bRenderable;
	int			 m_nSegments;
	int			 m_nFlags;
	Vector		 m_vecCenter;
	float		 m_flStartRadius;
	float		 m_flEndRadius;

	__forceinline BeamInfo_t() {
		m_nType = 0; // TE_BEAMPOINTS;
		m_nSegments = -1;
		m_pszModelName = 0;
		m_pszHaloName = 0;
		m_nModelIndex = -1;
		m_nHaloIndex = -1;
		m_bRenderable = true;
		m_nFlags = 0;
	}
};

class IViewRenderBeams {
public:
	virtual void _pad_00() = 0;
	virtual void _pad_01() = 0;
	virtual void _pad_02() = 0;
	virtual void _pad_03() = 0;
	virtual void DrawBeam(Beam_t *beam) = 0;
	virtual void _pad_05() = 0;
	virtual void _pad_06() = 0;
	virtual void _pad_07() = 0;
	virtual void _pad_08() = 0;
	virtual void _pad_09() = 0;
	virtual void _pad_10() = 0;
	virtual	void CreateBeamPoints(const Vector& start, const Vector& end, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b) = 0;
};