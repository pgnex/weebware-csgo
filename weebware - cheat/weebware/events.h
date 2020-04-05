#pragma once

#include "Header.h"

using eventfunc_t = void(__cdecl *)(i_game_event *);

// all of our event funcs.
namespace EventFuncs {
	void player_hurt(i_game_event *event);
	void player_death(i_game_event *event);
	void bullet_impact(i_game_event *event);
	void round_end(i_game_event* event);
	void round_start(i_game_event* event);
}

class FeatureFuncs {
public:
	// some buybot maps
	std::map<int, std::string> prim_buy_bot_map = { {M4A4AK47, "buy ak47"}, {FamasGalil, "buy famas"}, {UMP, "buy ump45"}, {P90, "buy p90"} };
	std::map<int, std::string> sec_buy_bot_map = { {Deagle, "buy deagle"}, {Elites, "buy elite"}, {Five7Tec9, "buy fn57"}, {P250, "buy p250"} };
	void buy_bot();
	void on_paint();
	void killsay(i_game_event* event);


private:
	// 	const char* primary_weapons[] = { "M4A4", "AK-47", "Famas", "Galil", "UMP-45", "P90" };
	enum buy_bot_primary {
		M4A4AK47 = 1,
		FamasGalil,
		UMP,
		P90
	};

	//	const char* secondary_weapons[] = { "None", "Desert Eagle", "Dual Berettas", "Five Seven / Tec-9", "P250" };
	enum buy_bot_secondary {
		Deagle = 1,
		Elites,
		Five7Tec9,
		P250
	};

	const std::vector<std::string> killsay_messages = {
	"Supa kawaii desu!!!!!!!! ___________________^",
	"SUPA SUPA SUPA KAWAII SASUKE-SAMA!!!!!",
	"Nyaaaaa!!! (________<) _________________;;;;;;;;;;;;;;;;",
	"I absolutely luuuv @_____@ anime <3",
	"What the desu did you just kawaii say about me, you little baka",
	"wigglez booty n' squirms",
	"please punish me licks lips nyea~",
	"notices buldge OwO what's that",
	"that's a penis UwU you towd me you wewe a giww!!",
	"Cummy desu, you are my senpai",
	"uguu desu desu kawaii neee~~~~~ <3",
	"wiww shit fuwwies aww ovew you and you wiww dwown in them",
	"AYAYA!~ >__<",
	"Nani the fuck did omae just fucking iimasu about watashi, you chiisai bitch desuka?",
	"Uhm by the way MOM They're not Chinese cartoons it's called ANIME!!"
	};

};
extern FeatureFuncs g_events_features;

// overrides funcs in IGameEventListener2 and forwards to our custom events.
class GameEvents : public i_game_event_listener {
private:
	// container for our own events.
	struct Event_t {
		std::string m_name;
		eventfunc_t m_func;
	};


	std::vector< Event_t > m_events;

	// override IGameEventListener2::GetEventDebugID.
	// need to override this for this stuff to work.
	// https://github.com/pmrowla/hl2sdk-csgo/blob/49e950f3eb820d88825f75e40f56b3e64790920a/game/shared/GameEventListener.h#L24
	int GetEventDebugID() {
		return EVENT_DEBUG_ID_INIT;
	}

	// override IGameEventListener2::FireGameEvent.
	// call into the custom event.
	void FireGameEvent(i_game_event *event) {
		// should never happen...
		if (!event)
			return;

		const std::string event_name = event->GetName();

		for (const auto &e : m_events) {
			if (e.m_name == event_name)
				e.m_func(event);
		}
	}

	// add our events by name.
	__forceinline void register_events() {
		for (const auto &e : m_events)
			g_weebware.g_game_events->AddListener(this, e.m_name.c_str(), false);
	}

public:
	// default ctor.
	__forceinline GameEvents() {

	}

	__forceinline void add_event(const std::string &str, eventfunc_t func) {
		m_events.push_back({ str, func });
	}

	// setup class.
	// add your events here.
	void init();

};

extern GameEvents g_events;