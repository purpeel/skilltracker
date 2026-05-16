extends Node

var BASE_URL = "http://127.0.0.1:8000/api" 
var auth_token = ""

signal auth_success
signal auth_failed(msg)
signal load_finished

func _ready():
	if Profile.has_meta("auth_token"):
		auth_token = Profile.get_meta("auth_token")


func _send_request(url_path: String, method: int, body_dict: Dictionary, callback: Callable):
	var request = HTTPRequest.new()
	add_child(request)
	
	var headers = ["Content-Type: application/json"]
	if auth_token != "":
		headers.append("Authorization: Token " + auth_token)
	
	var body_json = ""
	if body_dict.size() > 0:
		body_json = JSON.stringify(body_dict)
		
	request.request_completed.connect(func(_result, _code, _hdrs, _body):
		callback.call(_code, _body)
		request.queue_free()
	)
	
	request.request(BASE_URL + url_path, headers, method, body_json)

func login(username, password):
	_send_request("/login/", HTTPClient.METHOD_POST, {"username": username, "password": password}, _on_login_done)

func _on_login_done(_code, _body):
	if _code == 200:
		var data = JSON.parse_string(_body.get_string_from_utf8())
		auth_token = data.get("token", "")
		Profile.set_meta("auth_token", auth_token)
		auth_success.emit()
	else:
		auth_failed.emit("Ошибка логина!")


func fetch_initial_data():
	_send_request("/catalog/", HTTPClient.METHOD_GET, {}, func(_code1, _body1):
		if _code1 == 200:
			GM.load_catalog_from_json(_body1.get_string_from_utf8())
			
			_send_request("/tree/", HTTPClient.METHOD_GET, {}, func(_code2, _body2):
				if _code2 == 200:
					GM.parse_user_tree(_body2.get_string_from_utf8())
					load_finished.emit()
				else:
					print("Ошибка загрузки дерева!")
			)
	)


func push_action(skill_id: String, added_progress: int, xp_reward: int, task_type: int):
	var payload = {
		"skill_id": skill_id,
		"progress": added_progress,
		"xp": xp_reward,
		"task_type": task_type,
		"timestamp": floor(Time.get_unix_time_from_system())
	}
	_send_request("/action/", HTTPClient.METHOD_POST, payload, func(_code, _body):
		if _code != 200:
			print("СЕРВЕР НЕ ПРИНЯЛ ПРОГРЕСС! Ошибка ", _code)
	)
