#version 460 core

in vec3 v_color;
in vec2 v_texcoord;
in vec2 v_normal;

out vec4 f_color;

struct light{
	vec3 position;
	vec3 color;
};

struct Material{
	sampler2D baseMap;
	vec3 baseColor;
	float shininess;
	vec2 tiling;
	vec2 offset;
};

uniform float u_time;
uniform sampler2D u_texture;
uniform Material u_material;
uniform vec3 u_ambient_light;

void main() {
	//f_color = vec4(0, 1, 1, 1);
	//float offset = sin(u_time + gl_FragCoord.x);
	//f_color = vec4(v_color * offset, 1);

	f_color = texture(u_texture, v_texcoord) * vec4(v_color, 1);
}

