
shader_type canvas_item;
uniform vec2 light_position;
uniform vec2 heading = vec2(1.0, 0.0);
uniform vec2 node_size;
uniform float fall_off = 0.01f;
uniform float aspect_ratio = 0.5;
uniform vec2 offset = vec2(0.0f, 0.0f);
uniform float half_vision_angle = 0.7854; // PI / 4
varying vec2 vert;


float when_gt(float x, float y) {
    return max(sign(x - y), 0.0);
}

void vertex() {
	vert = VERTEX;
}

float dist(vec2 p0, vec2 p1) {
	return sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
}

void fragment() {
	vec2 uv = vert / node_size;
	uv.y *= aspect_ratio;
	vec2 p = light_position * vec2(1.0, aspect_ratio);
	
	float d = dist(p, uv) * fall_off;
	
	vec2 lightDir = normalize(uv - p);
    vec2 spotDir = normalize(heading);
    vec4 color = vec4(0.0);
    color += vec4(1.0 - d) * when_gt(dot(lightDir, spotDir), cos(half_vision_angle));
	COLOR = color;
}
