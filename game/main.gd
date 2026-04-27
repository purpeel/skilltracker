extends Node2D

@onready var tree = $Tree
@onready var manager = $Manager
@onready var camera = $Tree/Camera2D 
@onready var fog = $Tree/FogOfWar
@onready var back_btn = $UI/HUD/TopLeft/VBoxContainer/BackButton

@onready var skill_window = $UI/SkillWindow
@onready var title_label = $UI/SkillWindow/Title
@onready var desc_label = $UI/SkillWindow/Description
@onready var upgrade_btn = $UI/SkillWindow/UpgradeBtn
@onready var close_btn = $UI/SkillWindow/CloseBtn
@onready var lvl_up_btn = $UI/SkillWindow/LvlUpBtn
@onready var extra_info = $UI/SkillWindow/ExtraInfo
@onready var lvl_label = $UI/SkillWindow/LvlLabel

@onready var prog_slider = $UI/SkillWindow/ProgressSlider
@onready var slider_val_label = $UI/SkillWindow/SliderValLabel

@onready var tooltip = $UI/Tooltip
@onready var tooltip_name = $UI/Tooltip/VBoxContainer/NameLabel
@onready var tooltip_info = $UI/Tooltip/VBoxContainer/InfoLabel

@onready var lvl_display = $UI/HUD/TopLeft/VBoxContainer/HBoxContainer/LevelLabel
@onready var xp_bar = $UI/HUD/TopLeft/VBoxContainer/XPBar
@onready var xp_text_label = $UI/HUD/TopLeft/VBoxContainer/XPText

@onready var energy_text = $UI/HUD/TopRight/HBoxContainer/EnergyText
@onready var cooldown_text = $UI/HUD/TopRight/HBoxContainer/CooldownText

@onready var todo_panel = $UI/HUD/ToDoPanel
@onready var todo_toggle_btn = $UI/HUD/ToDoPanel/ToggleBtn
@onready var task_list_container = $UI/HUD/ToDoPanel/ScrollContainer/TaskList

var selected_skill = null 
var is_dragging_camera = false 
var is_todo_open = false

