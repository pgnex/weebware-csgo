#pragma once

class c_move_data
{
public:
	bool    m_bFirstRunOfFunctions : 1;
	bool    m_bGameCodeMovedPlayer : 1;
	int     m_nPlayerHandle;        // edict index on server, client entity handle on client=
	int     m_nImpulseCommand;      // Impulse command issued.
	Vector  m_vecViewAngles;        // Command view angles (local space)
	Vector  m_vecAbsViewAngles;     // Command view angles (world space)
	int     m_nButtons;             // Attack buttons.
	int     m_nOldButtons;          // From host_client->oldbuttons;
	float   m_flForwardMove;
	float   m_flSideMove;
	float   m_flUpMove;
	float   m_flMaxSpeed;
	float   m_flClientMaxSpeed;
	Vector  m_vecVelocity;          // edict::velocity        // Current movement direction.
	Vector  m_vecAngles;            // edict::angles
	Vector  m_vecOldAngles;
	float   m_outStepHeight;        // how much you climbed this move
	Vector  m_outWishVel;           // This is where you tried 
	Vector  m_outJumpVel;           // This is your jump velocity
	Vector  m_vecConstraintCenter;
	float   m_flConstraintRadius;
	float   m_flConstraintWidth;
	float   m_flConstraintSpeedFactor;
	float   m_flUnknown[5];
	Vector  m_vecAbsOrigin;
};

class c_move_helper
{
public:

	bool m_bfirstrunoffunctions : 1;
	bool m_bgamecodemovedplayer : 1;
	int m_nplayerhandle; // edict index on server, client entity handle on client=
	int m_nimpulsecommand; // impulse command issued.
	QAngle m_vecviewangles; // command view angles (local space)
	QAngle m_vecabsviewangles; // command view angles (world space)
	int m_nbuttons; // attack buttons.
	int m_noldbuttons; // from host_client->oldbuttons;
	float m_flforwardmove;
	float m_flsidemove;
	float m_flupmove;
	float m_flmaxspeed;
	float m_flclientmaxspeed;
	Vector m_vecvelocity; // edict::velocity // current movement direction.
	QAngle m_vecangles; // edict::angles
	QAngle m_vecoldangles;
	float m_outstepheight; // how much you climbed this move
	Vector m_outwishvel; // this is where you tried 
	Vector m_outjumpvel; // this is your jump velocity
	Vector m_vecconstraintcenter;
	float m_flconstraintradius;
	float m_flconstraintwidth;
	float m_flconstraintspeedfactor;
	float m_flunknown[5];
	Vector m_vecabsorigin;

	virtual	void _vpad() = 0;
	virtual void sethost(c_base_entity* host) = 0;
};


class c_prediction
{
public:
	bool in_prediction()
	{
		return getvfunc<bool(__thiscall*)(void*)>(this, 14)(this);
	}

	void run_command(c_base_entity *player, c_usercmd *ucmd, c_move_helper *moveHelper)
	{
		return getvfunc<void(__thiscall*)(void*, c_base_entity*, c_usercmd*, c_move_helper*)>(this, 19)(this, player, ucmd, moveHelper);
	}

	void setup_move(c_base_entity *player, c_usercmd *ucmd, c_move_helper *moveHelper, void* pMoveData)
	{
		return getvfunc<void(__thiscall*)(void*, c_base_entity*, c_usercmd*, c_move_helper*, void*)>(this, 20)(this, player, ucmd, moveHelper, pMoveData);
	}

	void finish_move(c_base_entity *player, c_usercmd *ucmd, void* move_data_ptr)
	{
		return getvfunc<void(__thiscall*)(void*, c_base_entity*, c_usercmd*, void*)>(this, 21)(this, player, ucmd, move_data_ptr);
	}
};

class c_gamemovement
{
public:
	virtual			~c_gamemovement(void) {}

	virtual void	          processmovement(c_base_entity *pplayer, c_move_data *pmove) = 0;
	virtual void	          reset(void) = 0;
	virtual void	          starttrackpredictionerrors(c_base_entity *pplayer) = 0;
	virtual void	          finishtrackpredictionerrors(c_base_entity *pplayer) = 0;
	virtual void	          diffprint(char const *fmt, ...) = 0;
	virtual Vector const&	  getplayermins(bool ducked) const = 0;
	virtual Vector const&	  getplayermaxs(bool ducked) const = 0;
	virtual Vector const&   getplayerviewoffset(bool ducked) const = 0;
	virtual bool		        ismovingplayerstuck(void) const = 0;
	virtual c_base_entity*   getmovingplayer(void) const = 0;
	virtual void		        unblockpusher(c_base_entity *pplayer, c_base_entity *ppusher) = 0;
	virtual void            setupmovementbounds(c_move_data *pmove) = 0;
};

class c_engine_prediction
{
public:
	void begin(c_usercmd *userCMD, c_base_entity* player);

	void end(c_base_entity* player);

private:

	float old_curtime;
	float old_frametime;

	c_move_data move_data;

	int *predictionRandomSeed;
	c_base_entity *predictionPlayer;

};

extern c_engine_prediction g_engine_prediction;