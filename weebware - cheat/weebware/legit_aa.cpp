#include "Header.h"
#include "legit_aa.h"

void LegitAntiAim::Run(c_usercmd* cmd)
{
    c_base_entity* local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

    if (!local)
        return;

    static bool fakelag_reset = false;

    if (!g_weebwarecfg.misc_legit_aa_enabled) {
        if (!fakelag_reset) {
            g_weebwarecfg.fake_lag_factor = 0;
            g_weebwarecfg.fake_lag = 0;
            fakelag_reset = true;
        }
        return;
    }

   //if (g_weebwarecfg.fake_lag_factor <= 1) g_weebwarecfg.fake_lag_factor = 1;
   //if (g_weebwarecfg.fake_lag_factor >= 6) g_weebwarecfg.fake_lag_factor = 6;
    g_weebwarecfg.fake_lag_factor = 1;
    g_weebwarecfg.fake_lag = 2;
    fakelag_reset = false;

    c_basecombat_weapon* weapon = local->m_pActiveWeapon();

    if (!weapon)
        return;

    if (weapon->is_grenade())
        return;

    if (cmd->buttons & in_attack) return;
    if (cmd->buttons & in_use) return;

    int local_move_type = local->get_move_type();
    if (local_move_type == MOVETYPE_LADDER || local_move_type == MOVETYPE_NOCLIP) return;

    float& ptr_sidemove = cmd->sidemove;

    bool command_switch = cmd->command_number % 2;
    static bool command_switch_s = false;
    if (command_switch) command_switch_s = !command_switch_s;

    if (command_switch_s && !(cmd->buttons & in_moveleft) && !(cmd->buttons & in_moveright) && !(cmd->buttons & in_forward) && !(cmd->buttons & in_back))
        if (cmd->buttons & in_duck)
            command_switch ? ptr_sidemove += -4.6 : ptr_sidemove += 4.6;
        else
            command_switch ? ptr_sidemove += -1.6 : ptr_sidemove += 1.6;

    if (!g_weebware.send_packet)
        if (command_switch_s)
            cmd->viewangles.y -= 57 * (GetKeyState(g_weebwarecfg.misc_legit_aa_side_key) ? 1 : -1);
        else
            cmd->viewangles.y += 57 * (GetKeyState(g_weebwarecfg.misc_legit_aa_side_key) ? 1 : -1);
}