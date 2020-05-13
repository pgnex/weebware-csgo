#include "Header.h"
#include "navmesh.h"
#include "Legit.h"

// Lots of credit to cr33per for his navmesh parsing

CWalkbot g_Walkbot;

const char* ExePath() {
	TCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);
	return buffer;
}

// Our main logical function.

void CWalkbot::create_move(c_usercmd* pCmd, c_base_entity* alocal)
{
#define debug 0

	m_pLocal = alocal;

	m_iswalking = false;

	//If local player doesn't exist then there was error in code.
	if (!m_pLocal || m_pLocal->is_valid_player() == false)
		return;

	const char* pszMapName = g_weebware.g_engine->get_level_name_short();
	//Nav mesh loading...
	if (m_maploaded == false)
	{
		m_navfile.Initialize();
		char buffer[MAX_PATH];
		TCHAR path_buf[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, path_buf);
		sprintf(buffer, "%s/csgo/maps/%s.nav", path_buf, pszMapName);
		std::ifstream ifs(buffer, std::ios::binary);
		if (!ifs.is_open())
		{
#if debug
			printf("failed to open map \n");
#endif
			m_maploaded = true;
			m_error_loading = true;
			ifs.close();
			return;
		}
#if debug
		printf("opened nav map \n");
#endif
		if (!m_navfile.Load(ifs))
		{
#if debug
			printf("failed to load map \n");
#endif
			m_maploaded = true;
			m_error_loading = true;
			ifs.close();
			return;
		}
#if debug
		printf("nav map now loaded \n");
#endif
		for (int i = 0; i < 32; i++)
		{
			m_lastmap[i] = pszMapName[i];
			if (pszMapName[i] == '\0')
				break;
		}

		//Build navmesh array for easy grabbing them by ID
		unsigned int iHigestID = 0;
		for (unsigned int i = 0; i < m_navfile.m_areas.size(); i++)
		{
			const CNavArea* pArea = &m_navfile.m_areas[i];

			if (pArea->m_id > iHigestID)
				iHigestID = pArea->m_id;
		}

		if (iHigestID == 0)
		{
#if debug
			printf("failed to get id \n");
#endif
			m_maploaded = true;
			m_error_loading = true;
			ifs.close();
			return;
		}

		m_NavAreas = (CNavArea**)malloc(sizeof(CNavArea*) * (iHigestID + 1));

		for (unsigned int i = 0; i <= iHigestID; i++)
			m_NavAreas[i] = (CNavArea*)m_navfile.GetNavAreaByID(i);

		m_iAreasArraySize = iHigestID + 1;

		m_maploaded = true;
		m_error_loading = false;
		ifs.close();
		for (unsigned int i = 0; i < m_navfile.m_areas.size(); i++)
		{
			CNavArea* area = &m_navfile.m_areas[i];
			if (area)
			{
				if (!strcmp(get_place_name(area), "CTSpawn"))
					SpawnCT = area->m_center;
				else if (!strcmp(get_place_name(area), "TSpawn"))
					SpawnT = area->m_center;
			}
		}
	}
	else
	{
		//Check if we're still on that same map.
		bool bSame = true;
		for (int i = 0; i < 32; i++)
		{
			if (pszMapName[i] == '\0' || m_lastmap[i] == '\0')
				break;

			if (m_lastmap[i] != pszMapName[i])
			{
				bSame = false;
				break;
			}
		}

		if (!bSame)
		{
			free(m_NavAreas);
			m_maploaded = false;
			return;
		}
	}

	//We've had some errors during loading, do not proceed.
	if (m_error_loading)
		return;

	// do rest of stuff here.

	// Lets get our current area
	m_pCurrentArea = GetNavArea(*m_pLocal->m_vecOrigin());

	if (!m_pCurrentArea)
		return;

#if debug
	printf("Successfully Located our NAVAREA\n");
#endif
	if (m_CurrentMode == ai_movement::random) {

		if (!m_target_area)
		{
			for (int i = 0; i < 5; i++)
			{
				int iPotentialNode = rand() % (m_navfile.m_areas.size() + 1);

				CNavArea* pArea = &m_navfile.m_areas[iPotentialNode];

				if (!pArea)
					return;

				if (pArea->m_attributeFlags & NAV_MESH_AVOID ||
					pArea->m_attributeFlags & NAV_MESH_INVALID ||
					pArea->m_attributeFlags & NAV_MESH_HAS_ELEVATOR || 
					pArea->m_attributeFlags & NAV_MESH_NAV_BLOCKER ||
					pArea->m_attributeFlags & NAV_MESH_CROUCH||
					pArea->m_attributeFlags & NAV_MESH_JUMP)
					continue;								

				init_path_finding();

				if (find_path(m_pCurrentArea, pArea))	//Found path, we're good.
				{
					m_target_area = pArea;
					break;
				}

			}
			if (m_target_area)
			{
				m_path.clear();
				create_path();
				m_iCurrentPathPoint = m_path.size() - 1;
			}
		}
	}

	if (m_CurrentMode == ai_movement::defuse_bomb) {

		if (m_TargetEntity) {

			if (!m_target_area)
			{
#if debug
				printf("We have defuse entity! \n");
#endif

				CNavArea* bomb_area = GetNavArea(*m_TargetEntity->m_vecOrigin());

				if (!bomb_area)
					return;
#if debug
				printf("We have found the bomb! \n");
#endif
				init_path_finding();

				if (find_path(m_pCurrentArea, bomb_area)) {
					m_target_area = bomb_area;
				}

				if (m_target_area) {
					m_path.clear();
					create_path();
					m_iCurrentPathPoint = m_path.size() - 1;
				}
			}

		}
	}


	if (m_target_area && m_pCurrentArea != m_target_area) {
		WalkOnPath(pCmd);
	}
	else {
		m_target_area = nullptr;
	}

}

