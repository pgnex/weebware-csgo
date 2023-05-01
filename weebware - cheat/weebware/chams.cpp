#include "Header.h"
#include "shared.h"
#include "hook_funcs.h"
#include "chams.h"
#include <iostream>

// https://github.com/search?q=repo%3Arollraw%2Fqo0-base%20CKeyValues&type=code

void chams::utils::init_key_vals(KeyValues* keyValues, char* name) {
  //static DWORD keyval_addr = 0;

  //if ( !keyval_addr )keyval_addr = g_weebware.pattern_scan( "client.dll", "68 ?? ?? ?? ?? 8B C8 E8 ?? ?? ?? ?? 89 45 FC EB 07 C7 45 ?? ?? ?? ?? ?? 8B 03 56" ) + 7;

  //static DWORD dwFunction = 0;

  //if ( !dwFunction ) dwFunction = keyval_addr + *reinterpret_cast< PDWORD_PTR >( keyval_addr + 1 ) + 5;

  //if ( !dwFunction ) {
  //	return;
  //}
  //__asm
  //{
  //	push name
  //	mov ecx, keyValues
  //	call dwFunction
  //}
  // using init_key_values_t = void(__thiscall*)(void*, const char*);
  using CKeyValuesConstructorFn = void(__thiscall*)(void*, const char*, void*, void*);
  static CKeyValuesConstructorFn init_key_values_ex = nullptr;

  if (!init_key_values_ex)
    init_key_values_ex = (CKeyValuesConstructorFn)g_weebware.pattern_scan("client.dll", "55 8B EC 56 8B F1 33 C0 8B 4D 0C 81"); // 55 8B EC 51 33 C0 C7 45

  init_key_values_ex(keyValues, name, 0, 0);

}

void chams::utils::load_from_buf(KeyValues* keyValues, char const* resourceName, const char* pBuffer, class IBaseFileSystem* pFileSystem, const char* pPathID, void* pUnknown, void* uu) {
  //static  DWORD dwFunction = 0;

  //if ( !dwFunction ) dwFunction = g_weebware.pattern_scan( "client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89" );

  //if ( !dwFunction ) {
  //	return;
  //}

  //__asm
  //{
  //	push uu
  //	push pUnknown
  //	push pPathID
  //	push pFileSystem
  //	push pBuffer
  //	push resourceName
  //	mov ecx, keyValues
  //	call dwFunction
  //}

  static const auto key_values_load_from_buffer = g_weebware.pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89");
  return reinterpret_cast<void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*)>(key_values_load_from_buffer)(keyValues, resourceName, pBuffer, pFileSystem, pPathID, pUnknown, uu);
}

