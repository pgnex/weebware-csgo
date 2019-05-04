#pragma once
#include "Ai.h"


class HidingSpot
{
public:
	enum
	{
		IN_COVER = 0x01,							// in a corner with good hard cover nearby
		GOOD_SNIPER_SPOT = 0x02,							// had at least one decent sniping corridor
		IDEAL_SNIPER_SPOT = 0x04,							// can see either very far, or a large area, or both
		EXPOSED = 0x08							// spot in the open, usually on a ledge or cliff
	};

	Vector m_pos{ 0.0f, 0.0f, 0.0f }; // Position

	unsigned int m_id{ 0 }; // Unique id inside this area
	unsigned char m_flags; // Flags are defined above
};

struct Ray
{
	Vector from{ 0.0f, 0.0f, 0.0f };
	Vector to{ 0.0f, 0.0f, 0.0f };
};

enum NavAttributeType
{
	NAV_MESH_INVALID = 0,
	NAV_MESH_CROUCH = 0x0000001, // must crouch to use this node/area
	NAV_MESH_JUMP = 0x0000002, // must jump to traverse this area (only used during generation)
	NAV_MESH_PRECISE = 0x0000004, // do not adjust for obstacles, just move along area
	NAV_MESH_NO_JUMP = 0x0000008, // inhibit discontinuity jumping
	NAV_MESH_STOP = 0x0000010, // must stop when entering this area
	NAV_MESH_RUN = 0x0000020, // must run to traverse this area
	NAV_MESH_WALK = 0x0000040, // must walk to traverse this area
	NAV_MESH_AVOID = 0x0000080, // avoid this area unless alternatives are too dangerous
	NAV_MESH_TRANSIENT = 0x0000100, // area may become blocked, and should be periodically checked
	NAV_MESH_DONT_HIDE = 0x0000200, // area should not be considered for hiding spot generation
	NAV_MESH_STAND = 0x0000400, // bots hiding in this area should stand
	NAV_MESH_NO_HOSTAGES = 0x0000800, // hostages shouldn't use this area
	NAV_MESH_STAIRS = 0x0001000, // this area represents stairs, do not attempt to climb or jump them - just walk up
	NAV_MESH_NO_MERGE = 0x0002000, // don't merge this area with adjacent areas
	NAV_MESH_OBSTACLE_TOP = 0x0004000, // this nav area is the climb point on the tip of an obstacle
	NAV_MESH_CLIFF = 0x0008000, // this nav area is adjacent to a drop of at least CliffHeight

	NAV_MESH_FIRST_CUSTOM = 0x00010000, // apps may define custom app-specific bits starting with this value
	NAV_MESH_LAST_CUSTOM = 0x04000000, // apps must not define custom app-specific bits higher than with this value

	NAV_MESH_HAS_ELEVATOR = 0x40000000, // area is in an elevator's path
	NAV_MESH_NAV_BLOCKER = 0x80000000, // area is blocked by nav blocker ( Alas, needed to hijack a bit in the attributes to get within a cache line [7/24/2008 tom])
};

enum NavDirType
{
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3,

	NUM_DIRECTIONS
};

enum NavCornerType
{
	NORTH_WEST = 0,
	NORTH_EAST = 1,
	SOUTH_EAST = 2,
	SOUTH_WEST = 3,

	NUM_CORNERS
};

class CNavLadder
{
public:
	enum LadderDirectionType
	{
		LADDER_UP = 0,
		LADDER_DOWN,
		NUM_LADDER_DIRECTIONS
	};

	Vector m_top;
	Vector m_bottom;

	float m_length;
	float m_width;
};

enum { MAX_NAV_TEAMS = 2 };

class CNavArea;

struct NavConnect
{
	union
	{
		unsigned int id{ 0 };
		CNavArea *area;
	};

	mutable float length{ 0.0f };

	bool operator==(const NavConnect &other) const
	{
		return (area == other.area);
	}
};
union NavLadderConnect
{
	unsigned int id{ 0 };
	CNavLadder *ladder;

	bool operator==(const NavLadderConnect &other) const
	{
		return (ladder == other.ladder) ? true : false;
	}
};


struct SpotOrder
{
	float t{ 0.0f }; // parametric distance along ray where this spot first has LOS to our path

	union
	{
		HidingSpot *spot{ nullptr }; // the spot to look at
		unsigned int id; // spot ID for save/load
	};
};

struct SpotEncounter
{
	NavConnect from{};
	NavDirType fromDir{ NavDirType::NUM_DIRECTIONS };

	NavConnect to{};
	NavDirType toDir{ NavDirType::NUM_DIRECTIONS };

	Ray path{}; // the path segment

	std::vector<SpotOrder> spots; // list of spots to look at, in order of occurrence
};

struct AreaBindInfo	// for pointer loading and binding
{
	union
	{
		CNavArea *area{ nullptr };
		unsigned int id;
	};

	unsigned char attributes;				// VisibilityType

	bool operator==(const AreaBindInfo &other) const
	{
		return (area == other.area);
	}
};

class CNavArea
{
	// This should be private but this makes things a whole lot easier
public:
	unsigned int m_id{ 0 };