inline CNavArea* CWalkbot::GetAreaByID(int nID)
{
	return m_NavAreas[nID];
}

void CWalkbot::create_path()
{
	int i = 0;
	CNavArea* pArea = m_target_area;

	while (true)
	{
		i++;
		if (i > 100)
			break;

		m_path.push_back(pArea->m_center);
		if (pArea->m_pParent == (CNavArea*)INVALID_NAV_PTR)
			return;
		pArea = pArea->m_pParent;
	}

}


// Credits for some parts to:
// Michael S. Booth (mike@turtlerockstudios.com)
// Valve @ January, 2003
// https://github.com/pmrowla/hl2sdk-csgo/blob/49e950f3eb820d88825f75e40f56b3e64790920a/game/server/nav_pathfind.h

const char* CWalkbot::get_place_name(CNavArea* area)
{
	if (!area)
		return "unknown";
	if (area->entry == 0)
		return "unknown";
	unsigned int i = area->entry - 1;
	if (i >= m_navfile.m_vStrPlaceNames.size())
		return "unknown";
	return m_navfile.m_vStrPlaceNames[i].c_str();
}

//TODO: Checking actual visibility, not just picking nearest one...
CNavArea* CWalkbot::GetNavArea(Vector vecPos)
{
	float flBestDistance = FLT_MAX;
	CNavArea* pBestArea = nullptr;

	for (unsigned int i = 0; i < m_navfile.m_areas.size(); i++)
	{
		CNavArea* pArea = &m_navfile.m_areas[i];

		float flDistance = (pArea->m_center - vecPos).size();

		if (flDistance < flBestDistance)
		{
			flBestDistance = flDistance;
			pBestArea = pArea;
		}
	}

	return pBestArea;
}

