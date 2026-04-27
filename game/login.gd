extends Control

func _ready():
	$VBoxContainer/LoginBtn.pressed.connect(_on_login_pressed)

func _on_login_pressed():
	var nick = $VBoxContainer/NickInput.text
	
	if nick != "":
		Profile.set_player_name(nick) # Сохраняем ник в наш С++ класс на всю игру!
	
	# ЗАПУСКАЕМ ГЛАВНУЮ ИГРУ (ДЕРЕВО)
	get_tree().change_scene_to_file("res://main.tscn")
