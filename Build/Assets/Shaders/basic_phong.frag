#version 460 core

in vec2 v_texcoord;
in vec3 v_position;
in vec3 v_normal;

out vec4 f_color;

uniform struct light{
	vec3 position;
	vec3 color;
}u_light;

uniform vec3 u_ambient_light;
uniform sampler2D u_texture;

struct Material{
	sampler2D baseMap;
	vec3 baseColor;
	float shininess;
	vec2 tiling;
	vec2 offset;
};

uniform float u_time;
uniform Material u_material;

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
	//f_color = vec4(0, 1, 1, 1);
	//float offset = sin(u_time + gl_FragCoord.x);
	//f_color = vec4(v_color * offset, 1);
	vec3 color = calculateLight(v_position, v_normal);

	f_color = texture(u_texture, v_texcoord) * vec4(color, 1);
}