float CWalkbot::GetMoveCost(CNavArea* pFrom, CNavArea* pTo, CNavLadder* pLadder)
{

	float flDistance = (pFrom->m_center - pTo->m_center).size();

	// if this is a "crouch" area, add penalty
	if (pTo->m_attributeFlags & NAV_MESH_CROUCH)
	{
		const float crouchPenalty = 20.0f;		// 10
		flDistance *= crouchPenalty;
	}

	// if this is a "jump" area, add penalty
	if (pTo->m_attributeFlags & NAV_MESH_JUMP)
	{
		const float jumpPenalty = 5.0f;
		flDistance *= jumpPenalty;
	}

	// test flags added by me: (they should make sense based on attribute names...)
	if (pTo->m_attributeFlags & NAV_MESH_WALK)
	{
		const float flPenalty = 5.0f;
		flDistance *= flPenalty;
	}
	if (pTo->m_attributeFlags & NAV_MESH_AVOID)
	{
		const float flPenalty = 12.5f;
		flDistance *= flPenalty;
	}

	return pFrom->m_flTotalCostSoFar + flDistance;
}

void CWalkbot::init_path_finding()
{
	//Resize vectors if needed...
	//if (m_pOpenList.size() < m_iAreasArraySize)
	//	m_pOpenList.resize(m_iAreasArraySize);
	//if (m_pClosedList.size() < m_iAreasArraySize)
	//	m_pClosedList.resize(m_iAreasArraySize);
	if (m_pOpenList.capacity() < m_iAreasArraySize)
		m_pOpenList.reserve(m_iAreasArraySize);
	if (m_pClosedList.capacity() < m_iAreasArraySize)
		m_pClosedList.reserve(m_iAreasArraySize);

	m_pOpenList.clear();
	m_pClosedList.clear();

	for (unsigned int i = 0; i < m_navfile.m_areas.size(); i++)
	{
		CNavArea* pArea = &m_navfile.m_areas[i];

		//Reset pathfinding values
		pArea->InitPathfindingVars();
	}
}

template <typename T>
bool CWalkbot::ListContainsElement(std::vector<T> pVector, T pElement)
{
	return std::find(pVector.begin(), pVector.end(), pElement) != pVector.end();
}

bool CWalkbot::find_path(CNavArea* pSource, CNavArea* pTarget)
{
	m_pOpenList.push_back(pSource);		//Start with source
	pSource->m_flTotalCostSoFar = 0.f;

	while (true)
	{
		//We ran out of checking fields.
		if (m_pOpenList.size() == 0)
		{
			return false;
		}

		//Pick field with smallest cost so far
		CNavArea*	pBestSoFar = nullptr;
		float		flBestCostSoFar = FLT_MAX;
		int			iBestAreaIndex = -1;
		for (unsigned int i = 0; i < m_pOpenList.size(); i++)
		{
			CNavArea* pArea = m_pOpenList[i];
			if (pArea->m_flTotalCostSoFar < flBestCostSoFar)
			{
				flBestCostSoFar = pArea->m_flTotalCostSoFar;
				pBestSoFar = pArea;
				iBestAreaIndex = i;
			}
		}

		//Assert we found smth
		if (pBestSoFar == nullptr || iBestAreaIndex == -1)
		{
			return false;
		}

		//Maybe we're on golden node already?
		if (pBestSoFar->m_id == pTarget->m_id)
		{
			//No parent? We fucked up smth...
			if (!pBestSoFar->m_pParent)
			{
				return false;
			}

			return true;
		}

		//For each connected node calculate move cost and add it there.
		for (unsigned int i = 0; i < pBestSoFar->m_connect.size(); i++)
		{
			const NavConnect* pConnect = &pBestSoFar->m_connect[i];
			if (!pConnect)
			{
				continue;
			}
			CNavArea* pArea = this->GetAreaByID(pConnect->id);
			if (!pArea)
			{
				continue;
			}

			//Check if we have element on closed list, if so then ignore it.
			if (ListContainsElement(m_pClosedList, pArea))
				continue;

			//Is on openlist maybe?
			if (ListContainsElement(m_pOpenList, pArea))
			{
				//We have looked at this node before, maybe current path is better?
				float flCurrentCost = this->GetMoveCost(pBestSoFar, pArea);

				if (pArea->m_flTotalCostSoFar > flCurrentCost)
				{
					pArea->m_flTotalCostSoFar = flCurrentCost;
					pArea->m_pParent = pBestSoFar;
				}
			}
			else
			{
				//We're dealing with new node
				pArea->m_flTotalCostSoFar = this->GetMoveCost(pBestSoFar, pArea);
				pArea->m_pParent = pBestSoFar;

				m_pOpenList.push_back(pArea);
			}
		}

		//Remove from open list
		m_pOpenList.erase(m_pOpenList.begin() + iBestAreaIndex);

		//Push to closed list
		m_pClosedList.push_back(pBestSoFar);
	}

	return false;
}

