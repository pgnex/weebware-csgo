#pragma once

struct s_player_info
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
	char guid[33];
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};