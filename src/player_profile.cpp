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

    ClassDB::bind_method(D_METHOD("set_avatar_id", "id"), &PlayerProfile::set_avatar_id);
    ClassDB::bind_method(D_METHOD("get_avatar_id"), &PlayerProfile::get_avatar_id);
    
    ADD_SIGNAL(MethodInfo("profile_updated"));
    ADD_SIGNAL(MethodInfo("level_up_achieved", PropertyInfo(Variant::INT, "new_level")));

}

PlayerProfile::PlayerProfile() {
    current_XP = 0;
    max_XP= 100;
    current_level = 1;
    player_name = "Player";
    avatar_id = 0;

}

PlayerProfile::~PlayerProfile() {}

void PlayerProfile::add_xp(int amount) {
    if (amount <= 0) {
        return;
    }
    
    current_XP += amount;
    bool leveled_up = false;

    while (current_XP >= max_XP) {
        current_XP -= max_XP;
        current_level++;
        max_XP = (int)(max_XP * 1.5);
        emit_signal("level_up_achieved", current_level);
    }
        
    emit_signal("profile_updated");
    
}