#include "Header.h"
#include "legit_aa.h"
#include "shared.h"

void LegitAntiAim::Run(c_usercmd* cmd) {

    if (!g_weebwarecfg.misc_legit_aa_enabled)
        return;

    c_base_entity* local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

    if (!local)
        return;

    c_basecombat_weapon* weapon = local->m_pActiveWeapon();

    if (!weapon)
        return;

    if (weapon->is_grenade())
        return;

    if (cmd->buttons & in_attack) return;
    if (cmd->buttons & in_use) return;

    int local_move_type = local->get_move_type();
    if (local_move_type == MOVETYPE_LADDER || local_move_type == MOVETYPE_NOCLIP) return;

    static int chokes = 0;
    g_weebware.send_packet = (chokes == 0) ? true : false;
    chokes++;
    if (chokes > 3)
        chokes = 0;

    float& ptr_sidemove = cmd->sidemove;

    bool command_switch = cmd->command_number % 2;
    static bool command_switch_s = false;
    if (command_switch) command_switch_s = !command_switch_s;

    static int m_last_moving;

    if (last_moving(cmd))
        m_last_moving = last_moving(cmd);

    if (command_switch_s && !m_last_moving) {
        if (cmd->buttons & in_duck)
            command_switch ? ptr_sidemove += -4.6 : ptr_sidemove += 4.6;
        else
            command_switch ? ptr_sidemove += -1.6 : ptr_sidemove += 1.6;
    }

    // packet is being sent lets not set angles..
    if (g_weebware.send_packet)
        return;

    if (command_switch_s)
        cmd->viewangles.y -= 57 * (GetKeyState(g_weebwarecfg.misc_legit_aa_side_key) ? 1 : -1);
    else
        cmd->viewangles.y += 57 * (GetKeyState(g_weebwarecfg.misc_legit_aa_side_key) ? 1 : -1);

    //if (m_last_moving && get_epoch_s() > m_last_moving + 2) {
    //    float delta = local->m_angEyeAngles().y - local->m_flLowerBodyYawTarget();
    //    if (delta >= 359 && delta <= 361 || delta >= -1 && delta <= 1) {
    //        std::cout << "need reset" << std::endl;
    //    }
    //}
}

int LegitAntiAim::last_moving(c_usercmd* cmd) {
    if ((cmd->buttons & in_moveleft) || (cmd->buttons & in_moveright) || (cmd->buttons & in_forward) || (cmd->buttons & in_back))
        return get_epoch_s();
    else
        return 0;
}