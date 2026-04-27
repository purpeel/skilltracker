#include "skill_node.hpp"

#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/interval_tweener.hpp>
#include <godot_cpp/classes/callback_tweener.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp> //для принтов в консоль годота

using namespace godot;

void SkillNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_skill_id", "id"), &SkillNode::setSkillId);
    ClassDB::bind_method(D_METHOD("get_skill_id"), &SkillNode::getSkillId);
    
    ClassDB::bind_method(D_METHOD("set_skill_name", "name"), &SkillNode::setSkillName);
    ClassDB::bind_method(D_METHOD("get_skill_name"), &SkillNode::getSkillName);
    
    ClassDB::bind_method(D_METHOD("set_skill_title", "title"), &SkillNode::setSkillTitle);
    ClassDB::bind_method(D_METHOD("get_skill_title"), &SkillNode::getSkillTitle);

    ClassDB::bind_method(D_METHOD("force_finish_timer"), &SkillNode::force_finish_timer);

    ClassDB::bind_method(D_METHOD("set_skill_state", "state"), &SkillNode::setSkillState);
    ClassDB::bind_method(D_METHOD("get_skill_state"), &SkillNode::getState);

    ClassDB::bind_method(D_METHOD("set_cooldown_type", "type"), &SkillNode::setCooldownType);
    ClassDB::bind_method(D_METHOD("get_cooldown_type"), &SkillNode::getCooldownType);

    ClassDB::bind_method(D_METHOD("set_task_type", "type"), &SkillNode::setTaskType);
    ClassDB::bind_method(D_METHOD("get_task_type"), &SkillNode::getTaskType);

    ClassDB::bind_method(D_METHOD("set_skill_level", "lvl"), &SkillNode::setSkillLevel);
    ClassDB::bind_method(D_METHOD("get_skill_level"), &SkillNode::getSkillLevel);

    ClassDB::bind_method(D_METHOD("set_tree_depth", "d"), &SkillNode::set_tree_depth);
    ClassDB::bind_method(D_METHOD("get_tree_depth"), &SkillNode::get_tree_depth);
    ClassDB::bind_method(D_METHOD("set_layer_index", "idx"), &SkillNode::set_layer_index);
    ClassDB::bind_method(D_METHOD("get_layer_index"), &SkillNode::get_layer_index);
    ClassDB::bind_method(D_METHOD("get_current_timer_sec"), &SkillNode::get_current_timer_sec);
    ClassDB::bind_method(D_METHOD("is_timer_active"), &SkillNode::is_timer_active);

    ClassDB::bind_method(D_METHOD("add_progress", "prog"), &SkillNode::addProgress);
    ClassDB::bind_method(D_METHOD("start_progress_time"), &SkillNode::startProgressTime);
    ClassDB::bind_method(D_METHOD("refresh_target_by_level"), &SkillNode::refresh_target_by_level);
    
    ClassDB::bind_method(D_METHOD("set_skill_xp", "xp"), &SkillNode::setSkillXP);
    ClassDB::bind_method(D_METHOD("get_skill_xp"), &SkillNode::getSkillXP);
    ClassDB::bind_method(D_METHOD("set_skill_cur_prog", "prog"), &SkillNode::setSkillCurProg);
    ClassDB::bind_method(D_METHOD("get_skill_cur_prog"), &SkillNode::getSkillCurProg);
    ClassDB::bind_method(D_METHOD("set_skill_nes_prog", "prog"), &SkillNode::setSkillNesProg);
    ClassDB::bind_method(D_METHOD("get_skill_nes_prog"), &SkillNode::getSkillNesProg);
    ClassDB::bind_method(D_METHOD("set_skill_rarity", "rarity"), &SkillNode::setSkillRarity);
    ClassDB::bind_method(D_METHOD("get_skill_rarity"), &SkillNode::getRarity);
    ClassDB::bind_method(D_METHOD("set_skill_subject_area", "subj"), &SkillNode::setSkillSubjectArea);
    ClassDB::bind_method(D_METHOD("get_subject_area"), &SkillNode::getSubjectArea);
    ClassDB::bind_method(D_METHOD("set_skill_time", "time"), &SkillNode::setSkillTime);
    ClassDB::bind_method(D_METHOD("get_skill_time"), &SkillNode::getSkillTime);
    ClassDB::bind_method(D_METHOD("set_required_prev_skills", "skills"), &SkillNode::setRequiredPrevSkills);
    ClassDB::bind_method(D_METHOD("get_required_prev_skills"), &SkillNode::getRequiredPrevSkills);
    ClassDB::bind_method(D_METHOD("set_base_progress", "value"), &SkillNode::set_base_progress);
    ClassDB::bind_method(D_METHOD("get_base_progress"), &SkillNode::get_base_progress);

    ClassDB::bind_method(D_METHOD("set_tex_border", "tex"), &SkillNode::setTexBorder);
    ClassDB::bind_method(D_METHOD("get_tex_border"), &SkillNode::getTexBorder);
    ClassDB::bind_method(D_METHOD("set_tex_flash", "tex"), &SkillNode::setTexFlash);
    ClassDB::bind_method(D_METHOD("get_tex_flash"), &SkillNode::getTexFlash);
    ClassDB::bind_method(D_METHOD("set_tex_star", "tex"), &SkillNode::setTexStar);
    ClassDB::bind_method(D_METHOD("get_tex_star"), &SkillNode::getTexStar);

    ClassDB::bind_method(D_METHOD("set_icon_read_on", "tex"), &SkillNode::setIconReadOn);
    ClassDB::bind_method(D_METHOD("get_icon_read_on"), &SkillNode::getIconReadOn);

    ClassDB::bind_method(D_METHOD("set_icon_read_off", "tex"), &SkillNode::setIconReadOff);
    ClassDB::bind_method(D_METHOD("get_icon_read_off"), &SkillNode::getIconReadOff);

    ClassDB::bind_method(D_METHOD("set_icon_fit_on", "tex"), &SkillNode::setIconFitOn);
    ClassDB::bind_method(D_METHOD("set_icon_fit_off", "tex"), &SkillNode::setIconFitOff);
    
    ClassDB::bind_method(D_METHOD("set_icon_creativity_on", "tex"), &SkillNode::setIconCreativityOn);
    ClassDB::bind_method(D_METHOD("set_icon_creativity_off", "tex"), &SkillNode::setIconCreativityOff);
    
    ClassDB::bind_method(D_METHOD("set_icon_custom_on", "tex"), &SkillNode::setIconCustomOn);
    ClassDB::bind_method(D_METHOD("set_icon_custom_off", "tex"), &SkillNode::setIconCustomOff);

    ClassDB::bind_method(D_METHOD("set_icon_language_on", "tex"), &SkillNode::setIconLanguageOn);
    ClassDB::bind_method(D_METHOD("set_icon_language_off", "tex"), &SkillNode::setIconLanguageOff);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "skill_id"), "set_skill_id", "get_skill_id");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "current_level"), "set_skill_level", "get_skill_level");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "current_state", PROPERTY_HINT_ENUM, "Hidden,Revealed,Active,Finished"), "set_skill_state", "get_skill_state");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "required_prev_skills"), "set_required_prev_skills", "get_required_prev_skills");
    
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "tex_border", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_tex_border", "get_tex_border");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "tex_flash", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_tex_flash", "get_tex_flash");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "tex_star", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_tex_star", "get_tex_star");

    ADD_SIGNAL(MethodInfo("progress_updated", PropertyInfo(Variant::INT, "current"), PropertyInfo(Variant::INT, "necessary")));
    ADD_SIGNAL(MethodInfo("skill_finished"));
}


