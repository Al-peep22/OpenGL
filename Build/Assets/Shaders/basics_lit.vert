#version 460 core

// a_### = attributes/inputs
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

out vec3 v_color;
out vec2 v_texcoord;
uniform float u_time;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec3 u_ambient_light;

uniform struct light{
	vec3 position;
	vec3 color;
} u_light;

vec3 calculateLight(in vec3 position, in vec3 normal){
	//direction from surface to light
	vec3 light_dir = normalize(u_light.position - position);

	//difusse lighting (lambertian)
	float intensity = max(dot(normal,light_dir),0);
	vec3 diffuse = u_light.color * intensity;

	// specular
	vec3 reflection = reflect(-light_dir, normal);
	vec3 view_dir = normalize(-position);
	intensity = max(dot(reflection,view_dir),0);
	intensity = pow(intensity, 128);
	vec3 specular = vec3(intensity);

	//final light (ambiant * diffuse)
	return u_ambient_light + diffuse + specular;
}

void main() {
	v_texcoord = a_texcoord;
	//gl_Position = vec4(a_position + tan(u_time), 1.0);
	float frequency = 2.5;
	float amplitude = 0.3;

	//vec3 position = a_position;
	mat4 model_view = u_view * u_model;
	vec3 position = vec3(model_view * vec4(a_position, 1.0));
	vec3 normal = normalize(mat3(model_view) * a_normal);

	float offset = sin(u_time * frequency + position.y) * amplitude;
	position.x += offset;

	v_color = calculateLight(position, normal);
	//gl_Position = vec4(a_position * offset, 0.2); // Acrobatic effect
	//gl_Position = vec4(position, 1.0); // Sway effect
	//gl_Position = vec4(position - offset, 1.0); // Bounce effect
	//gl_Position = vec4(a_position, 1.0); // Still effect
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0); 
}