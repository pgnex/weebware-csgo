 #pragma once
#include "Header.h"

class c_legitbot
{
public:
	c_base_entity * m_local;
	c_basecombat_weapon * g_weapon;
	void create_move(c_usercmd* cmd);

	// Expose this function so accuracy boost can use it.
	Vector center_hitbox(c_base_entity* ent, int id);
	QAngle calcute_delta(QAngle src, QAngle dst, float f);

	class c_accuracy_boost
	{
	public:
		struct c_accuracy_records
		{
			float m_simulation_time = -1;
#if 0
			QAngle m_angles = QAngle(0, 0, 0);
			Vector m_abs_origin = Vector(0, 0, 0);
			Vector m_origin = Vector(0, 0, 0);
			float m_cycle = -1;
			float m_sequence = -1;
			Vector m_mins = Vector(0, 0, 0);
			Vector m_max = Vector(0, 0, 0);
#endif
			Vector m_head = Vector(0, 0, 0);
			Vector m_w2s_head = Vector(0, 0, 0);
			bool m_has_w2s = false;
			int record_tick = -1;
			bool m_best_record = false;
			// legs might be fucked, we aren't restoring animation layer.

			// Here atm for bone skeletons

			Vector parent[125] = { Vector(0,0,0) };
			Vector child[125] = { Vector(0,0,0) };
			int bonecount;
			int index = 0;
		};

		c_accuracy_records m_best_record;

		std::vector<c_accuracy_records> accuracy_records;

		void accuracy_boost(c_usercmd* cmd);

		void clear_all_records();

		bool is_valid_record(c_accuracy_records record);
	private:
		void set_abs_origins(c_base_entity* a1, const Vector& a2);
		void set_abs_angles(c_base_entity* a1, const Vector& a2);
		void invalidate_bone_cache(c_base_entity* entity);
		c_accuracy_records create_record(c_base_entity* entity, c_usercmd* cmd);
		void set_record(c_base_entity* player, c_accuracy_records record);



	};

	void triggerbot_main(c_usercmd* cmd);
	void magnet_triggerbot(c_usercmd* cmd);
	QAngle magnet_hitbox(c_base_entity* target);
	int get_config_index();
	void auto_stop(c_usercmd* cmd);
	c_base_entity* closest_target_triggerbot();


private:
	bool raytrace_hc(Vector viewAngles, float chance, c_base_entity* target, float dst);
	c_base_entity * closest_target_available();
	bool is_visible(c_base_entity* target);
	QAngle closest_hitbox(c_base_entity* target);
	QAngle rcs_scaled(QAngle original_angle, float pitch, float yaw);
	void standalone_rcs(c_usercmd* cmd);
	long get_epoch();
	double m_last_time = 0;
	bool next_attack_queued();

};

extern c_legitbot g_legitbot;
extern c_legitbot::c_accuracy_boost g_accuracy;