//конструктор
SkillNode::SkillNode() {
    tree_depth = 0;
    layer_index = 0;
    shadow_sprite = nullptr;
    node_light = nullptr;
    base_progress = 1;
    current_level = 1;
    cooldown_type = CD_ONCE;
    task_type = TASK_MANUAL;
    reset_timer = 0.0f;

    set_process(true); 
    border_sprite = nullptr;
    icon_sprite = nullptr;
    current_level = 1;

    skill_id = "new_skill";
    skill_name = "New Skill";
    skill_title = "Description";
    
    last_state = STATE_HIDDEN;
    current_state = STATE_HIDDEN;
    current_rare = RARITY_COMMON;
    current_subject = AREA_CUSTOM;

    received_xp = 10;
    current_progress = 0;
    necessary_progress = 100;

    time_for_upgrade = -1;  //автоматом будет стоять чтобы безвременные навыки не показывались
    is_timer_running = false;
    current_timer_sec = 0.0f;
    
}


SkillNode::~SkillNode() {
}


// гетеры/сеттеры...
void SkillNode::setSkillLevel(int level) { current_level = level; }
int SkillNode::getSkillLevel() const { return current_level; }

void SkillNode::setSkillId(const String& id) { skill_id = id; }
String SkillNode::getSkillId() const { return skill_id; }