func _ready():
	skill_window.hide()
	tooltip.hide()
	
	todo_toggle_btn.pressed.connect(_on_todo_toggle)
	back_btn.pressed.connect(_on_back_btn_pressed)
	prog_slider.value_changed.connect(_on_slider_changed)
	upgrade_btn.pressed.connect(_on_upgrade_btn_pressed)
	close_btn.pressed.connect(_on_close_btn_pressed)
	lvl_up_btn.pressed.connect(_on_lvl_up_btn_pressed)
	todo_toggle_btn.pressed.connect(_on_todo_toggle)
	
	manager.obligations_updated.connect(refresh_todo_list)
	manager.cooldown_started.connect(update_energy_hud)
	manager.cooldown_finished.connect(update_energy_hud)
	
	update_player_hud()

	var fake_catalog = """[
		{"id": 301, "node_name": "Чтение: 10 страниц", "node_info": "Прочитай 10 страниц.", "node_rarity": 0, "area": 0, "cooldown": "daily", "base_progress": 10, "xp_reward": 20},
		{"id": 302, "node_name": "Скорочтение", "node_info": "Тренировка фокуса на время.", "node_rarity": 1, "area": 0, "cooldown": "daily", "duration_sec": 600, "xp_reward": 50},
		{"id": 201, "node_name": "Питьевой режим", "node_info": "Выпить 2 стакана воды утром.", "node_rarity": 0, "area": 1, "cooldown": "daily", "base_progress": 2, "xp_reward": 15},
		{"id": 202, "node_name": "Отжимания", "node_info": "От пола.", "node_rarity": 0, "area": 1, "cooldown": "daily", "base_progress": 20, "xp_reward": 30},
		{"id": 203, "node_name": "Пробежка", "node_info": "Бежать полчаса.", "node_rarity": 1, "area": 1, "cooldown": "weekly", "duration_sec": 1800, "xp_reward": 100},
		{"id": 401, "node_name": "10 новых слов", "node_info": "В словарь.", "node_rarity": 0, "area": 2, "cooldown": "daily", "base_progress": 10, "xp_reward": 25},
		{"id": 501, "node_name": "Скетчинг", "node_info": "Быстрые наброски.", "node_rarity": 1, "area": 3, "cooldown": "daily", "duration_sec": 600, "xp_reward": 45}
	]"""
	manager.load_catalog_from_json(fake_catalog)

	var fake_user_tree = """[
		{ "id": 1000, "parent": null, "node_name": "ЧТЕНИЕ", "node_info": "Дефолт", "node_state": "active", "node_level": 1, "node_rarity": 0, "area": 0, "base_progress": 1, "current_progress": 0, "target_progress": 1, "xp_reward": 10 },
		{ "id": 1001, "parent": null, "node_name": "СПОРТ", "node_info": "Дефолт", "node_state": "active", "node_level": 1, "node_rarity": 0, "area": 1, "base_progress": 1, "current_progress": 0, "target_progress": 1, "xp_reward": 10 },
		{ "id": 1002, "parent": null, "node_name": "ЯЗЫКИ", "node_info": "Дефолт", "node_state": "active", "node_level": 1, "node_rarity": 0, "area": 2, "base_progress": 1, "current_progress": 0, "target_progress": 1, "xp_reward": 10 },
		{ "id": 1003, "parent": null, "node_name": "ТВОРЧЕСТВО", "node_info": "Дефолт", "node_state": "active", "node_level": 1, "node_rarity": 0, "area": 3, "base_progress": 1, "current_progress": 0, "target_progress": 1, "xp_reward": 10 },
		{ "id": 2003, "parent": 1001, "node_name": "Питьевой режим", "node_info": "2 стакана", "node_state": "active", "node_level": 0, "node_rarity": 0, "area": 1, "base_progress": 2, "current_progress": 0, "target_progress": 2, "xp_reward": 15, "cooldown": "daily" },
		{ "id": 2004, "parent": 1000, "node_name": "Гарри Поттер", "node_info": "Прочесть книгу целиком", "node_state": "active", "node_level": 0, "node_rarity": 2, "area": 0, "base_progress": 1, "current_progress": 0, "target_progress": 1, "xp_reward": 500, "cooldown": "once" }
	]"""
	var parsed_nodes = manager.parse_user_tree(fake_user_tree)

	# 1. ЗАГРУЖАЕМ И РАССТАВЛЯЕМ ОСНОВНЫЕ НОДЫ
	for node in parsed_nodes:
		tree.add_child(node)
		tree.registerNode(node)
		setup_node_graphics(node)

		if node.get_required_prev_skills().size() == 0:
			# Это корни (4 центральных узла)
			var area = node.get_subject_area()
			if area == 0: node.position = Vector2(0, -150)
			if area == 1: node.position = Vector2(150, 0)
			if area == 2: node.position = Vector2(0, 150)
			if area == 3: node.position = Vector2(-150, 0)
			node.set_tree_depth(0)
			node.set_layer_index(0)
		else:
			# Это дочерние узлы из базы
			var parent = tree.find_skill_node(node.get_required_prev_skills()[0])
			# Ставим child_index = 0 по умолчанию. Алгоритм сам их подвинет!
			tree.place_node_on_map(node, parent, 0) 
			
		# Добавляем в To-Do
		if node.get_skill_state() >= 2:
			manager.add_obligation(node)
			
	# 2. ГЕНЕРИРУЕМ ТЕНИ (ВИЛОЧКИ) ДЛЯ ВСЕХ АКТИВНЫХ/ЗАВЕРШЕННЫХ НОД
	# (Делаем это во втором цикле, когда всё дерево уже построено!)
	var all_nodes_in_tree = tree.get_children()
	for child in all_nodes_in_tree:
		if child.has_method("get_skill_state"):
			if child.get_skill_state() >= 2:
				spawn_branches_for_node(child)
	
	# 3. ФИНАЛЬНОЕ ОБНОВЛЕНИЕ UI
	refresh_todo_list()
	
func spawn_branches_for_node(parent_node):
	if parent_node.get_tree_depth() >= 6: return
	spawn_new_random_skill(0, parent_node)
	spawn_new_random_skill(1, parent_node)
	tree.reveal_successors(parent_node)

func _process(_delta):
	check_hover()
	update_fog()
	
	if skill_window.visible and selected_skill != null:
		if selected_skill.is_timer_active():
			upgrade_btn.text = "Завершить досрочно[" + format_time(selected_skill.get_current_timer_sec()) + "]"
			
	if not manager.can_start_new_skill():
		cooldown_text.text = format_time(manager.get_cooldown_time_left())
	else:
		cooldown_text.text = "MAX"

