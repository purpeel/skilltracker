extends Control

@onready var content_container = $VBoxContainer/MainContent

@onready var btn_tree = $VBoxContainer/NavBar/HBoxContainer/BtnTree
@onready var btn_stats = $VBoxContainer/NavBar/HBoxContainer/BtnStats
@onready var btn_profile = $VBoxContainer/NavBar/HBoxContainer/BtnProfile

# Обязательно проверь, чтобы пути к файлам сцен были правильными!
var scene_tree = preload("res://main.tscn")
var scene_stats = preload("res://stats.tscn") # Скоро создадим
var scene_profile = preload("res://profile.tscn") # Скоро создадим

var current_scene_node = null

func _ready():
	# Подключаем нижние кнопки
	btn_tree.pressed.connect(func(): load_tab(scene_tree))
	btn_stats.pressed.connect(func(): load_tab(scene_stats))
	btn_profile.pressed.connect(func(): load_tab(scene_profile))
	
	# По умолчанию при старте открываем вкладку с Деревом!
	load_tab(scene_tree)

func load_tab(scene_resource):
	# 1. Если там уже открыта какая-то вкладка, удаляем её из памяти
	if current_scene_node != null:
		current_scene_node.queue_free()
		
	# 2. Создаем новую вкладку из файла сцены (например, main.tscn)
	current_scene_node = scene_resource.instantiate()
	
	# 3. Добавляем её в контейнер (она займет 90% экрана над меню)
	content_container.add_child(current_scene_node)