void SkillNode::setSkillName(const String& name) { skill_name = name; }
String SkillNode::getSkillName() const { return skill_name; }

void SkillNode::setSkillTitle(const String& title) { skill_title = title; }
String SkillNode::getSkillTitle() const { return skill_title; }

void SkillNode::setSkillState(int state) { current_state = (SkillState)state; updateVisuals(true); }
int SkillNode::getState() const { return (int)current_state; }

void SkillNode::setSkillRarity(int rarity) { current_rare = (SkillRarity)rarity; }
int SkillNode::getRarity() const { return (int)current_rare; }

void SkillNode::setSkillSubjectArea(int sub) { current_subject = (SkillSubjectArea)sub;  updateVisuals(false); }
int SkillNode::getSubjectArea() const { return (int)current_subject; }

void SkillNode::setSkillXP(int xp) { received_xp = xp; }
int SkillNode::getSkillXP() const { return received_xp; }

void SkillNode::setSkillTime(int time) { time_for_upgrade = time; }
int SkillNode::getSkillTime() const { return time_for_upgrade; }

void SkillNode::setSkillCurProg(int c_prog) { current_progress = c_prog; }
int SkillNode::getSkillCurProg() const { return current_progress; }

void SkillNode::setSkillNesProg(int n_prog) { necessary_progress = n_prog; }
int SkillNode::getSkillNesProg() const { return necessary_progress; }

void SkillNode::setRequiredPrevSkills(const PackedStringArray& skills) { required_prev_skills = skills; }
PackedStringArray SkillNode::getRequiredPrevSkills() const { return required_prev_skills; }


///____________игровая логика_______________

void SkillNode::addProgress(int prog) {     //добавляем прогресс
    if (current_state != STATE_ACTIVE) {    //искл
        UtilityFunctions::print("CANNOT ADD PROGRESS! SKILL IS NOT ACTIVE!");
        return;
    }

    current_progress += prog;

    if (current_progress >= necessary_progress) {   //достигли необходимого прогресса чтобы завершить?
        current_progress = necessary_progress;
        current_state = STATE_FINISHED;             //меняем состояние на завершенный навык

        emit_signal("progress_updated", current_progress, necessary_progress);
        emit_signal("skill_finished");
        UtilityFunctions::print("Skill ", skill_name, " Finished!!!");
    } else {
        emit_signal("progress_updated", current_progress, necessary_progress);
    }
}

void SkillNode::startProgressTime() {
    if (time_for_upgrade > 0 && current_state == STATE_ACTIVE) {
        is_timer_running = true;
        current_timer_sec = (float)time_for_upgrade;
        UtilityFunctions::print("Timer started for: ", skill_name);
    }
}


//___обновление кадров 
void SkillNode::_process(double delta) {
    //если таймер запущен то отнимаем время
    if (is_timer_running) {
        current_timer_sec -= delta; //delta - доли снкнд между кадрами
        
        if (current_timer_sec <= 0.0f) {
            current_timer_sec = 0.0f;
            is_timer_running = false;
            
            UtilityFunctions::print("Timer finished for: ", skill_name);
            
            //добавить вопрос о выполнелил пользователь задание? если нет, то можно будет приступить позже
            current_progress = necessary_progress; 
            addProgress(0);
        }
    }
}