imaterial* chams::utils::borrow_mat(custom_mats type) {
  const char* material_list[] = { "", "", "", "debug/debugdrawflat", "models/inventory_items/cologne_prediction/cologne_prediction_glass", "models/inventory_items/trophy_majors/crystal_clear", "models/inventory_items/trophy_majors/gold", "models/inventory_items/trophy_majors/crystal_blue" };

  // TEXTURE_GROUP_MODEL : TEXTURE_GROUP_OTHER
  imaterial* mat = g_weebware.g_mat_sys->find_material(material_list[type], TEXTURE_GROUP_MODEL);
  if (!mat || mat->iserrormaterial())
    return g_weebware.g_mat_sys->find_material("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_MODEL);

  mat->incrementreferencecount();
  return mat;
}

imaterial* chams::utils::create_default() {


  std::stringstream s;

  s << "\"VertexLitGeneric\" {\n\n\t";
  s << "\"basetexture\" \"vgui/white_additive\"\n\t";
  s << "\"$additive\" \"0\"\n\t";
  //s << "\"$envmap\" \"models/effects/cube_white\"\n\t";
  //s << "\"$envmaptint\"" << " \"[" << 1 << " " << 1 << " " << 1 << "]\"\n\t";
  s << "\"$ignorez\" \"0\"\n\t";
  //s << "\"$envmapfresnelminmaxexp\" \"[0 1 2]\"\n\t";
  s << "\"$alpha\" \"1\"\n";
  s << "}";


  KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
  init_key_vals(keyValues, "VertexLitGeneric");
  load_from_buf(keyValues, "default.vmt", s.str().c_str(), 0, 0, 0, 0);

  imaterial* created_mat = g_weebware.g_mat_sys->create_mat("default.vmt", keyValues);

  if (!created_mat || created_mat->iserrormaterial())
    return NULL;

  return created_mat;
}

imaterial* chams::utils::create_ignorez() {


  std::stringstream s;

  s << "\"VertexLitGeneric\" {\n\n\t";
  s << "\"basetexture\" \"vgui/white_additive\"\n\t";
  s << "\"$additive\" \"0\"\n\t";
  //s << "\"$envmap\" \"models/effects/cube_white\"\n\t";
  //s << "\"$envmaptint\"" << " \"[" << 1 << " " << 1 << " " << 1 << "]\"\n\t";
  s << "\"$ignorez\" \"1\"\n\t";
  //s << "\"$envmapfresnelminmaxexp\" \"[0 1 2]\"\n\t";
  s << "\"$alpha\" \"1\"\n";
  s << "}";


  KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
  init_key_vals(keyValues, "VertexLitGeneric");
  load_from_buf(keyValues, "ignorez.vmt", s.str().c_str(), 0, 0, 0, 0);
  imaterial* created_mat = g_weebware.g_mat_sys->create_mat("ignorez.vmt", keyValues);

  if (!created_mat || created_mat->iserrormaterial())
    return NULL;

  return created_mat;
}

imaterial* chams::utils::create_glow() {

  std::stringstream s;

  s << "\"VertexLitGeneric\" {\n\n\t";
  s << "\"basetexture\" \"vgui/white\"\n\t";
  s << "\"$additive\" \"0\"\n\t";
  s << "\"$envmap\" \"models/effects/cube_white\"\n\t";
  s << "\"$envmaptint\"" << " \"[" << 1 << " " << 1 << " " << 1 << "]\"\n\t";
  s << "\"$envmapfresnel\" \"1\"\n\t";
  s << "\"$envmapfresnelminmaxexp\" \"[0 1 2]\"\n\t";
  s << "\"$alpha\" \"0.8\"\n";
  s << "}";


  KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
  init_key_vals(keyValues, "VertexLitGeneric");
  load_from_buf(keyValues, "glow.vmt", s.str().c_str(), 0, 0, 0, 0);
  imaterial* created_mat = g_weebware.g_mat_sys->create_mat("glow.vmt", keyValues);

  return created_mat;
}

void chams::se::desync_chams() {
  static imaterial* mat = utils::create_default();

  // get localplayer
  auto local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

  if (!local)
    return;

  c_color col = c_color(g_weebwarecfg.visuals_desync_cham_col);
  float col_blend[4] = { col.r / 255, col.g / 255, col.b / 255, col.a / 255 };

  Vector orig_angle = local->m_angEyeAngles();
  local->set_angles(Vector(0, g_weebware.fake_angle.y, 0));

  g_weebware.g_render_view->SetColorModulation(col_blend);
  g_weebware.g_render_view->SetBlend(col.a / 255.f);
  g_weebware.g_model_render->forcedmaterialoverride(mat);
  local->draw_model(1, 255);

  local->set_angles(Vector(0, orig_angle.y, 0));
  g_weebware.g_model_render->forcedmaterialoverride(nullptr);
}

void chams::se::local_chams() {
  static imaterial* mat = utils::create_default();

  // get localplayer
  auto local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

  if (!local)
    return;

  c_color col = c_color(g_weebwarecfg.visuals_local_chams_col);
  float col_blend[4] = { col.r / 255, col.g / 255, col.b / 255, col.a / 255 };

  g_weebware.g_render_view->SetColorModulation(col_blend);
  g_weebware.g_render_view->SetBlend(col.a / 255.f);
  g_weebware.g_model_render->forcedmaterialoverride(mat);
  local->draw_model(1, 255);
  g_weebware.g_model_render->forcedmaterialoverride(nullptr);
}

void chams::se::player_chams() {

  if (g_weebwarecfg.visuals_chams > custom_mats::max)
    g_weebwarecfg.visuals_chams = 1;

  // setting up mats
  static bool init = false;
  static imaterial* mat_list[custom_mats::max];
  if (!init) {
    // grab pre-generated materials
    for (auto i = 1; i < custom_mats::max; i++) {
      mat_list[i] = utils::borrow_mat(static_cast<custom_mats>(i));
    }
    mat_list[custom_mats::plain] = utils::create_default();
    mat_list[custom_mats::glow_cham] = utils::create_glow();
    init = true;
  }

  // last null check,,
  if (!mat_list[g_weebwarecfg.visuals_chams])
    return;

  if (mat_list[g_weebwarecfg.visuals_chams]->iserrormaterial())
    return;

  // get localplayer
  auto local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

  if (!local)
    return;

  for (int i = 1; i <= g_weebware.g_global_vars->maxclients; i++) {
    auto player = g_weebware.g_entlist->getcliententity(i);

    // nullptr check localplayer
    if (!player)
      continue;

    // make sure ent is a valid player
    if (!player->is_valid_player())
      continue;

    bool smoke_visible = player->trace_from_smoke(*local->m_vecOrigin());


    // if player is behind smoke and we dont have xqz on, go next
    if (smoke_visible && !g_weebwarecfg.visuals_chams_xqz) {
      continue;
    }

    // setup our colors here..
    c_color col;

    if (smoke_visible && g_weebwarecfg.visuals_chams_xqz) {
      col = c_color(player->m_iTeamNum() == local->m_iTeamNum() ? g_weebwarecfg.visuals_chams_team_col_xqz : g_weebwarecfg.visuals_chams_col_xqz);
    }
    else {
      col = c_color(player->m_iTeamNum() == local->m_iTeamNum() ? g_weebwarecfg.visuals_chams_team_col : g_weebwarecfg.visuals_chams_col);
    }

    float col_blend[4] = { col.r / 255.f, col.g / 255.f, col.b / 255.f, 1.f };

    if (g_weebwarecfg.visuals_chams_xqz) {
      c_color xqz_col = c_color(player->m_iTeamNum() == local->m_iTeamNum() ? g_weebwarecfg.visuals_chams_team_col_xqz : g_weebwarecfg.visuals_chams_col_xqz);
      float xqz_col_blend[4] = { xqz_col.r / 255.f, xqz_col.g / 255.f, xqz_col.b / 255.f, 1.f };
      g_weebware.g_render_view->SetBlend(xqz_col.a / 255.f);
      g_weebware.g_render_view->SetColorModulation(xqz_col_blend);
    }

    //if (g_weebwarecfg.misc_legit_aa_enabled && player == local) {
    //	player->set_angles(Vector(0, g_weebware.fake_angle.y, 0));
    //	g_weebware.g_render_view->SetBlend(0.5f);
    //	player->draw_model(1, 255);
    //	g_weebware.g_render_view->SetBlend(1.f);
    //	player->set_angles(Vector(0, g_weebware.real_angle.y, 0));
    //}

    // skip if team chams is off and entity is teammate
    if ((player->m_iTeamNum() == local->m_iTeamNum()) && !g_weebwarecfg.visuals_chams_render_team)
      continue;


    if (!mat_list[g_weebwarecfg.visuals_chams])
      continue;


    if (mat_list[g_weebwarecfg.visuals_chams]->iserrormaterial())
      continue;

    static imaterial* plain_ignorez = utils::create_ignorez();

    if (local->m_iTeamNum() == player->m_iTeamNum()) {
      if (g_weebwarecfg.visuals_chams_render_team) {
        if (g_weebwarecfg.visuals_chams_xqz) {
          g_weebware.g_model_render->forcedmaterialoverride(plain_ignorez);
          player->draw_model(1, 255);
          g_weebware.g_model_render->forcedmaterialoverride(nullptr);
        }

        // Set material info.
        g_weebware.g_render_view->SetBlend(col.a / 255.f);
        g_weebware.g_render_view->SetColorModulation(col_blend);
        g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams]);
        player->draw_model(1, 255);
        g_weebware.g_model_render->forcedmaterialoverride(nullptr);
      }
    }
    else {
      if (g_weebwarecfg.visuals_chams_xqz) {
        g_weebware.g_model_render->forcedmaterialoverride(plain_ignorez);
        player->draw_model(1, 255);
        g_weebware.g_model_render->forcedmaterialoverride(nullptr);
      }

      if (g_weebwarecfg.visuals_chams == custom_mats::glow_cham) {

        g_weebware.g_render_view->SetBlend(col.a / 255.f);
        g_weebware.g_render_view->SetColorModulation(col_blend);

        c_color glow_clr = g_weebwarecfg.visuals_chams_glow_col;

        mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, false);
        g_weebware.g_model_render->forcedmaterialoverride(mat_list[custom_mats::plain]);
        player->draw_model(1, 255);

        bool found = false;
        imaterialvar* pVar = mat_list[g_weebwarecfg.visuals_chams]->FindVar("$envmaptint", &found);
        if (found)
          (*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, glow_clr.r / 255.f, glow_clr.g / 255.f, glow_clr.b / 255.f);

        mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, false);
        g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams]);
        player->draw_model(1, 255);
        g_weebware.g_model_render->forcedmaterialoverride(nullptr);
      }
      else {
        // Set material info.
        g_weebware.g_render_view->SetBlend(col.a / 255.f);
        g_weebware.g_render_view->SetColorModulation(col_blend);

        mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, false);
        g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams]);
        player->draw_model(1, 255);
        g_weebware.g_model_render->forcedmaterialoverride(nullptr);
      }
    }
  }
}

