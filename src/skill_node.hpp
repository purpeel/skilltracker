#ifndef SKILL_NODE_H
#define SKILL_NODE_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/gpu_particles2d.hpp>
#include <godot_cpp/classes/particle_process_material.hpp>
#include <godot_cpp/classes/canvas_item_material.hpp>
#include <godot_cpp/classes/point_light2d.hpp>

namespace godot {

class SkillNode : public Node2D {
    GDCLASS(SkillNode, Node2D)

public:                         //STATES
    enum SkillState {
        STATE_HIDDEN = 0,       //не видно
        STATE_REVEALED = 1,     //видно, залочено
        STATE_ACTIVE = 2,       //разблочено, активно
        STATE_FINISHED =3       //закончен (для временных)
    };

    enum SkillRarity {          //редкости навыка
        RARITY_COMMON = 0,
        RARITY_RARE =1 ,
        RARITY_EPIC = 2,
        RARITY_LEGENDARY = 3
    };

    enum SkillSubjectArea {     //тип (область/направление) прокачки
        AREA_READING = 0,
        AREA_FITNESS = 1,
        AREA_LANGUAGE = 2,
        AREA_CREATIVITY = 3,    //?? мб потом область поменять нужно на что то более значимое
        AREA_CUSTOM = 4,
    };

    enum CooldownType {         //ну пон чист
        CD_ONCE = 0, 
        CD_DAILY = 1, 
        CD_WEEKLY = 2, 
        CD_MONTHLY = 3 
    };

    enum TaskType {             //ну пон чист
        TASK_MANUAL = 0, 
        TASK_TIMER = 1 
    };


private:
    //узлы(рамки)
    Sprite2D* border_sprite;
    Sprite2D* icon_sprite;
    Sprite2D* flash_sprite;             //вспышка
    GPUParticles2D* stars_emit;         //система частиц

    //текстуры
    Ref<Texture2D> tex_border;          //кружок для навыков
    Ref<Texture2D> icon_read_on;        //книга/вкл
    Ref<Texture2D> icon_read_off;       //     /выкл(НЕ ПРОКАЧЕНО, все что ниже аналогично)

    Ref<Texture2D> icon_fit_on;         //фитнесс
    Ref<Texture2D> icon_fit_off;

    Ref<Texture2D> icon_language_on;    //язык
    Ref<Texture2D> icon_language_off; 

    Ref<Texture2D> icon_creativity_on;  //креативность
    Ref<Texture2D> icon_creativity_off;

    Ref<Texture2D> icon_custom_on;      //кастомная
    Ref<Texture2D> icon_custom_off;

    Ref<Texture2D> tex_flash;           //блик
    Ref<Texture2D> tex_star;            //звездочка
    
    void updateVisuals(bool animate = true);             //для переключения визуала
    void _setup_particles();                             //настройка частиц кодом

    SkillState last_state;        //для отслежки изменений
    Ref<Tween> active_tween;      //для анимайии
    Ref<CanvasItemMaterial> flash_material;

    Sprite2D* shadow_sprite;    //тень
    PointLight2D* node_light;   //свет для тумана

private:
    String skill_id;                    //идентификатор
    String skill_name;                  //имя
    String skill_title;                 //описание навыка

    SkillState current_state;           //текущее состояние
    SkillRarity current_rare;           //редкость
    SkillSubjectArea current_subject;   //направление прокачки

    int received_xp;                    //сколько xp получит

    int base_progress;
    int current_progress;               //насколько выполнен навык в данный момент
    int necessary_progress;             //сколько нужно выполнить
    int current_level;

    int tree_depth;
    int layer_index;
    
    String icon_path;                   //путь к картинкек

    int time_for_upgrade;               //время прокачки (не для продолжительных навыков можно сделатт =-1)
    bool is_timer_running;              //начался ли таймер с момента начала прокачки навыка (в пример 5 дней читать по 15 страниц, либо просто таймер начни читать 30 мин книгу)
    float current_timer_sec;            //текущее время таймера
    
    float reset_timer;
    
    PackedStringArray required_prev_skills; //оптимизированный массив id предыдущих node-ов

    CooldownType cooldown_type;
    TaskType task_type;

protected:
    static void _bind_methods();        //тут биндим методы и свойства
    void _notification(int mes);        //инициализация спрайта

public:
    SkillNode();
    ~SkillNode();