	Vector m_nwCorner{ 0.0f, 0.0f, 0.0f }; // North-west corner
	Vector m_seCorner{ 0.0f, 0.0f, 0.0f }; // South-east corner

	float m_invDxCorners{ 0.0f }; // TODO: What is this?
	float m_invDyCorners{ 0.0f }; // TODO: What is this?

	float m_neZ{ 0.0f }; // Height of the north-east corner
	float m_swZ{ 0.0f }; // Height of the south-west corner

	Vector m_center{ 0.0f, 0.0f, 0.0f }; // Center

	int m_attributeFlags{ 0 }; // Flags for this area, see NavAttributeType

	std::vector<NavConnect> m_connect; // Connected areas for each direction
	std::vector<NavLadderConnect> m_ladder[CNavLadder::NUM_LADDER_DIRECTIONS]; // Connected ladders

	unsigned int m_visibleAreaCount{ 0 }; // Areas visible from this area

	float m_lightIntensity[NUM_CORNERS]; // 0.0 -> 1.0

	unsigned int m_uiVisibleAreaCount{ 0 };

	unsigned short entry{ 0 };

	AreaBindInfo m_inheritVisibilityFrom;

	std::vector<AreaBindInfo> m_potentiallyVisibleAreas;

	std::vector<HidingSpot> m_hidingSpots;

	std::vector<SpotEncounter> m_spotEncounters;

	float m_earliestOccupyTime[MAX_NAV_TEAMS]; // Minimum time to reach this area from the teams spawn

public:
	//Pathfinding stuff:
	float m_flTotalCostSoFar;
	CNavArea* m_pParent;

	void InitPathfindingVars();

public:

	// Default constructor
	CNavArea(void);

	// Constructor
	CNavArea(std::ifstream &ifs);

	// Calculates the height of the area at the given x, z coordinates
	// @return Height at the given point
	float GetZ(const Vector &pos) const;

	// Calculates the distance between the area and the given point
	// @return Non-squared distance
	float GetDistanceSquaredToPoint(const Vector &pos) const;

	// Check if the given point is overlapping the area
	// @return True if 'pos' is within 2D extents of area.
	bool IsOverlapping(const Vector &pos, float tolerance = 0.0f) const;

	// Calculates whether or not the given point is inside the area
	// @return True if the point is inside the area
	bool Contains(const Vector &vecPoint) const;

	// Loads the area from a file stream
	// @return True on success
	bool LoadFromFile(std::ifstream &ifs);
};


class INavFile
{
private:

public:

	unsigned int m_magic{ 0 }; // Magic number used to identify the file

	unsigned int m_version{ 0 }; // Version of the file, CS:GO is using version 16 files
	unsigned int m_subVersion{ 0 };

	unsigned int m_saveBspSize{ 0 }; // Size of the corresponding BSP file used to verify an up-to-date a navigation mesh

	unsigned char m_isAnalyzed{ (unsigned char)0x00 };

	unsigned short m_usPlaceCount{ 0 };
	std::vector<std::string> m_vStrPlaceNames;
	unsigned char m_hasUnnamedAreas{ (unsigned char)0x00 };

	std::vector<CNavArea> m_areas;
	unsigned int m_uiAreaCount{ 0 };

	// Loads a nav file
	// @return True on success
	bool Load(std::ifstream &ifs);

	// Initializes the class
	bool Initialize(void);

	// Restores default values
	void RestoreDefaults(void);

	// Default constructor
	INavFile(void);

	// Destructor
	virtual ~INavFile(void);

	const CNavArea* GetNavAreaByID(int id) const;
};

class CWalkbot
{
public:
	c_base_entity * m_pLocal = nullptr;
	void create_move(c_usercmd* pCmd, c_base_entity* alocal);
	void OnUnload();

	//Make path public for drawing?
	std::vector<Vector> m_path;

	INavFile m_navfile;

	bool m_iswalking = false;

	bool m_maploaded = false;

	CNavArea* m_target_area = nullptr;

	const char* get_place_name(CNavArea* area);

	ai_movement	m_CurrentMode; 

	c_base_entity * m_TargetEntity = nullptr;
private:
	bool m_error_loading = false;
	char m_lastmap[32];


	unsigned int m_iAreasArraySize = 0;
	CNavArea** m_NavAreas;
	inline CNavArea* GetAreaByID(int nID);

	//Bot logic
	float		m_flLastThink = 0.f;
	CNavArea*	m_pCurrentArea = nullptr;
	int			m_iCurrentPathPoint = 0;

	//A* Pathfinding	===============================================================
	std::vector<CNavArea*> m_pOpenList;
	std::vector<CNavArea*> m_pClosedList;
	Vector SpawnT;
	Vector SpawnCT;

	//Utils
	CNavArea* GetNavArea(Vector vecPos);
	float GetMoveCost(CNavArea* pFrom, CNavArea* pTo, CNavLadder* pLadder = nullptr);

	template <typename T>
	bool ListContainsElement(std::vector<T> pVector, T pElement);

	void init_path_finding();
	bool find_path(CNavArea* pSource, CNavArea* pTarget);

	void WalkOnPath(c_usercmd* pCmd);
	void create_path();

};

extern CWalkbot g_Walkbot;

#define INVALID_NAV_PTR 0x1
