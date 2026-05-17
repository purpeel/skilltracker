extends Control

@onready var top_bar = $MarginContainer/MainSplit/VBoxContainer/PlayerCard/MarginContainer/VBoxContainer/TopBar
@onready var settings_btn = $MarginContainer/MainSplit/VBoxContainer/PlayerCard/MarginContainer/VBoxContainer/TopBar/Control3/SettingsBtn
@onready var avatar_btn = $MarginContainer/MainSplit/VBoxContainer/PlayerCard/MarginContainer/VBoxContainer/TopBar/Avatar

@onready var nick_label = $MarginContainer/MainSplit/VBoxContainer/PlayerCard/MarginContainer/VBoxContainer/Nickname
@onready var nick_edit = $MarginContainer/MainSplit/VBoxContainer/PlayerCard/MarginContainer/VBoxContainer/NickEdit
@onready var status_label = $MarginContainer/MainSplit/VBoxContainer/PlayerCard/MarginContainer/VBoxContainer/Status

@onready var lvl_label = $MarginContainer/MainSplit/VBoxContainer/PlayerCard/MarginContainer/VBoxContainer/LvlLabel
@onready var xp_bar = $MarginContainer/MainSplit/VBoxContainer/PlayerCard/MarginContainer/VBoxContainer/XPBar
@onready var xp_text = $MarginContainer/MainSplit/VBoxContainer/PlayerCard/MarginContainer/VBoxContainer/XPText

@onready var logout_btn = $MarginContainer/MainSplit/VBoxContainer/PlayerCard/MarginContainer/VBoxContainer/LogoutBtn
@onready var save_btn = $MarginContainer/MainSplit/VBoxContainer/PlayerCard/MarginContainer/VBoxContainer/SaveBtn

@onready var streak_label = $MarginContainer/MainSplit/VBoxContainer/SummaryCard/MarginContainer/VBoxContainer/StreakLb
@onready var sum_tasks_label = $MarginContainer/MainSplit/VBoxContainer/SummaryCard/MarginContainer/VBoxContainer/SumTasksLb
@onready var dom_area_label = $MarginContainer/MainSplit/VBoxContainer/SummaryCard/MarginContainer/VBoxContainer/Label2
@onready var achiev_grid = $MarginContainer/MainSplit/AchievCard/MarginContainer/VBoxContainer/ScrollContainer/AchievGrid

var is_editing = false

var avatar_list = [
	preload("res://assets/bg/avatar2.png"),
	preload("res://assets/bg/p0.png"),
	preload("res://assets/bg/p1.png"),
	preload("res://assets/bg/p2.png")
]
var current_avatar_id = 0

func _ready():
	Profile.profile_updated.connect(update_ui)
	
	logout_btn.pressed.connect(_on_logout)
	settings_btn.pressed.connect(toggle_edit_mode)
	save_btn.pressed.connect(save_profile)
	avatar_btn.pressed.connect(_on_avatar_clicked)
	
	nick_edit.hide()
	save_btn.hide()
	
	update_ui()
	populate_summary()
	populate_achievements()

func update_ui():
	var p_name = Profile.get_player_name()
	if p_name == "": p_name = "Player"
	nick_label.text = p_name
	
	current_avatar_id = Profile.get_avatar_id()
	if current_avatar_id >= 0 and current_avatar_id < avatar_list.size():
		avatar_btn.texture_normal = avatar_list[current_avatar_id]
	
	var lvl = Profile.get_level()
	lvl_label.text = "Уровень " + str(lvl)
	
	if lvl <= 3: status_label.text = "Начинающий пользователь"
	elif lvl <= 7: status_label.text = "Опытный пользователь"
	elif lvl <= 15: status_label.text = "Мастер"
	else: status_label.text = "Легенда"       #допридумать норм названия....
	
	xp_bar.max_value = Profile.get_max_xp()
	var tw = create_tween()
	tw.tween_property(xp_bar, "value", Profile.get_xp(), 0.5).set_trans(Tween.TRANS_QUAD)
	xp_text.text = str(Profile.get_xp()) + " / " + str(Profile.get_max_xp()) + " XP"

func populate_summary():
	var streak = GM.calculate_current_streak()
	streak_label.text = "Серия: " + str(streak) + " дней"
	
	var history = []
	if GM.has_method("get_action_history"): history = GM.get_action_history()
	
	sum_tasks_label.text = "Выполнено действий: " + str(history.size())
	
	var fav_skill = "Чтение"
	if history.size() > 0:
		dom_area_label.text = "Последний активный: ID " + history[history.size() - 1].get("skill_id", "Отжимания")
	else:
		dom_area_label.text = "Лучший навык: Нет данных"

