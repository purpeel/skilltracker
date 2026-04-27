#include "player_profile.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void PlayerProfile::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_player_name", "name"), &PlayerProfile::set_player_name);
    ClassDB::bind_method(D_METHOD("get_player_name"), &PlayerProfile::get_player_name);
    
    ClassDB::bind_method(D_METHOD("set_level", "lvl"), &PlayerProfile::set_level);
    ClassDB::bind_method(D_METHOD("get_level"), &PlayerProfile::get_level);
    
    ClassDB::bind_method(D_METHOD("set_xp", "xp"), &PlayerProfile::set_xp);
    ClassDB::bind_method(D_METHOD("get_xp"), &PlayerProfile::get_xp);
    
    ClassDB::bind_method(D_METHOD("set_max_xp", "xp"), &PlayerProfile::set_max_xp);
    ClassDB::bind_method(D_METHOD("get_max_xp"), &PlayerProfile::get_max_xp);

    ClassDB::bind_method(D_METHOD("add_xp", "amount"), &PlayerProfile::add_xp);

    ADD_SIGNAL(MethodInfo("profile_updated"));
    ADD_SIGNAL(MethodInfo("level_up_achieved", PropertyInfo(Variant::INT, "new_level")));

}

PlayerProfile::PlayerProfile() {
    current_XP = 0;
    max_XP= 100;
    current_level = 1;
    player_name = "Player";

}

PlayerProfile::~PlayerProfile() {}

void PlayerProfile::add_xp(int amount) {
    current_XP += amount;
    bool leveled_up = false;

    if (current_XP >= max_XP){
        current_level++;
        current_XP -= max_XP;
        max_XP = (int)(max_XP*1.5);
        leveled_up = true;
    }

    emit_signal("profile_updated");
    
    if (leveled_up) {
        UtilityFunctions::print("GLOBAL LEVEL UP! Now level: ", current_level);
        emit_signal("level_up_achieved", current_level);
    }
}