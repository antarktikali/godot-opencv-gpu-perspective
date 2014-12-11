extends Camera

var perspective
var camera_default_transform

func _ready():
	camera_default_transform = get_camera_transform()
	perspective = OpenCVPerspective.new()
	if perspective.start():
		set_process(true)
	else:
		print("Could not connect to the camera :(")
	pass

func _process(delta):
	var transform = camera_default_transform
	
	var x = -0.4 * perspective.get_horizontal_offset()
	var y = -0.4 * perspective.get_vertical_offset()
	transform = transform.rotated(Vector3(0,1,0), -0.3*x)
	transform = transform.rotated(Vector3(1,0,0), -0.3*y)
	transform = transform.translated(Vector3(x,0,0))
	transform = transform.translated(Vector3(0,y,0))
	
	set_transform(transform)
	