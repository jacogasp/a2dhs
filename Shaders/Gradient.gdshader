
shader_type canvas_item;
uniform vec2 mouse_position;
uniform vec2 node_size;
uniform float fall_off = 0.01f;
uniform float aspect_ratio = 0.5;
uniform vec2 offset = vec2(0.0f, 0.0f);


varying vec2 vert;

void vertex() {
	vert = VERTEX;
}

float dist(vec2 p0, vec2 p1) {
	return sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
}

void fragment() {
	vec2 uv = vert / node_size;
	uv.y *= aspect_ratio;
	float d = dist(mouse_position * vec2(1.0, aspect_ratio), uv) * fall_off;
	
	COLOR = vec4(uv, 1, 1);
	COLOR = mix(vec4(1), vec4(0.0), d);
}