//ToDo: fix and call on unload, currently it's ignored :(
void CWalkbot::OnUnload()
{
	//Cleanup allocated memory, etc.
	free(m_NavAreas);
}

bool INavFile::Load(std::ifstream &ifs)
{
	static std::string strFunctionName{ "INavFile::Load" };

	ifs.read((char *)&this->m_magic, sizeof(this->m_magic));

	if (this->m_magic != 0xFEEDFACE)
	{
		printf("[NavParse] Failed to verify file against m_magic.\n");
		return false;
	}

	ifs.read((char *)&this->m_version, sizeof(this->m_version));

	if (this->m_version != 16)
	{
		printf("[NavParse] Nav file version mismatch!\n");
		return false;
	}

	ifs.read((char *)&this->m_subVersion, sizeof(this->m_subVersion));
	ifs.read((char *)&this->m_saveBspSize, sizeof(this->m_saveBspSize));
	ifs.read((char *)&this->m_isAnalyzed, sizeof(this->m_isAnalyzed));

	ifs.read((char *)&this->m_usPlaceCount, sizeof(this->m_usPlaceCount));

	for (unsigned short us{ 0 }; us < this->m_usPlaceCount; ++us)
	{
		unsigned short usLength{ 0 };
		ifs.read((char *)&usLength, sizeof(usLength));

		char *buffer = new char[usLength];
		ifs.read(buffer, usLength);
		std::string str(buffer);
		this->m_vStrPlaceNames.push_back(str);
	}

	ifs.read((char *)&this->m_hasUnnamedAreas, sizeof(this->m_hasUnnamedAreas));
	ifs.read((char *)&this->m_uiAreaCount, sizeof(this->m_uiAreaCount));

	for (unsigned int ui{ 0 }; ui < this->m_uiAreaCount; ++ui)
	{
		CNavArea area{};
		area.LoadFromFile(ifs);
		this->m_areas.push_back(area);
	}

	return true;
}

// Initializes the class
bool INavFile::Initialize(void)
{
	this->RestoreDefaults();

	return true;
}

// Restores default values
void INavFile::RestoreDefaults(void)
{
	m_magic = 0;
	m_version = 0;
	m_subVersion = 0;
	m_saveBspSize = 0;
	m_isAnalyzed = (unsigned char)0x00;
	m_usPlaceCount = 0;
	m_vStrPlaceNames.clear();
	m_hasUnnamedAreas = (unsigned char)0x00;
	m_areas.clear();
	m_uiAreaCount = 0;
}

// Default constructor
INavFile::INavFile(void)
{
	RestoreDefaults();
}

// Destructor
INavFile::~INavFile(void)
{

}

const CNavArea* INavFile::GetNavAreaByID(int id) const
{
	for (unsigned int i = 0; i < this->m_areas.size(); i++)
	{
		const CNavArea* pArea = &this->m_areas[i];
		if (!pArea)
			continue;
		if (pArea->m_id == id)
			return pArea;
	}

	return nullptr;
}

// Default constructor
CNavArea::CNavArea(void)
{

}

// Constructor
CNavArea::CNavArea(std::ifstream &ifs)
{
	LoadFromFile(ifs);
}