void chams::dme::player_chams(void* thisptr, void* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld, hooks::drawmodelexecute o_dme) {

  if (!g_weebwarecfg.visuals_chams)
    return;

  // get local
  c_base_entity* local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());
  if (!local)
    return;

  c_base_entity* model_ent = g_weebware.g_entlist->getcliententity(pInfo.entity_index);
  if (!model_ent || !model_ent->is_valid_player())
    return;

  if ((local->m_iTeamNum() == model_ent->m_iTeamNum()) && !g_weebwarecfg.visuals_chams_render_team)
    return;

  // setting up mats
  static bool init = false;
  static imaterial* mat_list[custom_mats::max];
  if (!init) {
    // grab pre-generated materials
    for (auto i = 1; i < custom_mats::max; i++) {
      mat_list[i] = utils::borrow_mat(static_cast<custom_mats>(i));
    }
    mat_list[custom_mats::plain] = utils::create_default();
    mat_list[custom_mats::glow_cham] = utils::create_glow();
    init = true;
  }

  // last null check,,
  if (!mat_list[g_weebwarecfg.visuals_chams])
    return;

  if (mat_list[g_weebwarecfg.visuals_chams]->iserrormaterial())
    return;

  // setup colors
  c_color col, xqz_col;
  col = c_color(model_ent->m_iTeamNum() == local->m_iTeamNum() ? g_weebwarecfg.visuals_chams_team_col : g_weebwarecfg.visuals_chams_col);
  xqz_col = c_color(model_ent->m_iTeamNum() == local->m_iTeamNum() ? g_weebwarecfg.visuals_chams_team_col_xqz : g_weebwarecfg.visuals_chams_col_xqz);

  float col_blend[4] = { col.r / 255.f, col.g / 255.f, col.b / 255.f, 1.f };
  float xqz_col_blend[4] = { xqz_col.r / 255.f, xqz_col.g / 255.f, xqz_col.b / 255.f, 1.f };

  // do chams
  switch (g_weebwarecfg.visuals_chams) {
  case custom_mats::plain: {
    if (g_weebwarecfg.visuals_chams_xqz) {
      static imaterial* plain_ignorez = utils::create_ignorez();
      g_weebware.g_render_view->SetBlend(xqz_col.a / 255.f);
      g_weebware.g_render_view->SetColorModulation(xqz_col_blend);
      g_weebware.g_model_render->forcedmaterialoverride(plain_ignorez);
      // orig..
      o_dme(g_weebware.g_model_render, ctx, state, pInfo, pCustomBoneToWorld);
    }
    g_weebware.g_render_view->SetBlend(col.a / 255.f);
    g_weebware.g_render_view->SetColorModulation(col_blend);
    g_weebware.g_model_render->forcedmaterialoverride(mat_list[custom_mats::plain]);
    return;
  }
  case custom_mats::glow_cham: {
    c_color glow_clr = g_weebwarecfg.visuals_chams_glow_col;

    g_weebware.g_render_view->SetBlend(col.a / 255.f);
    g_weebware.g_render_view->SetColorModulation(col_blend);
    mat_list[custom_mats::plain]->setmaterialvarflag(material_var_ignorez, false);
    g_weebware.g_model_render->forcedmaterialoverride(mat_list[custom_mats::plain]);

    static bool found = false;
    static imaterialvar* pVar = mat_list[g_weebwarecfg.visuals_chams]->FindVar("$envmaptint", &found);
    if (found)
      (*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, glow_clr.r / 255.f, glow_clr.g / 255.f, glow_clr.b / 255.f);

    if (g_weebwarecfg.visuals_chams_xqz) {
      g_weebware.g_render_view->SetBlend(xqz_col.a / 255.f);
      g_weebware.g_render_view->SetColorModulation(xqz_col_blend);
      mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, true);
      g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams]);
    }
    return;
  }
  default: {
    if (g_weebwarecfg.visuals_chams_xqz) {
      g_weebware.g_render_view->SetBlend(xqz_col.a / 255.f);
      g_weebware.g_render_view->SetColorModulation(xqz_col_blend);
      mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, true);
      g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams]);
      // orig..
      o_dme(g_weebware.g_model_render, ctx, state, pInfo, pCustomBoneToWorld);
    }
    g_weebware.g_render_view->SetBlend(col.a / 255.f);
    g_weebware.g_render_view->SetColorModulation(col_blend);
    mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, false);
    g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams]);
    return;
  }
  }
}


void chams::dme::hand_chams(const modelrenderinfo_t& pInfo) {

}