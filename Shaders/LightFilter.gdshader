shader_type canvas_item;
uniform vec2 mouse_position = vec2(0.5, 0.5);
uniform float falloff = 1.0f;
uniform float aspect_ratio = 1.0f;

float dist(vec2 p0, vec2 p1) {
	return sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
}


void fragment() {
	vec4 bg = texture(SCREEN_TEXTURE, SCREEN_UV);
	vec2 uv = UV;
	uv.y *= aspect_ratio;
	float d = dist(mouse_position * vec2(1.0, aspect_ratio), uv) * falloff;
	vec4 mask = mix(vec4(1.0), vec4(0.0, 0.0, 0.0, 1.0), d);
	COLOR = bg * mask;
}