// Calculates the height of the area at the given x, z coordinates
// @return Height at the given point
float CNavArea::GetZ(const Vector &pos) const
{
	if (m_invDxCorners == 0.0f || m_invDyCorners == 0.0f)
		return m_neZ;

	float u = (pos.x - m_nwCorner.x) * m_invDxCorners;
	float v = (pos.z - m_nwCorner.z) * m_invDyCorners;

	// clamp Z values to (x,y) volume
	u = u >= 0 ? u : 0;
	u = u >= 1 ? 1 : u;
	v = v >= 0 ? v : 0;
	v = v >= 1 ? 1 : v;

	float northZ = m_nwCorner.z + u * (m_neZ - m_nwCorner.z);
	float southZ = m_swZ + u * (m_seCorner.z - m_swZ);

	return northZ + v * (southZ - northZ);
}

//Return shortest distance squared between point and this area
float CNavArea::GetDistanceSquaredToPoint(const Vector &pos) const
{
	if (pos.x < m_nwCorner.x)
	{
		if (pos.y < m_nwCorner.y)
		{
			// position is north-west of area
			return (m_nwCorner - pos).size_sqr();
		}
		else if (pos.y > m_seCorner.y)
		{
			// position is south-west of area
			Vector d;
			d.x = m_nwCorner.x - pos.x;
			d.y = m_seCorner.y - pos.y;
			d.z = m_swZ - pos.z;
			return d.size_sqr();
		}
		else
		{
			// position is west of area
			float d = m_nwCorner.x - pos.x;
			return d * d;
		}
	}
	else if (pos.x > m_seCorner.x)
	{
		if (pos.y < m_nwCorner.y)
		{
			// position is north-east of area
			Vector d;
			d.x = m_seCorner.x - pos.x;
			d.y = m_nwCorner.y - pos.y;
			d.z = m_neZ - pos.z;
			return d.size_sqr();
		}
		else if (pos.y > m_seCorner.y)
		{
			// position is south-east of area
			return (m_seCorner - pos).size_sqr();
		}
		else
		{
			// position is east of area
			float d = pos.x - m_seCorner.x;
			return d * d;
		}
	}
	else if (pos.y < m_nwCorner.y)
	{
		// position is north of area
		float d = m_nwCorner.y - pos.y;
		return d * d;
	}
	else if (pos.y > m_seCorner.y)
	{
		// position is south of area
		float d = pos.y - m_seCorner.y;
		return d * d;
	}
	else
	{
		// position is inside of 2D extent of area - find delta Z
		float z = GetZ(pos);
		float d = z - pos.z;
		return d * d;
	}
}

// Check if the given point is overlapping the area
// @return True if 'pos' is within 2D extents of area.
bool CNavArea::IsOverlapping(const Vector &vecPos, float flTolerance) const
{
	if (vecPos.x + flTolerance < this->m_nwCorner.x)
		return false;

	if (vecPos.x - flTolerance > this->m_seCorner.x)
		return false;

	if (vecPos.y + flTolerance < this->m_nwCorner.y)
		return false;

	if (vecPos.y - flTolerance > this->m_seCorner.y)
		return false;

	return true;
}

// Calculates whether or not the given point is inside the area
// @return True if the point is inside the area
bool CNavArea::Contains(const Vector &vecPoint) const
{
	if (!this->IsOverlapping(vecPoint))
	{
		return false;
	}
	if ((GetZ(vecPoint) - 18.0f) > vecPoint.z)
	{
		return false;
	}
	return true;
}

//Big credits to Cre3per (lukas96s@web.de) for loading nav meshes