func update_fog():
	var lines_array = PackedVector4Array()
	for child in tree.get_children():
		if not child.has_method("get_skill_state"): continue
		if child.get_skill_state() >= 1: 
			var parents = child.get_required_prev_skills()
			if parents.size() > 0:
				for p_id in parents:
					var pt = tree.find_skill_node(p_id)
					if pt and pt.get_skill_state() >= 1:
						lines_array.append(Vector4(child.global_position.x, child.global_position.y, pt.global_position.x, pt.global_position.y))
			else:
				var c_p = child.global_position
				lines_array.append(Vector4(c_p.x, c_p.y, c_p.x, c_p.y))
	fog.material.set_shader_parameter("lines_pos", lines_array)
	fog.material.set_shader_parameter("lines_count", lines_array.size())
		
func check_hover():
	var mp = get_global_mouse_position()
	var found = null
	for child in tree.get_children():
		if not child.has_method("get_skill_state"): continue
		if child.get_skill_state() == 0: continue 
		if child.global_position.distance_to(mp) < 45.0:
			found = child
			break
	if found != null: show_tooltip(found)
	else: tooltip.hide()

func show_tooltip(node):
	if skill_window.visible:
		tooltip.hide()
		return
	tooltip_name.text = node.get_skill_name()
	if node.get_skill_state() == 1: tooltip_info.text = "Скрытый навык"
	else: 
		if node.get_skill_level() == 0: tooltip_info.text = "Таргет фиксирован" 
		else: tooltip_info.text = "Уровень: " + str(node.get_skill_level())
	tooltip.global_position = get_viewport().get_mouse_position() + Vector2(20, -50)
	tooltip.show()

func _unhandled_input(event):
	if event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_RIGHT or event.button_index == MOUSE_BUTTON_MIDDLE:
			is_dragging_camera = event.pressed
	elif event is InputEventMouseMotion and is_dragging_camera:
		if camera: camera.position -= event.relative * camera.zoom.x
	
	if event is InputEventMouseButton and event.pressed and event.button_index == MOUSE_BUTTON_LEFT:
		var click_pos = get_global_mouse_position()
		for child in tree.get_children():
			if not child.has_method("get_skill_state"): continue
			if child.get_skill_state() == 0: continue 
			if child.global_position.distance_to(click_pos) < 45.0:
				open_skill_window(child)
				break

func _on_close_btn_pressed():
	skill_window.hide()
	selected_skill = null


# ==========================================
# УМНЫЕ ТЕКСТЫ В ГЛАВНОМ ОКНЕ
# ==========================================
func open_skill_window(node):
	selected_skill = node
	title_label.text = node.get_skill_name()
	desc_label.text = node.get_skill_title()
	prog_slider.hide()
	slider_val_label.hide()
	
	if node.get_skill_level() == 0:
		lvl_label.text = "Фиксированный навык"
		lvl_up_btn.hide()
	else:
		lvl_label.text = "Уровень: " + str(node.get_skill_level())
		if node.get_skill_state() >= 2: lvl_up_btn.show()
		else: lvl_up_btn.hide()
	
	var period_text = ""
	var cd = node.get_cooldown_type()
	if cd == 1: period_text = "[ЕЖЕДНЕВНО] "
	elif cd == 2: period_text = "[ЕЖЕНЕДЕЛЬНО] "
	elif cd == 3: period_text = "[ЕЖЕМЕСЯЧНО] "
	
	var state = node.get_skill_state()
	
	if state == 1: # REVEALED
		extra_info.text = period_text + "\nСТАТУС: Ожидает открытия"
		upgrade_btn.text = "Начать изучение"
		upgrade_btn.show()
		upgrade_btn.disabled = false
		
	elif state == 2: # ACTIVE
		if node.get_task_type() == 1: # ТАЙМЕР
			extra_info.text = period_text + "\nЗадача на время: " + str(node.get_skill_time() / 60) + " мин."
			if node.is_timer_active(): upgrade_btn.text = "Остановить"
			else: upgrade_btn.text = "Запустить Таймер"
		else: # ОБЫЧНЫЙ НАВЫК С ПРОГРЕССОМ
			var c = node.get_skill_cur_prog()
			var n = node.get_skill_nes_prog()
			extra_info.text = period_text + "\nЦель: " + str(c) + " / " + str(n)
			
			var diff = n - c
			if diff > 1:
				prog_slider.show()
				slider_val_label.show()
				prog_slider.min_value = 1
				prog_slider.max_value = diff
				prog_slider.value = 1
				slider_val_label.text = "+1"
				upgrade_btn.text = "Добавить (+1)"
			else:
				upgrade_btn.text = "Отметить выполнение (+1)"
			
		upgrade_btn.show()
		upgrade_btn.disabled = false
		
	elif state == 3: # FINISHED
		extra_info.text = period_text + "\nСТАТУС: Успешно завершено!"
		upgrade_btn.hide()
		
	tooltip.hide() 
	skill_window.show()

