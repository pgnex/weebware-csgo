#pragma once
#include "Header.h"


class c_backtrack {
public:

	struct c_accuracy_records
	{
		float m_simulation_time = -1;
		Vector m_head = Vector(0, 0, 0);
		Vector m_w2s_head = Vector(0, 0, 0);
		bool m_has_w2s = false;
		int record_tick = -1;
		bool m_best_record = false;
		bool visible;

		Vector parent[125] = { Vector(0,0,0) };
		Vector child[125] = { Vector(0,0,0) };
		int bonecount;
		int index = 0;
	};

	c_accuracy_records m_best_record;

	std::vector<c_accuracy_records> accuracy_records;
	c_base_entity* m_local;
	void accuracy_boost(c_usercmd* cmd);
	void clear_all_records();
	bool is_valid_record(c_accuracy_records record);

private:
	c_accuracy_records create_record(c_base_entity* entity, c_usercmd* cmd);
};

extern c_backtrack g_backtrack;