// Loads the area from a file stream
// @return True on success
bool CNavArea::LoadFromFile(std::ifstream &ifs)
{
	ifs.read((char *)&this->m_id, sizeof(this->m_id));
	ifs.read((char *)&this->m_attributeFlags, sizeof(this->m_attributeFlags));
	ifs.read((char *)&this->m_nwCorner, sizeof(this->m_nwCorner));
	ifs.read((char *)&this->m_seCorner, sizeof(this->m_seCorner));

	this->m_center.x = (this->m_nwCorner.x + this->m_seCorner.x) / 2.0f;
	this->m_center.y = (this->m_nwCorner.y + this->m_seCorner.y) / 2.0f;
	this->m_center.z = (this->m_nwCorner.z + this->m_seCorner.z) / 2.0f;

	if ((m_seCorner.x - m_nwCorner.x) > 0.0f && (m_seCorner.z - m_nwCorner.z) > 0.0f)
	{
		m_invDxCorners = 1.0f / (m_seCorner.x - m_nwCorner.x);
		m_invDyCorners = 1.0f / (m_seCorner.y - m_nwCorner.y);
	}
	else
	{
		m_invDxCorners = m_invDyCorners = 0.0f;
	}

	ifs.read((char *)&this->m_neZ, sizeof(this->m_neZ));
	ifs.read((char *)&this->m_swZ, sizeof(this->m_swZ));

	for (size_t s{ 0 }; s < NavDirType::NUM_DIRECTIONS; ++s)
	{
		// Number of connections in the current direction
		unsigned int uiCount{ 0 };
		ifs.read((char *)&uiCount, sizeof(uiCount));

		for (unsigned int ui{ 0 }; ui < uiCount; ++ui)
		{
			NavConnect nc{};
			ifs.read((char *)&nc.id, sizeof(nc.id));

			if (nc.id == this->m_id)
			{
				continue;
			}

			this->m_connect.push_back(nc);
		}
	}

	unsigned char hidingSpotCount{ (unsigned char)0x00 };
	ifs.read((char *)&hidingSpotCount, sizeof(hidingSpotCount));

	for (unsigned char c{ (unsigned char)0x00 }; c < hidingSpotCount; ++c)
	{
		HidingSpot hs;
		ifs.read((char *)&hs.m_id, sizeof(hs.m_id));
		ifs.read((char *)&hs.m_pos, sizeof(hs.m_pos));
		ifs.read((char *)&hs.m_flags, sizeof(hs.m_flags));
		this->m_hidingSpots.push_back(hs);
	}

	unsigned int uiEncounterSpotCount{ 0 };
	ifs.read((char *)&uiEncounterSpotCount, sizeof(uiEncounterSpotCount));

	for (unsigned int ui{ 0 }; ui < uiEncounterSpotCount; ++ui)
	{
		SpotEncounter se;

		ifs.read((char *)&se.from.id, sizeof(se.from.id));
		ifs.read((char *)&se.fromDir, sizeof(unsigned char));

		ifs.read((char *)&se.to.id, sizeof(se.to.id));
		ifs.read((char *)&se.toDir, sizeof(unsigned char));

		// Spots along this path
		unsigned char spotCount{ (unsigned char)0x00 };
		ifs.read((char *)&spotCount, sizeof(spotCount));

		for (unsigned char c{ (unsigned char)0x00 }; c < spotCount; ++c)
		{
			SpotOrder order{};
			ifs.read((char *)&order.id, sizeof(order.id));
			ifs.read((char *)&order.t, sizeof(unsigned char));
			se.spots.push_back(order);
		}

		this->m_spotEncounters.push_back(se);
	}

	// PlaceDirectory::IndexType entry = fileBuffer.GetUnsignedShort();
	ifs.read((char *)&this->entry, sizeof(this->entry));

	for (size_t s{ 0 }; s < CNavLadder::NUM_LADDER_DIRECTIONS; ++s)
	{
		unsigned int uiCount{ 0 };
		ifs.read((char *)&uiCount, sizeof(uiCount));

		for (unsigned int ui{ 0 }; ui < uiCount; ++ui)
		{
			bool bSkip{ false };
			NavLadderConnect connect{};

			ifs.read((char *)&connect.id, sizeof(connect.id));

			for (const NavLadderConnect &ladder : this->m_ladder[s])
			{
				if (ladder.id == connect.id)
				{
					bSkip = true;
					break;
				}
			}

			if (bSkip)
			{
				continue;
			}

			this->m_ladder[s].push_back(connect);
		}
	}

	for (size_t s{ 0 }; s < MAX_NAV_TEAMS; ++s)
	{
		ifs.read((char *)&this->m_earliestOccupyTime[s], sizeof(this->m_earliestOccupyTime[s]));
	}

	for (size_t s{ 0 }; s < NUM_CORNERS; ++s)
	{
		ifs.read((char *)&this->m_lightIntensity[s], sizeof(this->m_lightIntensity[s]));
	}

	ifs.read((char *)&this->m_uiVisibleAreaCount, sizeof(this->m_uiVisibleAreaCount));

	for (unsigned int ui{ 0 }; ui < this->m_uiVisibleAreaCount; ++ui)
	{
		AreaBindInfo info;
		ifs.read((char *)&info.id, sizeof(info.id));
		ifs.read((char *)&info.attributes, sizeof(info.attributes));
		this->m_potentiallyVisibleAreas.push_back(info);
	}

	ifs.read((char *)&this->m_inheritVisibilityFrom.id, sizeof(this->m_inheritVisibilityFrom.id));

	unsigned char unknownCount{ (unsigned char)0x00 };
	ifs.read((char *)&unknownCount, sizeof(unknownCount));

	for (unsigned char c{ (unsigned char)0x00 }; c < unknownCount; ++c)
	{
		unsigned char unknown[0x0E];
		ifs.read((char *)unknown, 0x0E);
	}

	return true;
}

