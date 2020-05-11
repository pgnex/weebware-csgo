#pragma once
#ifndef HEADERS
#define HEADERS
#pragma warning(disable : 4244)
#pragma warning(disable : 4227)
#pragma warning(disable : 4172)
#pragma warning(disable : 4307)
#pragma warning(disable : 4996)

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <cmath>
#include <time.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <Psapi.h>
#include <filesystem>
#include <memory>
#include <process.h>  
#include <unordered_map>
#include <string>
#include <fstream>
#include <ostream>
#include <streambuf>
#include <iterator>
#include <string.h>
#include <cstddef>
#include <cstdlib>
#include <d3d9.h>
#include <WinUser.h>
#include <C:\\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "Trampoline.h"
#include "utils.h"
#include "imgui\imgui.h"
#include "imgui\dx9\imgui_dx9.h"
#include "virtual_function.h"
#include "netvars.h"
#include "maths.h"
#include "c_glow_object_manager.h"
#include "imdlcache.h"
#include "configuration.h"
#include "c_engine_client.h"
#include "vector.h"
#include "player_info.h"
#include "net_channel.h"
#include "key_values.h"
#include "i_game_event_manager.h"
#include "frame_stage.h"
#include "c_entities.h"
#include "c_misc_sdk.h"
#include "c_input.h"
#include "skindefs.h"
#include "c_rec.h"
#include "c_client.h"
#include "c_surface.h"
#include "move_helper.h"
#include "trace.h"
#include "backtrack.h"
#include "hook_funcs.h"
#include "shared.h"
#include "authentication.h"
#include "navmesh.h"
#include "Ai.h"
#include "nightmode.h"
#include "knife_proxy_hook.h"
#include "glovechanger.h"
#include "weebware_config.h"
#include "networking.h"
#include "raleway_font.h"
#include "icon_font.h"
#include "visuals_shared.h"
#include "shared_funcs.h"



// custom include files
class c_weebware;
extern c_weebware g_weebware;


#endif
