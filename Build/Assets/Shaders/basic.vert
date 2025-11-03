#version 460 core

// a_### = attributes/inputs
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color; // might not need anymore?
layout (location = 2) in vec2 a_texcoord;

out vec3 v_color;
out vec2 v_texcoord;
uniform float u_time;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
	//gl_Position = vec4(a_position + tan(u_time), 1.0);
	float frequency = 2.5;
	float amplitude = 0.3;

	vec3 position = a_position;

	float offset = sin(u_time * frequency + position.y) * amplitude;
	position.x += offset;

	v_color = a_color;
	v_texcoord = a_texcoord;

	//gl_Position = vec4(a_position * offset, 0.2); // Acrobatic effect
	//gl_Position = vec4(position, 1.0); // Sway effect
	//gl_Position = vec4(position - offset, 1.0); // Bounce effect
	//gl_Position = vec4(a_position, 1.0); // Still effect
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0); 
}