# ==========================================
# ПРОКАЧКА 
# ==========================================
func _on_upgrade_btn_pressed():
	if selected_skill == null: return
	var state = selected_skill.get_skill_state()
	
	if state == 1: 
		if manager.request_start_skill():
			selected_skill.set_skill_state(2) 
			manager.add_obligation(selected_skill)
			spawn_branches_for_node(selected_skill) 
			open_skill_window(selected_skill)
			return 
			
	if state == 2: 
		if selected_skill.get_task_type() == 1: # ТАЙМЕР
			if selected_skill.is_timer_active():
				if selected_skill.has_method("force_finish_timer"):
					selected_skill.force_finish_timer() 
			else:
				selected_skill.start_progress_time() 
			manager.add_obligation(selected_skill) 
		else: # ПРОГРЕСС
			var add_amount = 1
			if prog_slider.visible:
				add_amount = int(prog_slider.value)
			
			selected_skill.add_progress(add_amount)
			manager.add_obligation(selected_skill) 
			
		open_skill_window(selected_skill)
		
		if selected_skill.get_skill_state() == 3: # FINISHED
			# Передаем опыт в глобальный профиль!
			Profile.add_xp(selected_skill.get_skill_xp())
			tree.reveal_successors(selected_skill)
			skill_window.hide()
			
func _on_lvl_up_btn_pressed():
	if selected_skill == null: return
	
	var lvl = selected_skill.get_skill_level()
	selected_skill.set_skill_level(lvl + 1)
	selected_skill.refresh_target_by_level()
	
	if selected_skill.get_skill_state() == 3:
		selected_skill.set_skill_state(2)
		
	manager.add_obligation(selected_skill) 
	open_skill_window(selected_skill)
	
# ==========================================
# TO-DO И HUD 
# ==========================================
func _on_todo_toggle():
	is_todo_open = !is_todo_open
	var tw = create_tween()
	if is_todo_open:
		tw.tween_property(todo_panel, "position:y", get_viewport_rect().size.y - todo_panel.size.y, 0.3)
		todo_toggle_btn.text = "Свернуть"
	else:
		tw.tween_property(todo_panel, "position:y", get_viewport_rect().size.y - 40, 0.3)
		todo_toggle_btn.text = "Задачи ^"

func refresh_todo_list():
	for child in task_list_container.get_children():
		child.queue_free()
		
	var tasks = manager.get_all_obligations()
	var active_tasks =[]
	var done_tasks =[]
	
	for t in tasks:
		if t["current"] >= t["target"]: done_tasks.append(t)
		else: active_tasks.append(t)
		
	for t in active_tasks: create_todo_item(t, false)
	for t in done_tasks: create_todo_item(t, true)

func create_todo_item(task, is_done):
	var btn = Button.new()
	btn.flat = true
	btn.alignment = HORIZONTAL_ALIGNMENT_LEFT
	btn.custom_minimum_size = Vector2(280, 35)
	
	var prefix = ""
	if task["cd_type"] == 1: prefix = "[ДЕНЬ] "
	elif task["cd_type"] == 2: prefix = "[НЕДЕЛЯ] "
	elif task["cd_type"] == 3: prefix = "[МЕСЯЦ] "
	
	var progress_txt = str(task["current"]) + "/" + str(task["target"])
	if task["task_type"] == 1: progress_txt = "Таймер"
	
	btn.text = prefix + task["name"] + " (" + progress_txt + ")"
	
	var item_bg = ColorRect.new()
	item_bg.custom_minimum_size = Vector2(0, 35)
	
	if is_done:
		btn.add_theme_color_override("font_color", Color(0.5, 0.5, 0.5))
		item_bg.color = Color(0.1, 0.1, 0.15, 0.8) 
	else:
		item_bg.color = Color(0.2, 0.2, 0.3, 0.8)  
		
	var t_id = task["id"]
	btn.pressed.connect(func():
		var target_node = tree.find_skill_node(t_id)
		if target_node:
			var tw = create_tween()
			# Плавно летим к ноде камерой!
			tw.tween_property(camera, "position", target_node.position, 0.4).set_trans(Tween.TRANS_QUAD)
			open_skill_window(target_node)
	)
	
	item_bg.add_child(btn)
	task_list_container.add_child(item_bg)

