#version 460 core

// a_### = attributes/inputs
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
//layout (location = 2) in vec2 a_uv;

// v_### = varyings (vertex -> fragment)
out vec3 v_color;

// u_### = uniform
uniform float u_time;

void main() {
	v_color = a_color;
	//gl_Position = vec4(a_position + tan(u_time), 1.0);
	float frequency = 3.0;
	float amplitude = 0.3;
	vec3 position = a_position;

	float offset = sin(u_time * frequency + position.y) * amplitude;
	position.x += offset;

	gl_Position = vec4(position, 1.0);
}