    //сеттеры / гетеры
    void setSkillId(const String& id);
    String getSkillId() const;
    void setSkillName(const String& name);
    String getSkillName() const;
    void setSkillTitle(const String& title);
    String getSkillTitle() const;
    void setSkillState(int state);
    int getState() const;
    void setSkillRarity(int rarity);
    int getRarity() const;
    void setSkillSubjectArea(int subj);
    int getSubjectArea() const;
    void setSkillXP(int xp);
    int getSkillXP() const;
    void setSkillTime(int time);
    int getSkillTime() const;
    void setSkillCurProg(int c_prog);
    int getSkillCurProg() const; 
    void setSkillNesProg(int n_prog);
    int getSkillNesProg() const;
    void setSkillLevel(int level);
    int getSkillLevel() const;
    void setRequiredPrevSkills(const PackedStringArray& skills);
    PackedStringArray getRequiredPrevSkills() const;
    void set_tree_depth(int d) { tree_depth = d; }
    int get_tree_depth() const { return tree_depth; }
    void set_layer_index(int idx) { layer_index = idx; }
    int get_layer_index() const { return layer_index; }

    void force_finish_timer();
    
    void setCooldownType(int p_type) { cooldown_type = (CooldownType)p_type; }
    int getCooldownType() const { return (int)cooldown_type; }

    void setTaskType(int p_type) { task_type = (TaskType)p_type; }
    int getTaskType() const { return (int)task_type; }
    
    void set_base_progress(int value) { base_progress = value; }
    int get_base_progress() const { return base_progress; }

    void refresh_target_by_level();
    void setTexFlash(const Ref<Texture2D> p_tex) { 
        tex_flash = p_tex; 
        if (flash_sprite) flash_sprite->set_texture(p_tex); 
    }
    Ref<Texture2D> getTexFlash() const { return tex_flash; }

    void setTexStar(const Ref<Texture2D> p_tex) { 
        tex_star = p_tex; 
        if (stars_emit) stars_emit->set_texture(p_tex); 
    }
    Ref<Texture2D> getTexStar() const { return tex_star; }

    void setTexBorder(const Ref<Texture2D> p_tex) { tex_border = p_tex; updateVisuals(false); }
    Ref<Texture2D> getTexBorder() const { return tex_border; }

    void setIconReadOn(const Ref<Texture2D> p_tex) { icon_read_on = p_tex; updateVisuals(false); }
    Ref<Texture2D> getIconReadOn() const { return icon_read_on; }
    void setIconReadOff(const Ref<Texture2D> p_tex) { icon_read_off = p_tex; updateVisuals(false); }
    Ref<Texture2D> getIconReadOff() const { return icon_read_off; }

    void setIconFitOn(const Ref<Texture2D> p_tex) { icon_fit_on = p_tex; updateVisuals(false); }
    Ref<Texture2D> getIconFitOn() const { return icon_fit_on; }
    void setIconFitOff(const Ref<Texture2D> p_tex) { icon_fit_off = p_tex; updateVisuals(false); }
    Ref<Texture2D> getIconFitOff() const { return icon_fit_off; }

    void setIconLanguageOn(const Ref<Texture2D> p_tex) { icon_language_on = p_tex; updateVisuals(false); }
    Ref<Texture2D> getIconLanguageOn() const { return icon_language_on; }
    void setIconLanguageOff(const Ref<Texture2D> p_tex) { icon_language_off = p_tex; updateVisuals(false); }
    Ref<Texture2D> getIconLanguageOff() const { return icon_language_off; }

    void setIconCreativityOn(const Ref<Texture2D> p_tex) { icon_creativity_on = p_tex; updateVisuals(false); }
    Ref<Texture2D> getIconCreativityOn() const { return icon_creativity_on; }
    void setIconCreativityOff(const Ref<Texture2D> p_tex) { icon_creativity_off = p_tex; updateVisuals(false); }
    Ref<Texture2D> getIconCreativityOff() const { return icon_creativity_off;}

    void setIconCustomOn(const Ref<Texture2D> p_tex) { icon_custom_on = p_tex; updateVisuals(false); }
    Ref<Texture2D> getIconCustomOn() const { return icon_custom_on; }
    void setIconCustomOff(const Ref<Texture2D> p_tex) { icon_custom_off = p_tex; updateVisuals(false); }
    Ref<Texture2D> getIconCustomOff() const { return icon_custom_off; }

    float get_current_timer_sec() const { return current_timer_sec; }
    bool is_timer_active() const { return is_timer_running; }
    Color getRarityColor() const;

    void addProgress(int prog); //добавить выполнение задачи
    void startProgressTime();   //старт таймера
    void _process(double delta) override; //кадры для таймера
};

}

//чтобы godot понимал enum
VARIANT_ENUM_CAST(godot::SkillNode::SkillState);
VARIANT_ENUM_CAST(godot::SkillNode::SkillRarity);
VARIANT_ENUM_CAST(godot::SkillNode::SkillSubjectArea);
VARIANT_ENUM_CAST(godot::SkillNode::CooldownType);
VARIANT_ENUM_CAST(godot::SkillNode::TaskType);

#endif //SKILL_NODE_H