void SkillNode::_notification(int p_what) {
    if (p_what == NOTIFICATION_READY) {
        if (!shadow_sprite) {
            shadow_sprite = memnew(Sprite2D);
            add_child(shadow_sprite);
            shadow_sprite->set_modulate(Color(0, 0, 0, 0.7)); 
            shadow_sprite->set_position(Vector2(-12, -12)); 
            shadow_sprite->set_z_index(-15); 
        }

        if (!node_light) {
            node_light = memnew(PointLight2D);
            add_child(node_light);
            node_light->set_texture(tex_flash); 
            node_light->set_texture_scale(100.0f);                   //мб обновить потом
            node_light->set_blend_mode(PointLight2D::BLEND_MODE_ADD);
        }
        if (!border_sprite) {
            border_sprite = memnew(Sprite2D);
            add_child(border_sprite);
        }
        if (!icon_sprite) {
            icon_sprite = memnew(Sprite2D);
            add_child(icon_sprite);
        }

        flash_sprite = memnew(Sprite2D);
        add_child(flash_sprite);
        flash_sprite->set_texture(tex_flash);
        flash_sprite->set_modulate(Color(1, 1, 1, 0));
        flash_sprite->set_z_index(150);

        flash_material.instantiate(); // создаем ресурс
        flash_material->set_blend_mode(CanvasItemMaterial::BLEND_MODE_ADD);
        flash_sprite->set_material(flash_material);

        stars_emit = memnew(GPUParticles2D);
        add_child(stars_emit);
        stars_emit->set_texture(tex_star);
        stars_emit->set_emitting(false); 
        stars_emit->set_one_shot(true);  
        stars_emit->set_amount(25);     
        stars_emit->set_z_index(51);
        updateVisuals(false);
    }
}


void SkillNode::_setup_particles() {
    if (!stars_emit) return;

    stars_emit->set_texture(tex_star);
    stars_emit->set_amount(25); 
    stars_emit->set_one_shot(true);
    stars_emit->set_emitting(false);

    Ref<ParticleProcessMaterial> mat;
    mat.instantiate(); 
    
    mat->set_direction(Vector3(0, -1, 0));
    mat->set_spread(200.0f);
    mat->set_param_min(ParticleProcessMaterial::PARAM_INITIAL_LINEAR_VELOCITY, 45.0f);
    mat->set_param_max(ParticleProcessMaterial::PARAM_INITIAL_LINEAR_VELOCITY, 100.0f);
    mat->set_gravity(Vector3(0, 150, 0));
    
    mat->set_param_min(ParticleProcessMaterial::PARAM_SCALE, 0.35f);
    mat->set_param_max(ParticleProcessMaterial::PARAM_SCALE, 0.6f);
    
    stars_emit->set_process_material(mat);
}