func toggle_edit_mode():
	is_editing = !is_editing
	if is_editing:
		nick_label.hide()
		nick_edit.show()
		nick_edit.text = Profile.get_player_name()
		
		logout_btn.hide()
		save_btn.show()
		settings_btn.modulate = Color(1.5, 1.5, 2.0)
	else:
		save_profile()

func save_profile():
	is_editing = false
	var new_name = nick_edit.text.strip_edges()
	if new_name != "":
		Profile.set_player_name(new_name)
		
	# Сохраняем аву в С++
	Profile.set_avatar_id(current_avatar_id)
	
	nick_edit.hide()
	nick_label.show()
	save_btn.hide()
	logout_btn.show()
	settings_btn.modulate = Color(1.0, 1.0, 1.0)

func _on_avatar_clicked():
	if is_editing:
		current_avatar_id += 1
		if current_avatar_id >= avatar_list.size(): current_avatar_id = 0
		avatar_btn.texture_normal = avatar_list[current_avatar_id]

# ==========================================
# ДОСТИЖЕНИЯ
# ==========================================
func populate_achievements():
	for child in achiev_grid.get_children(): child.queue_free()
	var fake_achievs =[
		{"name": "Первый шаг", "desc": "Завершите свой первый навык", "unlocked": true},
		{"name": "Неделя в тонусе", "desc": "Выполняйте дейлики 7 дней подряд", "unlocked": true},
		{"name": "Книжный червь", "desc": "Достигните 5 ур. в ветке Чтения", "unlocked": false},
		{"name": "Железный пресс", "desc": "Сделайте 1000 отжиманий", "unlocked": false},
		{"name": "Полиглот", "desc": "Выучите 500 новых слов", "unlocked": false},
		{"name": "Творец", "desc": "Закончите ветку Творчества", "unlocked": false},
		{"name": "Бессонница", "desc": "Прокачайте навык ровно в 3:00 ночи", "unlocked": true}
	]
	for ach in fake_achievs: create_achiev_card(ach["name"], ach["desc"], ach["unlocked"])

func create_achiev_card(a_name: String, a_desc: String, is_unlocked: bool):
	var card = PanelContainer.new()
	card.custom_minimum_size = Vector2(250, 120) 
	card.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	
	var sb = StyleBoxFlat.new()
	sb.corner_radius_top_left = 15; sb.corner_radius_top_right = 15
	sb.corner_radius_bottom_left = 15; sb.corner_radius_bottom_right = 15
	
	if is_unlocked:
		sb.bg_color = Color(0.25, 0.15, 0.4, 0.85) 
		sb.border_width_left = 2; sb.border_width_right = 2; sb.border_width_top = 2; sb.border_width_bottom = 2
		sb.border_color = Color(0.6, 0.3, 0.9, 0.6) 
	else:
		sb.bg_color = Color(0.12, 0.08, 0.18, 0.7)
		
	card.add_theme_stylebox_override("panel", sb)
	
	var margin = MarginContainer.new()
	margin.add_theme_constant_override("margin_left", 15)
	margin.add_theme_constant_override("margin_right", 15)
	margin.add_theme_constant_override("margin_top", 15)
	margin.add_theme_constant_override("margin_bottom", 15)
	card.add_child(margin)
	
	var vbox = VBoxContainer.new()
	vbox.alignment = BoxContainer.ALIGNMENT_CENTER
	margin.add_child(vbox)
	
	var title = Label.new()
	title.text = (" " if is_unlocked else " ") + a_name   #не зебыть подшорузить фотки и вставлять пнгщку замка в заблокированном
	title.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	title.add_theme_font_size_override("font_size", 18) 
	if not is_unlocked: title.add_theme_color_override("font_color", Color(0.5, 0.5, 0.5))
	vbox.add_child(title)
	
	var desc = Label.new()
	desc.text = a_desc
	desc.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	desc.autowrap_mode = TextServer.AUTOWRAP_WORD
	desc.add_theme_font_size_override("font_size", 13) 
	desc.add_theme_color_override("font_color", Color(0.9, 0.9, 1.0) if is_unlocked else Color(0.4, 0.4, 0.4))
	vbox.add_child(desc)
	
	achiev_grid.add_child(card)

func _on_logout():
	Profile.set_player_name("") 
	Profile.set_meta("auth_token", "")
	get_tree().change_scene_to_file("res://login.tscn")
