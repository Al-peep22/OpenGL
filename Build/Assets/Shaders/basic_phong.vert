#version 460 core

// a_### = attributes/inputs
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

out vec2 v_texcoord;
out vec3 v_position;
out vec3 v_normal;

struct Material{
	sampler2D baseMap;
	vec3 baseColor;
	float shininess;
	vec2 tiling;
	vec2 offset;
};

uniform Material u_material;
uniform float u_time;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
	v_texcoord = a_texcoord;
	//gl_Position = vec4(a_position + tan(u_time), 1.0);
	float frequency = 2.5;
	float amplitude = 0.3;

	//vec3 position = a_position;
	mat4 model_view = u_view * u_model;
	v_position = vec3(model_view * vec4(a_position, 1.0));
	v_normal = normalize(mat3(model_view) * a_normal);

	//gl_Position = vec4(a_position * offset, 0.2); // Acrobatic effect
	//gl_Position = vec4(position, 1.0); // Sway effect
	//gl_Position = vec4(position - offset, 1.0); // Bounce effect
	//gl_Position = vec4(a_position, 1.0); // Still effect
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0); 
}