void SkillNode::updateVisuals(bool animate) {
    if (!icon_sprite || !border_sprite || !flash_sprite || !shadow_sprite || !node_light || !is_inside_tree()) return;

    bool is_active_now = (current_state == STATE_ACTIVE || current_state == STATE_FINISHED);
    Ref<Texture2D> current_tex;
    switch (current_subject) {
        case AREA_READING:    current_tex = is_active_now ? icon_read_on : icon_read_off; break;
        case AREA_FITNESS:    current_tex = is_active_now ? icon_fit_on  : icon_fit_off; break;
        case AREA_LANGUAGE:   current_tex = is_active_now ? icon_language_on : icon_language_off; break;
        case AREA_CREATIVITY: current_tex = is_active_now ? icon_creativity_on : icon_creativity_off; break;
        default:              current_tex = is_active_now ? icon_custom_on : icon_custom_off; break;
    }

    icon_sprite->set_texture(current_tex);
    border_sprite->set_texture(tex_border);
    shadow_sprite->set_texture(tex_border); 

    Color rarity_color;      
    Color flash_color;       
    float flash_max_scale;   

    switch (current_rare) {
        case RARITY_RARE:      
            rarity_color = Color(0.2, 0.5, 1.0); 
            flash_color = Color(0.8, 1.5, 3.0); 
            flash_max_scale = 0.75f; 
            break;
        case RARITY_EPIC:      
            rarity_color = Color(0.7, 0.2, 0.9); 
            flash_color = Color(2.5, 0.8, 3.0); 
            flash_max_scale = 0.95f;
            break;
        case RARITY_LEGENDARY: 
            rarity_color = Color(1.0, 0.7, 0.1); 
            flash_color = Color(3.5, 2.2, 0.5); 
            flash_max_scale = 1.25f;
            break;
        default:
            rarity_color = Color(0.6, 0.45, 0.35); 
            flash_color = Color(1.5, 1.5, 1.8); 
            flash_max_scale = 0.5f;
            break;
    }
    border_sprite->set_modulate(rarity_color);

    Ref<CanvasItemMaterial> f_mat = flash_sprite->get_material();
    if (f_mat.is_valid()) {
        f_mat->set_light_mode(CanvasItemMaterial::LIGHT_MODE_UNSHADED);
    }

    Color target_mod = Color(1, 1, 1, 1);
    float target_scale = 0.4f;
    float target_light = 0.0f; 
    Tween::TransitionType trans = Tween::TRANS_CUBIC;

    switch (current_state) {
        case STATE_HIDDEN: 
            set_visible(false); 
            last_state = current_state; 
            node_light->set_energy(0.0f); 
            return; 
        case STATE_REVEALED:
            set_visible(true);
            target_mod = Color(0.35, 0.35, 0.35, 0.7);
            target_scale = 0.35f;
            target_light = 0.3f;
            break;
        case STATE_ACTIVE:
            set_visible(true);
            target_mod = Color(1.1, 1.1, 1.1, 1.0); 
            target_scale = 0.4f;
            target_light = 1.2f; 
            trans = Tween::TRANS_BACK;
            break;
        case STATE_FINISHED:
            set_visible(true);
            target_scale = 0.45f;
            target_light = 0.8f; 
            trans = Tween::TRANS_ELASTIC;
            break;
    }

    if (animate) {
        if (active_tween.is_valid() && active_tween->is_running()) active_tween->kill();
        active_tween = get_tree()->create_tween();
        active_tween->set_parallel(true);

        active_tween->tween_property(this, "modulate", target_mod, 0.5);
        active_tween->tween_property(this, "scale", Vector2(target_scale, target_scale), 0.6)
            ->set_trans(trans)->set_ease(Tween::EASE_OUT);
        
        active_tween->tween_property(node_light, "energy", target_light, 0.6);


         if ((last_state == STATE_REVEALED && current_state == STATE_ACTIVE) ||
            (last_state == STATE_ACTIVE && current_state == STATE_FINISHED)) { 
            
            flash_sprite->set_z_index(10);
            flash_sprite->set_scale(Vector2(0.1, 0.1));
            flash_sprite->set_modulate(flash_color); 
            
            Ref<Tween> ft = get_tree()->create_tween();
            ft->set_parallel(true);
            ft->tween_property(flash_sprite, "scale", Vector2(flash_max_scale, flash_max_scale), 0.45)
                ->set_trans(Tween::TRANS_EXPO)->set_ease(Tween::EASE_OUT);
            ft->tween_property(flash_sprite, "modulate:a", 0.0f, 0.5);

            _setup_particles(); 
            if (stars_emit) {
                Color p_col = flash_color; p_col.a = 1.0;
                stars_emit->set_modulate(p_col); 
                stars_emit->restart();
                stars_emit->set_emitting(true);
            }
            
            active_tween->tween_property(icon_sprite, "position", Vector2(5, -5), 0.05);
            active_tween->tween_property(icon_sprite, "position", Vector2(0, 0), 0.1)->set_delay(0.05);
        }
    } else {
        set_modulate(target_mod);
        set_scale(Vector2(target_scale, target_scale));
        node_light->set_energy(target_light);
    }

    last_state = current_state; 
}


void SkillNode::refresh_target_by_level() {
    if (current_level > 0) {
        necessary_progress = base_progress * current_level;
    } else {
        necessary_progress = base_progress; 
    }
    
    UtilityFunctions::print("Target refreshed! New target: ", necessary_progress);
    emit_signal("progress_updated", current_progress, necessary_progress);
}

Color SkillNode::getRarityColor() const {
    switch (current_rare) {
        case RARITY_RARE:      return Color(0.2, 0.5, 1.0);     
        case RARITY_EPIC:      return Color(0.7, 0.2, 0.9);    
        case RARITY_LEGENDARY: return Color(1.0, 0.7, 0.1);     
        default:               return Color(0.6, 0.45, 0.35);   
    }
}


void SkillNode::force_finish_timer() {
    if (is_timer_running) {
        is_timer_running = false;
        current_timer_sec = 0.0f;
        addProgress(necessary_progress - current_progress);
    }
}