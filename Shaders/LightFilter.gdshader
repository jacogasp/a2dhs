shader_type canvas_item;
uniform vec2 light_position = vec2(0.5, 0.5);
uniform vec2 heading = vec2(1.0, 0.0);
uniform float fall_off = 1.0f;
uniform float aspect_ratio = 1.0f;
uniform float half_vision_angle = 0.7854; // PI / 4

float dist(vec2 p0, vec2 p1) {
	return sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
}

float when_gt(float x, float y) {
    return max(sign(x - y), 0.0);
}

void fragment() {
	vec4 bg = texture(SCREEN_TEXTURE, SCREEN_UV);
	vec2 uv = UV;
	uv.y *= aspect_ratio;
	vec2 p = light_position * vec2(1.0, aspect_ratio);
	
	float d = dist(p, uv) * fall_off;
	
	vec2 lightDir = normalize(uv - p);
    vec2 spotDir = normalize(heading);
    vec4 mask = vec4(0., 0., 0., 1.);
    mask += vec4(1. - d) * when_gt(dot(lightDir, spotDir), cos(half_vision_angle));
	mask.w = 1.0;
	COLOR = bg * mask ;
}