// Pathfinding meme
void CNavArea::InitPathfindingVars()
{
	m_pParent = (CNavArea*)INVALID_NAV_PTR;
	m_flTotalCostSoFar = FLT_MAX;
}

// This is our function to walk

void CWalkbot::WalkOnPath(c_usercmd* cmd)
{
	if (m_iCurrentPathPoint >= m_path.size() || m_iCurrentPathPoint < 0)
	{
		m_target_area = nullptr;
		return;
	}
	Vector Origin = *m_pLocal->m_vecOrigin();

	if ((Origin - m_path[m_iCurrentPathPoint]).size() < 25.f)
	{
		m_iCurrentPathPoint--;
	}
	if (m_iCurrentPathPoint > 0)
	{
		if ((Origin - m_path[m_iCurrentPathPoint - 1]).size() < 25.f)
		{
			m_iCurrentPathPoint -= 2;
		}
	}
	if (m_iCurrentPathPoint < 0)
	{
		m_target_area = nullptr;
		return;
	}

	QAngle vecAngle;

	g_maths.vector_qangles(m_path[m_iCurrentPathPoint] - m_pLocal->get_vec_eyepos(), vecAngle);

	g_maths.clamp_angle(vecAngle);

	g_maths.normalize_angle(vecAngle);

	// Only gay fags set angle and walk, real men use maths.

#if 0
	QAngle delta = g_maths.calcute_delta(pCmd->viewangles, vecAngle, g_weebwarecfg.misc_cfg.misc_ai_legitfactor);

	pCmd->viewangles.y = delta.y;

	g_weebware.g_engine->set_view_angles(pCmd->viewangles);


	g_Walkbot.m_bIsWalking = true;
#endif

	if (m_CurrentMode == ai_movement::defuse_bomb) {

		// We are on the bomb.
		if (m_target_area == m_pCurrentArea) {

			// stop and defuse
			cmd->forwardmove = 0;
			cmd->sidemove = 0;

			// calculate bomb angle

		}
	}

	Vector vmove(cmd->forwardmove, cmd->sidemove, cmd->upmove);

	float flSpeed = sqrt(vmove.x * vmove.x + vmove.y * vmove.y), flYaw;

	Vector vMove2;
	g_maths.vector_qangles(vmove, vMove2);

	flYaw = DEG2RAD(cmd->viewangles.y - vecAngle.y + vMove2.y);

	cmd->forwardmove = cos(flYaw) * flSpeed;
	cmd->sidemove = sin(flYaw) * flSpeed;

	if (cmd->viewangles.x < -90.f || cmd->viewangles.x > 90.f)
		cmd->forwardmove = -cmd->forwardmove;

	auto m_pCurrentArea = GetNavArea(*m_pLocal->m_vecOrigin());

	g_Walkbot.m_iswalking = true;
}

