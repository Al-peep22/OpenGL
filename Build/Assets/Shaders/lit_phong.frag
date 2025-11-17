#version 460 core
 
#define MX_LIGHTS   5
#define POINT       0
#define DIRECTIONAL 1
#define SPOT        2
 
#define BASE_MAP	  (1 << 0)
#define SPECULAR_MAP (1 << 1)
#define EMISSIVE_MAP (1 << 2)
#define NORMAL_MAP   (1 << 3)
 
in VS_OUT {
    vec2 v_texcoord;
    vec3 v_position; 
    vec3 v_normal;
    mat3 v_tbn; 
} fs_in;
 
out vec4 f_color;
 
struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float range;
    float outerSpotAngle;
    float innerSpotAngle;
};
 
struct Material {
    vec3 baseColor;
    vec3 emissiveColor;
    float shininess;
    vec2 tiling;
    vec2 offset;
    uint parameters;
};
 
uniform vec3 u_ambient_light;
uniform int u_numLights;
uniform Light u_lights[MX_LIGHTS];
uniform Material u_material;
 
uniform sampler2D u_baseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_emissiveMap;
uniform sampler2D u_normalMap;
 
float calculateAttenuation(float light_distance, float range)
{
    float att = max(0.0, 1.0 - light_distance / range);
    return att * att;
}
 
vec3 calculateNormal()
{
    // Sample the normal from the normal map
    vec3 normal = texture(u_normalMap, fs_in.v_texcoord).rgb;
 
    // Convert from [0,1] -> [-1,1]
    normal = normal * 2.0 - 1.0;
 
    // OPTIONAL: Flip green channel if lighting appears inverted
    // normal.y = -normal.y;
 
    // Transform to view/model space
    normal = normalize(fs_in.v_tbn * normal);
 
    return normal;
}
 
vec3 calculateLight(in Light light, in vec3 position, in vec3 normal, in float specularMask)
{
    float attenuation = 1.0;
    vec3 light_dir = vec3(0);
 
    switch(light.type)
    {
        case POINT:
        {
            light_dir = normalize(light.position - position);
            float dist = length(light.position - position);
            attenuation = calculateAttenuation(dist, light.range);
        } break;
 
        case DIRECTIONAL:
        {
            light_dir = normalize(light.direction);
            attenuation = 1.0;
        } break;
 
        case SPOT:
        {
            light_dir = normalize(light.position - position);
            float dist = length(light.position - position);
            attenuation = calculateAttenuation(dist, light.range);
 
            float theta = acos(dot(light_dir, light.direction));
            if(theta > light.outerSpotAngle)
            {
                attenuation = 0.0;
            }
            else
            {
                float spotAttenuation = smoothstep(light.outerSpotAngle, light.innerSpotAngle, theta);
                attenuation *= spotAttenuation;
            }
        } break;
    }
 
    float intensity = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = light.color * intensity;
 
    vec3 reflection = reflect(-light_dir, normal);
    vec3 view_dir = normalize(-position);
 
    intensity = max(dot(reflection, view_dir), 0.0);
    intensity = pow(intensity, u_material.shininess);
 
    vec3 specular = vec3(intensity) * specularMask;
 
    return (diffuse + specular) * light.intensity * attenuation;
}
 
void main() 
{
    vec3 normal = ((u_material.parameters & NORMAL_MAP) != 0u)
        ? calculateNormal()
        : normalize(fs_in.v_normal);
 
    float specularMask = ((u_material.parameters & SPECULAR_MAP) != 0u)
        ? texture(u_specularMap, fs_in.v_texcoord).r
        : 1.0;
 
    vec3 color = u_ambient_light;
    for (int i = 0; i < u_numLights; i++) {
        color += calculateLight(u_lights[i], fs_in.v_position, normal, specularMask);
    }
 
    vec4 emissive = ((u_material.parameters & EMISSIVE_MAP) != 0u)
        ? texture(u_emissiveMap, fs_in.v_texcoord) * vec4(u_material.emissiveColor, 1)
        : vec4(u_material.emissiveColor, 1);
 
    f_color = texture(u_baseMap, fs_in.v_texcoord) * vec4(color, 1) + emissive;
}