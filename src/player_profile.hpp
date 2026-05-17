#ifndef PLAYER_PROFILE_H
#define PLAYER_PROFILE_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

class PlayerProfile : public Node {
    GDCLASS(PlayerProfile, Node)

protected:
    static void _bind_methods();

private:
    int current_XP;
    int max_XP;
    int current_level;
    int avatar_id;
    String player_name;

public:
    PlayerProfile();
    ~PlayerProfile();
    
    String get_player_name() const { return player_name; }
    void set_player_name(String name) { player_name = name; }

    int get_xp() const { return current_XP; }
    void set_xp(int cur_xp) { current_XP = cur_xp; } 

    int get_max_xp() const { return max_XP; }
    void set_max_xp(int max_xp) { max_XP = max_xp; }

    int get_level() const { return current_level; }
    void set_level(int cur_lvl) { current_level = cur_lvl; }

    int get_avatar_id() const { return avatar_id; }
    void set_avatar_id(int p_id) { avatar_id = p_id; emit_signal("profile_updated"); }
    
    void add_xp(int amount);
};

}


#endif  //PLAYER_PROFILE_H


