#version 460 core

#define MAX_LIGHTS		5

#define POINT			0
#define DIRECTIONAL		1
#define SPOT			2

#define BASE_MAP		(1 << 0)
#define SPECULAR_MAP	(1 << 1)
#define EMISSIVE_MAP	(1 << 2)

in VS_OUT
{
    vec2 v_texcoord;
	vec3 v_position;
	vec3 v_normal;
} fs_in;

out vec4 f_color;

struct Light
{
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float range;

	float innerSpotAngle;
	float outerSpotAngle;
};


struct Material{
	vec3 baseColor;
	vec3 emissiveColor;
	float shininess;
	vec2 tiling;
	vec2 offset;
	uint parameters;
};


uniform vec3 u_ambient_light;
uniform Light u_lights[MAX_LIGHTS];
uniform int u_numLights;
uniform Material u_material;

uniform sampler2D u_baseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_emissiveMap;

float calculateAttenuation(float light_distance, float range)
{
    float attenuation = max(0.0, 1.0 - (light_distance / range));
    return attenuation * attenuation;
}

vec3 calculateLight(in Light light, in vec3 position, in vec3 normal, in float specularMask)
{
    float attenuation = 1.0;
    vec3 light_dir;
    switch(light.type){

        case POINT:
        {
            light_dir = normalize(light.position - position);

            float light_distance = length(light.position - position);
            float attenuation = calculateAttenuation(light_distance, light.range);
        }
        break;

        case DIRECTIONAL:
        {
            light_dir = light.direction;
			attenuation = 1.0;
        }
        break;

        case SPOT:
        {
			light_dir = normalize(light.position - position);

			float light_distance = length(light.position - position);
            float attenuation = calculateAttenuation(light_distance, light.range);

			float angle = acos(dot(light_dir, light.direction));
			if(angle > light.outerSpotAngle) {
				attenuation = 0.0;
			}else{
                float spotAttenuation = smoothstep(light.outerSpotAngle, light.innerSpotAngle, angle);
                attenuation *= spotAttenuation;
            }
        }
        break;
	}
        //	//difusse lighting (lambertian)
	float intensity = max(dot(normal,light_dir),0);
	vec3 diffuse = light.color * intensity;
    // specular
	vec3 reflection = reflect(-light_dir, normal);
	vec3 view_dir = normalize(-position);
	intensity = max(dot(reflection,view_dir), 0);
	intensity = pow(intensity, u_material.shininess);
	vec3 specular = vec3(intensity) * specularMask;

	return diffuse + specular;
}
void main() {
	float specularMask = ((u_material.parameters & SPECULAR_MAP) != 0u)
		? texture(u_specularMap, fs_in.v_texcoord).r
		: 1.0;

	vec3 color = u_ambient_light;
	for (int i = 0; i < u_numLights; i++) {
		color += calculateLight(u_lights[i], fs_in.v_position, fs_in.v_normal, specularMask);
	}

	vec4 emissive = ((u_material.parameters & EMISSIVE_MAP) != 0u)
		? texture(u_emissiveMap, fs_in.v_texcoord) * vec4(u_material.emissiveColor, 1)
		: vec4(u_material.emissiveColor, 1);

	f_color = texture(u_baseMap, fs_in.v_texcoord) * vec4(color, 1) + emissive;
}

