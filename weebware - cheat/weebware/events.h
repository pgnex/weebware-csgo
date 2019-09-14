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
	void on_paint();
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