func update_player_hud():
	# БЕРЕМ ЦИФРЫ ИЗ ГЛОБАЛЬНОГО С++ ПРОФИЛЯ
	lvl_display.text = "Level " + str(Profile.get_level())
	xp_bar.max_value = Profile.get_max_xp()
	
	var tw = create_tween()
	tw.tween_property(xp_bar, "value", Profile.get_xp(), 0.4).set_trans(Tween.TRANS_QUAD)
	
	xp_text_label.text = str(Profile.get_xp()) + " / " + str(Profile.get_max_xp()) + " XP"

func update_energy_hud():
	if manager.can_start_new_skill(): energy_text.text = "Энергия: ГОТОВО"
	else: energy_text.text = "Восстановление..."

func _on_global_level_up(new_level):
	print("ИГРОК ПОЛУЧИЛ УРОВЕНЬ: ", new_level)
	# TODO: Добавить красивую Взрывную анимацию на весь экран!

func format_time(seconds: float) -> String:
	var m = int(seconds / 60.0)
	var s = int(seconds) % 60
	return "%02d:%02d" % [m, s]
	
func _on_slider_changed(value):
	slider_val_label.text = "+" + str(value)
	upgrade_btn.text = "Добавить (+" + str(value) + ")"
	
func spawn_new_random_skill(child_index: int, parent_node: SkillNode):  
	var new_id_base = manager.roll_new_skill(parent_node.get_subject_area())
	if new_id_base == "error": return 
	
	var data = manager.get_skill_data(new_id_base)
	var unique_id = str(new_id_base) + "_" + str(randi() % 10000)
	
	var new_node = SkillNode.new()
	new_node.set_skill_id(unique_id)
	
	if not data.is_empty():
		new_node.set_skill_name(data["node_name"])
		new_node.set_skill_title(data["node_info"])
		new_node.set_skill_rarity(int(data.get("node_rarity", 0)))
		new_node.set_skill_xp(int(data.get("xp_reward", 10)))
		
		# --- ЧИТАЕМ ПЕРИОДИЧНОСТЬ ---
		if data.has("cooldown"):
			var cd = data["cooldown"]
			if cd == "daily": new_node.set_cooldown_type(1)
			elif cd == "weekly": new_node.set_cooldown_type(2)
			elif cd == "monthly": new_node.set_cooldown_type(3)
			else: new_node.set_cooldown_type(0) 
			
		# --- ЧИТАЕМ ТАЙМЕРЫ ---
		if data.has("duration_sec") and data["duration_sec"] != null:
			new_node.set_task_type(1) 
			new_node.set_skill_time(int(data["duration_sec"]))
		else:
			new_node.set_task_type(0) 
			new_node.set_skill_time(-1)
			
		new_node.set_base_progress(int(data.get("base_progress", 1)))
	else:
		new_node.set_skill_name("Секрет")
		new_node.set_base_progress(1)
	
	new_node.set_skill_state(0) 
	new_node.set_skill_subject_area(parent_node.get_subject_area())
	new_node.set_skill_level(1) 
	new_node.refresh_target_by_level() 
	
	var reqs = PackedStringArray()
	reqs.append(parent_node.get_skill_id())
	new_node.set_required_prev_skills(reqs)
	
	tree.add_child(new_node)
	tree.registerNode(new_node)
	setup_node_graphics(new_node)
	tree.place_node_on_map(new_node, parent_node, child_index)
	
func add_player_xp(amount: int):
	# Мы просто передаем XP в глобальный С++ класс! 
	# А он сам крикнет "profile_updated", и полоска на экране красиво проанимируется.
	Profile.add_xp(amount)
	print("Получено XP: ", amount, " | Всего: ", Profile.get_xp())
	
func _on_back_btn_pressed():
	# Переключаемся на сцену-оболочку (shell.tscn)
	get_tree().change_scene_to_file("res://shell.tscn")
	
func setup_node_graphics(node: SkillNode):
	node.set_tex_border(load("res://assets/icons/skill-node.png"))
	node.set_icon_fit_on(load("res://assets/icons/gantelya.png"))
	node.set_icon_fit_off(load("res://assets/icons/gantelya.png")) 
	node.set_icon_read_on(load("res://assets/icons/book_skill_tree.png"))
	node.set_icon_read_off(load("res://assets/icons/book_skill_tree.png"))
	node.set_icon_creativity_on(load("res://assets/icons/lampochka.png"))
	node.set_icon_creativity_off(load("res://assets/icons/lampochka.png"))
