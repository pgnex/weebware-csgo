#pragma once
#include "Header.h"

bool is_visible(c_base_entity* local, c_base_entity* target, int bone = 8);
int get_config_index();
long long get_epoch_ms();
long get_epoch_s();
float random_float(float flMinVal, float flMaxVal);
void random_seed(UINT Seed);
Vector get_spread(c_basecombat_weapon* weapon, int seed);