#pragma once
#include "Header.h"
class c_glovechanger {
public:
	bool apply_glove_model(c_basecombat_weapon* glove) noexcept;
	bool apply_glove_skin(c_basecombat_weapon* glove, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear) noexcept;
	void run() noexcept;

    std::vector<const char*> set_glove_skin_array( );

    std::tuple<int, const char*> mt( int id, const char* name ) {
        return std::make_tuple( id, name);
    }

    std::vector<std::tuple<int, const char*>> default_gloves = { mt( 0, "Default" ) };
    std::vector<std::tuple<int, const char*>> sport_gloves = { mt( 10048, "Vice" ), mt( 10046, "Bronze Morph" ), mt( 10045, "Amphibious" ), mt( 10047, "Omega" ), mt( 10037, "Pandora's Box" ), mt( 10018, "Superconductor" ), mt( 10038, "Hedge Maze" ), mt( 10019, "Arid" ), mt( 10075, "Scarlet Shamagh" ), mt( 10076, "Nocts" ), mt( 10074, "Big Game" ), mt( 10073, "Slingshot" ) };
    std::vector<std::tuple<int, const char*>> hand_wrap_gloves = { mt( 10053, "Cobalt Skulls" ), mt( 10056, "Arboreal" ), mt( 10054, "Overprint" ), mt( 10055, "Duct Tape" ), mt( 10021, "Slaughter" ), mt( 10010, "Spruce DDPAT" ), mt( 10036, "Badlands" ), mt( 10009, "Leather" ), mt( 10083, "Constrictor" ), mt( 10081, "Desert Shamagh" ), mt( 10082, "Giraffe" ), mt( 10084, "CAUTION!" ) };
    std::vector<std::tuple<int, const char*>> specialist_gloves = { mt( 10063, "Fade" ), mt( 10061, "Crimson Web" ), mt( 10064, "Mogul" ), mt( 10062, "Buckshot" ), mt( 10033, "Crimson Kimono" ), mt( 10034, "Emerald Web" ), mt( 10035, "Foundation" ), mt( 10030, "Forest DDPAT" ), mt( 10065, "Marble Fade" ), mt( 10067, "Tiger Strike" ), mt( 10068, "Field Agent" ) };
    std::vector<std::tuple<int, const char*>> driver_gloves = { mt( 10041, "King Snake" ), mt( 10042, "Imperial Plaid" ), mt( 10043, "Overtake" ), mt( 10044, "Racing Green" ), mt( 10016, "Crimson Weave" ), mt( 10013, "Lunar Weave" ), mt( 10040, "Diamondback" ), mt( 10015, "Convoy" ), mt( 10069, "Rezan the Red" ), mt( 10071, "Queen Jaguar" ), mt( 10072, "Black Tie" ), mt( 10070, "Snow Leopard" ) };
    std::vector<std::tuple<int, const char*>> moto_gloves = { mt( 10052, "Polygon" ), mt( 10049, "POW!" ), mt( 10050, "Turtle" ), mt( 10051, "Transport" ), mt( 10026, "Spearmint" ), mt( 10024, "Eclipse" ), mt( 10027, "Boom!" ), mt( 10028, "Cool Mint" ), mt( 10080, "3rd Commando Company" ), mt( 10077, "Finish Line" ), mt( 10078, "Smoke Out" ), mt( 10079, "Blood Pressure" ) };
    std::vector<std::tuple<int, const char*>> hydra_gloves = { mt( 10060, "Case Hardened" ), mt( 10057, "Emerald" ), mt( 10059, "Rattler" ), mt( 10058, "Mangrove" ) };
    std::vector<std::tuple<int, const char*>> bloodhound_gloves = { mt( 10006, "Charred" ), mt( 10007, "Snakebite" ), mt( 10008, "Bronzed" ), mt( 10039, "Guerrilla" ) };
    std::vector<std::tuple<int, const char*>> fang_gloves = { mt( 10085, "Jade" ), mt( 10086, "Yellow-banded" ), mt( 10088, "Unhinged" ), mt( 10087, "Needle Point" ) };
    std::vector<std::vector<std::tuple<int, const char*>>> v_gloves = { default_gloves, sport_gloves, hand_wrap_gloves, specialist_gloves, driver_gloves, moto_gloves, hydra_gloves, bloodhound_gloves, fang_gloves };

};
extern c_glovechanger glove_changer;